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
#include "main.hh"
#include "netgame.hh"
#include "network.hh"
#include "options.hh"
#include "player.hh"
#include "server.hh"

#include "enet/enet.h"
#include "enet_ver.hh"

#include "SDL.h"
#include <string>

using namespace enigma;

#include "client_internal.hh"

//======================================================================
// Wire protocol
//======================================================================

namespace {

// Packet tags. Client -> host inputs live in the 0x10–0x7F band, host
// -> client events in 0x80+. The bands are separate so a malformed
// peer cannot confuse one for the other.
enum ProtoTag : Uint8 {
    // Client -> host (inputs)
    CL_MOUSE_FORCE       = 0x10,
    CL_ACTIVATE_ITEM     = 0x11,
    CL_ROTATE_INVENTORY  = 0x12,
    CL_COMMAND           = 0x13,
    CL_INHIBIT_PICKUP    = 0x14,

    // Host -> client (outbound EventSink stream)
    SV_COMMAND           = 0x80,
    SV_ADVANCE_LEVEL     = 0x81,
    SV_JUMP_BACK         = 0x82,
    SV_LEVEL_LOADED      = 0x83,
    SV_PLAYER_POSITION   = 0x84,
    SV_SPARKLE           = 0x85,
    SV_PLAY_SOUND        = 0x86,
    SV_PLAY_SOUND_REL    = 0x87,
    SV_SHOW_TEXT         = 0x88,
    SV_SHOW_DOCUMENT     = 0x89,
    SV_FINISHED_TEXT     = 0x8A,
    SV_TEATIME           = 0x8B,
    SV_ERROR             = 0x8C,
    SV_ACTOR_MOVED       = 0x8D,
    SV_ACTOR_SPRITE      = 0x8E,
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
    void OnPlaySound(const std::string &soundname, const ecl::V2 &pos,
                     double relative_volume) override {
        m_reliable << Uint8(SV_PLAY_SOUND) << soundname
                   << float(pos[0]) << float(pos[1]) << double(relative_volume);
    }
    void OnPlaySoundRelative(const std::string &soundname,
                             double relative_volume) override {
        m_reliable << Uint8(SV_PLAY_SOUND_REL) << soundname << double(relative_volume);
    }
    void OnError(const std::string &text) override {
        m_reliable << Uint8(SV_ERROR) << text;
    }
    void OnActorMoved(int actor_id, const ecl::V2 &pos, const ecl::V2 &vel) override {
        m_unreliable << Uint8(SV_ACTOR_MOVED) << Uint32(actor_id)
                     << float(pos[0]) << float(pos[1])
                     << float(vel[0]) << float(vel[1]);
    }
    void OnActorSpriteChanged(int actor_id, const std::string &model_name) override {
        m_reliable << Uint8(SV_ACTOR_SPRITE) << Uint32(actor_id) << model_name;
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
            server::Msg_ActivateItem();
            break;
        case CL_ROTATE_INVENTORY: {
            Uint8 dir;
            if (b >> dir)
                player::RotateInventory(int(Sint8(dir)));
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
        case SV_PLAY_SOUND: {
            std::string sn; float x, y; double v;
            if (b >> sn >> x >> y >> v)
                client::Msg_PlaySound(sn, ecl::V2(x, y), v);
            break;
        }
        case SV_PLAY_SOUND_REL: {
            std::string sn; double v;
            if (b >> sn >> v) client::Msg_PlaySound(sn, v);
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
            Uint32 id; float px, py, vx, vy;
            if (b >> id >> px >> py >> vx >> vy) {
                if (Actor *a = dynamic_cast<Actor *>(Object::getObject(int(id)))) {
                    ActorInfo *ai = a->get_actorinfo();
                    ai->pos = ecl::V2(px, py);
                    ai->vel = ecl::V2(vx, vy);
                    a->move_screen();
                }
            }
            break;
        }
        case SV_ACTOR_SPRITE: {
            Uint32 id; std::string model;
            if (b >> id >> model) {
                if (Actor *a = dynamic_cast<Actor *>(Object::getObject(int(id))))
                    a->set_model(model);
            }
            break;
        }
        default:
            enigma::Log << "netgame: unknown SV tag 0x" << std::hex << int(tag) << "\n";
            return;
        }
    }
}

}  // anonymous namespace

namespace {

Uint32 last_tick_time;

}  // namespace

