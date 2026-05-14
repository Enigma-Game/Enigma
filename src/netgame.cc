/*
 * Copyright (C) 2005 Daniel Heck
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "errors.hh"
#include "actors.hh"
#include "client.hh"
#include "display.hh"
#include "enigma.hh"
#include "main.hh"
#include "netgame.hh"
#include "network.hh"
#include "options.hh"
#include "player.hh"
#include "server.hh"
#include "SoundEffectManager.hh"
#include "video.hh"
#include "world.hh"

#include "lev/Index.hh"
#include "lev/Proxy.hh"

#include "enet/enet.h"
#include "enet_ver.hh"

#include "SDL.h"
#include <cstdint>
#include <string>

using namespace enigma;

#include "client_internal.hh"

//======================================================================
// Wire protocol
//======================================================================

namespace {

constexpr Uint16 PROTO_VERSION = 1;
constexpr int    NET_PORT      = 12345;

// Packet tags. Client -> host inputs live in the 0x10–0x7F band, host
// -> client events in 0x80+. The bands are separate so a malformed
// peer cannot confuse one for the other.
enum ProtoTag : Uint8 {
    // Client -> host (inputs)
    CL_HELLO             = 0x01,
    CL_MOUSE_FORCE       = 0x10,
    CL_ACTIVATE_ITEM     = 0x11,
    CL_ROTATE_INVENTORY  = 0x12,
    CL_COMMAND           = 0x13,
    CL_INHIBIT_PICKUP    = 0x14,

    // Host -> client (outbound EventSink stream)
    SV_HELLO             = 0x7F,
    SV_COMMAND           = 0x80,
    SV_ADVANCE_LEVEL     = 0x81,
    SV_JUMP_BACK         = 0x82,
    SV_LEVEL_LOADED      = 0x83,
    SV_PLAYER_POSITION   = 0x84,
    SV_SPARKLE           = 0x85,
    SV_SOUND             = 0x86,
    SV_SHOW_TEXT         = 0x88,
    SV_SHOW_DOCUMENT     = 0x89,
    SV_FINISHED_TEXT     = 0x8A,
    SV_TEATIME           = 0x8B,
    SV_ERROR             = 0x8C,
    SV_ACTOR_MOVED       = 0x8D,
    SV_ACTOR_SPRITE      = 0x8E,
    SV_GRID_SPRITE       = 0x8F,
    SV_GRID_KILL         = 0x90,
    SV_INVENTORY         = 0x91,
    SV_MOVE_COUNTER      = 0x92,
};

constexpr int CHANNEL_UNRELIABLE = 0;
constexpr int CHANNEL_RELIABLE   = 1;

// Batches outbound EventSink calls into a reliable and an unreliable
// buffer; flush() ships both. Position-style updates that arrive every
// tick go unreliable; everything else is reliable.
class NetworkSink : public client::EventSink {
public:
    explicit NetworkSink(Peer *peer) : m_peer(peer) {}

    void OnCommand(const std::string &cmd) override {
        m_reliable << Uint8(SV_COMMAND) << cmd;
    }
    void OnAdvanceLevel(lev::LevelAdvanceMode mode) override {
        m_reliable << Uint8(SV_ADVANCE_LEVEL) << Uint8(mode);
    }
    void OnJumpBack() override {
        m_reliable << Uint8(SV_JUMP_BACK);
    }
    void OnLevelLoaded(bool isRestart) override {
        m_reliable << Uint8(SV_LEVEL_LOADED) << Uint8(isRestart ? 1 : 0);
    }
    void OnPlayerPosition(unsigned iplayer, const ecl::V2 &pos) override {
        m_unreliable << Uint8(SV_PLAYER_POSITION) << Uint8(iplayer)
                     << float(pos[0]) << float(pos[1]);
    }
    void OnSparkle(const ecl::V2 &pos) override {
        m_reliable << Uint8(SV_SPARKLE) << float(pos[0]) << float(pos[1]);
    }
    void OnShowText(const std::string &text, bool scrolling, double duration) override {
        m_reliable << Uint8(SV_SHOW_TEXT) << text
                   << Uint8(scrolling ? 1 : 0) << double(duration);
    }
    void OnShowDocument(const std::string &text, bool scrolling, double duration) override {
        m_reliable << Uint8(SV_SHOW_DOCUMENT) << text
                   << Uint8(scrolling ? 1 : 0) << double(duration);
    }
    void OnFinishedText() override {
        m_reliable << Uint8(SV_FINISHED_TEXT);
    }
    void OnTeatime(bool onoff) override {
        m_reliable << Uint8(SV_TEATIME) << Uint8(onoff ? 1 : 0);
    }
    void OnError(const std::string &text) override {
        m_reliable << Uint8(SV_ERROR) << text;
    }
    void OnSound(const std::string &soundname, const ecl::V2 &pos,
                 double volume, bool global) override {
        m_reliable << Uint8(SV_SOUND) << soundname
                   << float(pos[0]) << float(pos[1])
                   << double(volume) << Uint8(global ? 1 : 0);
    }
    void OnInventoryChanged(int player_index,
                            const std::vector<std::string> &model_names) override {
        m_reliable << Uint8(SV_INVENTORY) << Uint8(player_index)
                   << Uint16(model_names.size());
        for (auto const &m : model_names)
            m_reliable << m;
    }
    void OnMoveCounter(int value) override {
        m_reliable << Uint8(SV_MOVE_COUNTER) << Uint32(value);
    }
    void OnActorMoved(int actor_id, const ecl::V2 &pos, const ecl::V2 &vel) override {
        m_unreliable << Uint8(SV_ACTOR_MOVED) << Uint32(actor_id)
                     << float(pos[0]) << float(pos[1])
                     << float(vel[0]) << float(vel[1]);
    }
    void OnActorSpriteChanged(int actor_id, const std::string &model_name) override {
        m_reliable << Uint8(SV_ACTOR_SPRITE) << Uint32(actor_id) << model_name;
    }
    void OnGridSpriteChanged(int layer, int x, int y,
                             const std::string &model_name) override {
        m_reliable << Uint8(SV_GRID_SPRITE) << Uint8(layer)
                   << Uint16(x) << Uint16(y) << model_name;
    }
    void OnGridSpriteCleared(int layer, int x, int y) override {
        m_reliable << Uint8(SV_GRID_KILL) << Uint8(layer)
                   << Uint16(x) << Uint16(y);
    }

    void flush() {
        if (m_reliable.size() > 0) {
            m_peer->send_reliable(m_reliable, CHANNEL_RELIABLE);
            m_reliable.clear();
        }
        if (m_unreliable.size() > 0) {
            m_peer->send_message(m_unreliable, CHANNEL_UNRELIABLE);
            m_unreliable.clear();
        }
    }

private:
    Peer *m_peer;
    ecl::Buffer m_reliable;
    ecl::Buffer m_unreliable;
};

// Parse a packet that arrived on the host. `player` is the index
// assigned to that connection (0 or 1); inputs are tagged with it on
// the server side so the simulation knows whose marble to drive.
void dispatch_input_from_client(ecl::Buffer &b, int player) {
    Uint8 tag;
    while (b >> tag) {
        switch (tag) {
        case CL_MOUSE_FORCE: {
            float dx, dy;
            if (b >> dx >> dy)
                server::Msg_MouseForce(player, ecl::V2(dx, dy));
            break;
        }
        case CL_ACTIVATE_ITEM:
            server::Msg_ActivateItem(player);
            break;
        case CL_ROTATE_INVENTORY: {
            Uint8 dir;
            if (b >> dir)
                player::RotateInventory(player, int(Sint8(dir)));
            break;
        }
        case CL_COMMAND: {
            std::string cmd;
            if (b >> cmd)
                server::Msg_Command(cmd);
            break;
        }
        case CL_INHIBIT_PICKUP: {
            Uint8 onoff;
            if (b >> onoff)
                player::InhibitPickup(player, onoff != 0);
            break;
        }
        default:
            enigma::Log << "netgame: unknown CL tag 0x" << std::hex << int(tag) << "\n";
            return;
        }
    }
}

// Parse a packet that arrived on a remote client and apply it to the
// local state. Goes through the existing client::Msg_* and engine
// APIs; since no NetworkSink is registered on the remote, this does
// not re-broadcast.
void dispatch_event_from_server(ecl::Buffer &b) {
    Uint8 tag;
    while (b >> tag) {
        switch (tag) {
        case SV_COMMAND: {
            std::string cmd;
            if (b >> cmd) client::Msg_Command(cmd);
            break;
        }
        case SV_ADVANCE_LEVEL: {
            Uint8 mode;
            if (b >> mode) client::Msg_AdvanceLevel(lev::LevelAdvanceMode(mode));
            break;
        }
        case SV_JUMP_BACK:
            client::Msg_JumpBack();
            break;
        case SV_LEVEL_LOADED: {
            Uint8 restart;
            if (b >> restart) client::Msg_LevelLoaded(restart != 0);
            break;
        }
        case SV_PLAYER_POSITION: {
            Uint8 ip; float x, y;
            if (b >> ip >> x >> y) client::Msg_PlayerPosition(ip, ecl::V2(x, y));
            break;
        }
        case SV_SPARKLE: {
            float x, y;
            if (b >> x >> y) client::Msg_Sparkle(ecl::V2(x, y));
            break;
        }
        case SV_SOUND: {
            std::string sn; float x, y; double v; Uint8 global;
            if (b >> sn >> x >> y >> v >> global)
                sound::EmitSoundEvent(sn, ecl::V2(x, y), v, global != 0);
            break;
        }
        case SV_INVENTORY: {
            Uint8 player_index; Uint16 count;
            if (b >> player_index >> count) {
                std::vector<std::string> models;
                models.reserve(count);
                bool ok = true;
                for (Uint16 i = 0; i < count && ok; ++i) {
                    std::string m;
                    if (b >> m) models.push_back(std::move(m));
                    else ok = false;
                }
                if (ok && int(player_index) == player::CurrentPlayer())
                    display::GetStatusBar()->set_inventory(
                        player_index == 0 ? YIN : YANG, models);
            }
            break;
        }
        case SV_MOVE_COUNTER: {
            Uint32 v;
            if (b >> v)
                display::GetStatusBar()->set_counter(int(v));
            break;
        }
        case SV_SHOW_TEXT: {
            std::string text; Uint8 scr; double dur;
            if (b >> text >> scr >> dur)
                client::Msg_ShowText(text, scr != 0, dur);
            break;
        }
        case SV_SHOW_DOCUMENT: {
            std::string text; Uint8 scr; double dur;
            if (b >> text >> scr >> dur)
                client::Msg_ShowDocument(text, scr != 0, dur);
            break;
        }
        case SV_FINISHED_TEXT:
            client::Msg_FinishedText();
            break;
        case SV_TEATIME: {
            Uint8 on;
            if (b >> on) client::Msg_Teatime(on != 0);
            break;
        }
        case SV_ERROR: {
            std::string text;
            if (b >> text) client::Msg_Error(text);
            break;
        }
        case SV_ACTOR_MOVED: {
            Uint32 idx; float px, py, vx, vy;
            if (b >> idx >> px >> py >> vx >> vy) {
                if (Actor *a = GetActorByIndex(int(idx))) {
                    ActorInfo *ai = a->get_actorinfo();
                    ai->pos = ecl::V2(px, py);
                    ai->vel = ecl::V2(vx, vy);
                    a->move_screen();
                } else {
                    static int miss_count = 0;
                    if (++miss_count <= 5)
                        fprintf(stderr, "CL: SV_ACTOR_MOVED for unknown index %u\n",
                                unsigned(idx));
                }
            }
            break;
        }
        case SV_ACTOR_SPRITE: {
            Uint32 idx; std::string model;
            if (b >> idx >> model) {
                if (Actor *a = GetActorByIndex(int(idx)))
                    a->set_model(model);
            }
            break;
        }
        case SV_GRID_SPRITE: {
            Uint8 layer; Uint16 x, y; std::string model;
            if (b >> layer >> x >> y >> model)
                display::SetModel(GridLoc(GridLayer(layer), GridPos(int(x), int(y))),
                                  model);
            break;
        }
        case SV_GRID_KILL: {
            Uint8 layer; Uint16 x, y;
            if (b >> layer >> x >> y)
                display::KillModel(GridLoc(GridLayer(layer), GridPos(int(x), int(y))));
            break;
        }
        default:
            enigma::Log << "netgame: unknown SV tag 0x" << std::hex << int(tag) << "\n";
            return;
        }
    }
}

}  // anonymous namespace

//======================================================================
// Client-session state and input helpers
//======================================================================

namespace {

bool  s_in_session        = false;  // host or client
bool  s_in_client_session = false;  // client only
Peer *s_client_peer       = nullptr;
ecl::Buffer s_client_out_unreliable;
ecl::Buffer s_client_out_reliable;

void flush_client_outbox() {
    if (s_client_out_unreliable.size() > 0 && s_client_peer) {
        s_client_peer->send_message(s_client_out_unreliable, CHANNEL_UNRELIABLE);
        s_client_out_unreliable.clear();
    }
    if (s_client_out_reliable.size() > 0 && s_client_peer) {
        s_client_peer->send_reliable(s_client_out_reliable, CHANNEL_RELIABLE);
        s_client_out_reliable.clear();
    }
}

}  // namespace

bool netgame::IsClient() {
    return s_in_client_session;
}

bool netgame::IsActive() {
    return s_in_session;
}

void netgame::SendInputMouseForce(const ecl::V2 &f) {
    if (!s_in_client_session) return;
    s_client_out_unreliable << Uint8(CL_MOUSE_FORCE) << float(f[0]) << float(f[1]);
}

void netgame::SendInputActivateItem() {
    if (!s_in_client_session) return;
    s_client_out_reliable << Uint8(CL_ACTIVATE_ITEM);
}

void netgame::SendInputRotateInventory(int dir) {
    if (!s_in_client_session) return;
    s_client_out_reliable << Uint8(CL_ROTATE_INVENTORY) << Uint8(Sint8(dir));
}

void netgame::SendInputCommand(const std::string &cmd) {
    if (!s_in_client_session) return;
    s_client_out_reliable << Uint8(CL_COMMAND) << cmd;
}

void netgame::SendInputInhibitPickup(bool onoff) {
    if (!s_in_client_session) return;
    s_client_out_reliable << Uint8(CL_INHIBIT_PICKUP) << Uint8(onoff ? 1 : 0);
}

//======================================================================
// Handshake and main loops
//======================================================================

namespace {

void log_player_actor_ids(const char *who) {
    for (unsigned p = 0; p < 2; ++p) {
        Actor *a = player::GetMainActor(p);
        if (a)
            fprintf(stderr, "%s: player %u main actor id=%d index=%d\n",
                    who, p, a->getId(), FindActorIndex(a));
        else
            fprintf(stderr, "%s: player %u has no main actor\n", who, p);
    }
}

// Wait up to `timeout_ms` for a single packet to arrive. Returns true
// on success (with the packet's bytes in `out`), false on timeout or
// disconnect.
bool wait_for_packet(Peer *peer, ecl::Buffer &out, int timeout_ms) {
    Uint32 start = SDL_GetTicks();
    while (peer->is_connected()) {
        int dummy;
        if (peer->poll_message(out, dummy))
            return true;
        if ((Uint32)(SDL_GetTicks() - start) >= (Uint32)timeout_ms)
            return false;
        SDL_Delay(5);
    }
    return false;
}

// Run the host's main loop: full simulation, plus broadcast every
// EventSink hit and consume the remote's inputs each tick.
void host_main_loop(Peer *peer) {
    NetworkSink sink(peer);
    client::RegisterEventSink(&sink);

    Uint32 last_tick_time = SDL_GetTicks();
    double dtime = 0;
    while (!client::AbortGameP() && peer->is_connected() && !app.bossKeyPressed) {
        last_tick_time = SDL_GetTicks();

        try {
            client::Tick(dtime);
            server::Tick(dtime);
        } catch (XLevelRuntime &err) {
            client::Msg_Error(std::string("Server Error: level runtime error:\n") + err.what());
            server::Msg_Panic(true);
        }

        ecl::Buffer buf;
        int dummy;
        while (peer->poll_message(buf, dummy))
            dispatch_input_from_client(buf, /*player=*/1);

        sink.flush();

        int sleeptime = 10 - (SDL_GetTicks() - last_tick_time);
        if (sleeptime >= 3)
            SDL_Delay(sleeptime);
        Uint32 now = SDL_GetTicks();
        dtime = (now - last_tick_time) / 1000.0;
        if (fabs(1 - dtime / 0.01) < 0.2)
            dtime = 0.01;
        if (dtime > 500.0)
            dtime = 0.0;
    }

    client::UnregisterEventSink(&sink);
}

// Run the remote's main loop: no physics, just consume state from the
// host and forward local inputs. client::Tick still runs so the local
// display, sound, and follower keep up.
void remote_main_loop(Peer *peer) {
    Uint32 last_tick_time = SDL_GetTicks();
    double dtime = 0;
    while (!client::AbortGameP() && peer->is_connected() && !app.bossKeyPressed) {
        last_tick_time = SDL_GetTicks();

        try {
            client::Tick(dtime);
        } catch (XLevelRuntime &err) {
            client::Msg_Error(std::string("Client Error: level runtime error:\n") + err.what());
            break;
        }

        ecl::Buffer buf;
        int dummy;
        while (peer->poll_message(buf, dummy))
            dispatch_event_from_server(buf);

        flush_client_outbox();

        int sleeptime = 10 - (SDL_GetTicks() - last_tick_time);
        if (sleeptime >= 3)
            SDL_Delay(sleeptime);
        Uint32 now = SDL_GetTicks();
        dtime = (now - last_tick_time) / 1000.0;
        if (fabs(1 - dtime / 0.01) < 0.2)
            dtime = 0.01;
        if (dtime > 500.0)
            dtime = 0.0;
    }
}

}  // anonymous namespace

void netgame::Start() {
    // Host plays whatever level the user has currently selected in the
    // level browser. A proper lobby (host/port/level/color picker) is
    // still missing.
    lev::Index *ind = lev::Index::getCurrentIndex();
    if (ind == nullptr) {
        fprintf(stderr, "SV: no current level pack selected.\n");
        return;
    }
    // Snapshot position once so the proxy we load and the level number
    // we ship to the remote stay consistent even if something else
    // would otherwise advance the index in between.
    int level_pos = ind->getCurrentPosition();
    lev::Proxy *proxy = ind->getProxy(level_pos);
    if (proxy == nullptr) {
        fprintf(stderr, "SV: no current level selected.\n");
        return;
    }
    printf("SV: hosting level pack '%s' level %d (%s)\n",
           ind->getName().c_str(), level_pos + 1,
           proxy->getTitle().c_str());

    ENetAddress network_address;
    network_address.host = ENET_HOST_ANY;
    network_address.port = NET_PORT;

    ENetHost *network_host =
#ifdef ENET_VER_EQ_GT_13
        enet_host_create(&network_address, 1, 2, 0, 0);
#else
        enet_host_create(&network_address, 1, 0, 0);
#endif
    if (network_host == nullptr) {
        fprintf(stderr, "SV: failed to create an ENet host on port %d.\n", NET_PORT);
        return;
    }

    Peer *peer = nullptr;
    printf("SV: waiting for client on port %d (press ESC to cancel)...\n", NET_PORT);
    while (peer == nullptr) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                enet_host_destroy(network_host);
                return;
            }
        }
        ENetEvent event;
        while (enet_host_service(network_host, &event, 0) > 0) {
            if (event.type == ENET_EVENT_TYPE_CONNECT) {
                printf("SV: client connected.\n");
                peer = new Peer_Enet(network_host, event.peer, 2);
            }
        }
        SDL_Delay(10);
    }

    // Handshake. Pick a fresh RNG seed and ship the level coordinates,
    // plus the host's current Object::next_id snapshot so the remote
    // can sync its id allocator and end up with matching actor ids.
    Uint32 seed = Uint32(SDL_GetTicks()) ^ Uint32(uintptr_t(peer));
    Uint32 next_id_snapshot = Uint32(Object::getNextIdSnapshot());
    {
        ecl::Buffer hello;
        hello << Uint8(SV_HELLO)
              << PROTO_VERSION
              << ind->getName()
              << Uint32(level_pos)
              << seed
              << next_id_snapshot
              << Uint8(1);  // remote is player 1
        peer->send_reliable(hello, CHANNEL_RELIABLE);
    }
    ecl::Buffer reply;
    if (!wait_for_packet(peer, reply, /*timeout_ms=*/10000)) {
        fprintf(stderr, "SV: handshake timed out.\n");
        peer->disconnect();
        delete peer;
        enet_host_destroy(network_host);
        return;
    }
    {
        Uint8 tag = 0;
        Uint16 cl_proto = 0;
        reply >> tag >> cl_proto;
        if (tag != CL_HELLO || cl_proto != PROTO_VERSION) {
            fprintf(stderr, "SV: handshake mismatch (tag=0x%x proto=%d).\n", tag, int(cl_proto));
            peer->disconnect();
            delete peer;
            enet_host_destroy(network_host);
            return;
        }
    }

    // Host loads the level locally with the agreed seed. Re-pin the
    // index position too in case it slipped between snapshot and now.
    ind->setCurrentPosition(level_pos);
    server::RandomState = Sint32(seed);
    server::Msg_LoadLevel(proxy, false);
    log_player_actor_ids("SV");

    // Cursor stays visible and mouse stays ungrabbed in network mode
    // so the user can move between two windows on the same machine
    // for testing. This may change once we have a real lobby UX.
    s_in_session = true;
    host_main_loop(peer);
    s_in_session = false;
    fprintf(stderr, "SV: host loop exited (connected=%d, abort=%d)\n",
            int(peer->is_connected()), int(client::AbortGameP()));

    peer->disconnect();
    delete peer;
    enet_host_destroy(network_host);
}