void server_loop(Peer *m_peer) {
    printf("SV: Entered server loop\n");
    server::InitNewGame();

    // The remote is assigned player 1; the host drives player 0.
    NetworkSink sink(m_peer);
    client::RegisterEventSink(&sink);

    double dtime = 0;
    while (!client::AbortGameP() && m_peer->is_connected()) {
        last_tick_time = SDL_GetTicks();

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type && e.key.keysym.sym == SDLK_ESCAPE)
                goto done;
        }

        try {
            client::Tick(dtime);
            server::Tick(dtime);
        } catch (XLevelRuntime &err) {
            client::Msg_Error(std::string("Server Error: level runtime error:\n") + err.what());
            server::Msg_Panic(true);
        }

        ecl::Buffer buf;
        int player_no;
        while (m_peer->poll_message(buf, player_no)) {
            dispatch_input_from_client(buf, player_no);
        }

        sink.flush();

        int sleeptime = 10 - (SDL_GetTicks() - last_tick_time);
        if (sleeptime >= 3)  // only sleep if relatively idle
            SDL_Delay(sleeptime);
        dtime = (SDL_GetTicks() - last_tick_time) / 1000.0;
        if (fabs(1 - dtime / 0.01) < 0.2) {
            // less than 20% deviation from desired frame time?
            dtime = 0.01;
        }

        if (dtime > 500.0) /* Time has done something strange, perhaps
                              run backwards */
            dtime = 0.0;
    }

done:
    client::UnregisterEventSink(&sink);
    return;
}

void netgame::Start() {

    // ---------- Create network host ----------
    ENetHost *network_host;
    ENetAddress network_address;

    network_address.host = ENET_HOST_ANY;
    network_address.port = 12345;

#ifdef ENET_VER_EQ_GT_13
    network_host = enet_host_create(&network_address, 1, 0, 0, 0);
#else
    network_host = enet_host_create(&network_address, 1, 0, 0);
#endif
    if (network_host == NULL) {
        fprintf(stderr, "SV: An error occurred while trying to create an ENet server host.\n");
        return;
    }

    // ---------- Wait for client(s) ----------
    ENetEvent event;
    Peer *m_peer = 0;
    printf("SV: Waiting for client...\n");

    while (!m_peer) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type && e.key.keysym.sym == SDLK_ESCAPE)
                return;
        }

        while (enet_host_service(network_host, &event, 0) > 0) {
            if (event.type == ENET_EVENT_TYPE_CONNECT) {
                printf("SV: Connected to client\n");
                m_peer = new Peer_Enet(network_host, event.peer, 2);
            }
        }
        SDL_Delay(10);
    }

    server_loop(m_peer);

    m_peer->disconnect();
    delete m_peer;
}

//======================================================================
// CLIENT
//======================================================================

namespace {

struct MovementCommand {
    float time_stamp;
    float force_x;
    float force_y;
};

typedef std::list<MovementCommand> MovementList;

MovementList movement_list;

Peer *server_peer;

}  // namespace


void netgame::Join(std::string hostname, int port) {
    printf("CL: trying to join remote game\n");

    // ---------- Create network host ----------
    ENetHost *m_network_host;
    m_network_host = enet_host_create(NULL, 1 /* only allow 1 outgoing connection */,
#ifdef ENET_VER_EQ_GT_13
                                      2 /* 2 channels are sufficient */,
#endif
                                      57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
                                      14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

    if (m_network_host == NULL) {
        fprintf(stderr, "CL: An error occurred while trying to create an ENet client host.\n");
        return;
    }

    // ---------- Connect to server ----------

    ENetAddress sv_address;
    ENetPeer *m_server;

    /* Connect to some.server.net:1234. */
    enet_address_set_host(&sv_address, hostname.c_str());
    sv_address.port = port;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    int numchannels = 2;
#ifdef ENET_VER_EQ_GT_13
    m_server = enet_host_connect(m_network_host, &sv_address, numchannels, 57600);
#else
    m_server = enet_host_connect(m_network_host, &sv_address, numchannels);
#endif

    if (m_server == NULL) {
        fprintf(stderr, "CL: No available peers for initiating an ENet connection.\n");
        return;
    }

    server_peer = 0;
    ENetEvent event;
    if (enet_host_service(m_network_host, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {
        fprintf(stderr, "CL: Connection to some.server.net:12345 succeeded.\n");
        if (m_server != event.peer)
            printf("CL: peers differ!?!\n");
        server_peer = new Peer_Enet(m_network_host, m_server, 0);
    } else
        return;

    ecl::Buffer out_unreliable;
    while (server_peer->is_connected()) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type && e.key.keysym.sym == SDLK_ESCAPE)
                goto done;
            else if (e.type == SDL_MOUSEMOTION) {
                float mouseforce = options::GetDouble("MouseSpeed");
                out_unreliable << Uint8(CL_MOUSE_FORCE)
                               << float(e.motion.xrel * mouseforce)
                               << float(e.motion.yrel * mouseforce);
            }
        }
        if (out_unreliable.size() > 0) {
            server_peer->send_message(out_unreliable, CHANNEL_UNRELIABLE);
            out_unreliable.clear();
        }

        ecl::Buffer buf;
        int peerno;
        while (server_peer->poll_message(buf, peerno)) {
            dispatch_event_from_server(buf);
        }
        SDL_Delay(10);
    }

done:
    server_peer->disconnect();
    delete server_peer;
    server_peer = 0;
    return;
}