void netgame::Join(std::string hostname, int port) {
    if (port <= 0)
        port = NET_PORT;
    printf("CL: connecting to %s:%d...\n", hostname.c_str(), port);

    ENetHost *network_host = enet_host_create(nullptr, 1,
#ifdef ENET_VER_EQ_GT_13
                                              2,
#endif
                                              0, 0);
    if (network_host == nullptr) {
        fprintf(stderr, "CL: failed to create ENet client host.\n");
        return;
    }

    ENetAddress sv_address;
    enet_address_set_host(&sv_address, hostname.c_str());
    sv_address.port = port;

    ENetPeer *raw_peer =
#ifdef ENET_VER_EQ_GT_13
        enet_host_connect(network_host, &sv_address, 2, 0);
#else
        enet_host_connect(network_host, &sv_address, 2);
#endif
    if (raw_peer == nullptr) {
        fprintf(stderr, "CL: no available peers for connection.\n");
        enet_host_destroy(network_host);
        return;
    }

    ENetEvent event;
    if (enet_host_service(network_host, &event, 5000) <= 0 ||
        event.type != ENET_EVENT_TYPE_CONNECT) {
        fprintf(stderr, "CL: connection to %s:%d failed.\n", hostname.c_str(), port);
        enet_peer_reset(raw_peer);
        enet_host_destroy(network_host);
        return;
    }
    Peer *peer = new Peer_Enet(network_host, raw_peer, 0);

    // Receive SV_HELLO and parse.
    ecl::Buffer hello;
    if (!wait_for_packet(peer, hello, /*timeout_ms=*/10000)) {
        fprintf(stderr, "CL: handshake timed out.\n");
        peer->disconnect();
        delete peer;
        enet_host_destroy(network_host);
        return;
    }
    Uint8 tag = 0;
    Uint16 proto = 0;
    std::string level_pack;
    Uint32 level_idx = 0;
    Uint32 seed = 0;
    Uint32 next_id_snapshot = 0;
    Uint8 assigned_player = 1;
    hello >> tag >> proto >> level_pack >> level_idx >> seed >> next_id_snapshot
          >> assigned_player;
    if (tag != SV_HELLO || proto != PROTO_VERSION) {
        fprintf(stderr, "CL: bad SV_HELLO (tag=0x%x proto=%d).\n", tag, int(proto));
        peer->disconnect();
        delete peer;
        enet_host_destroy(network_host);
        return;
    }
    printf("CL: host wants level pack '%s' level %d, seed=0x%x, next_id=%u, "
           "assigned player=%d\n",
           level_pack.c_str(), int(level_idx) + 1, unsigned(seed),
           unsigned(next_id_snapshot), int(assigned_player));

    if (!lev::Index::setCurrentIndex(level_pack)) {
        fprintf(stderr, "CL: missing level pack '%s'.\n", level_pack.c_str());
        peer->disconnect();
        delete peer;
        enet_host_destroy(network_host);
        return;
    }
    lev::Index *ind = lev::Index::getCurrentIndex();
    if (int(level_idx) >= ind->size()) {
        fprintf(stderr, "CL: level pack only has %d levels, host asked for %d.\n",
                ind->size(), int(level_idx));
        peer->disconnect();
        delete peer;
        enet_host_destroy(network_host);
        return;
    }
    ind->setCurrentPosition(int(level_idx));

    server::RandomState = Sint32(seed);
    Object::setNextId(int(next_id_snapshot));
    server::Msg_LoadLevel(ind->getProxy(int(level_idx)), false);
    player::SetCurrentPlayer(assigned_player);
    log_player_actor_ids("CL");

    // Acknowledge.
    {
        ecl::Buffer ack;
        ack << Uint8(CL_HELLO) << PROTO_VERSION;
        peer->send_reliable(ack, CHANNEL_RELIABLE);
    }

    s_in_session = true;
    s_in_client_session = true;
    s_client_peer = peer;

    remote_main_loop(peer);
    fprintf(stderr, "CL: remote loop exited (connected=%d, abort=%d)\n",
            int(peer->is_connected()), int(client::AbortGameP()));

    flush_client_outbox();
    s_in_client_session = false;
    s_in_session = false;
    s_client_peer = nullptr;

    peer->disconnect();
    delete peer;
    enet_host_destroy(network_host);
}
