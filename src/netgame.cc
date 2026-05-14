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
#include "Object.hh"
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
#include <random>
#include <set>
#include <string>

using namespace enigma;

#include "client_internal.hh"

//======================================================================
// Wire protocol
//======================================================================

namespace {

constexpr Uint16 PROTO_VERSION = 4;
constexpr int    NET_PORT      = 12345;

// Packet tags. Client -> host inputs live in the 0x10–0x7F band, host
// -> client events in 0x80+. The bands are separate so a malformed
// peer cannot confuse one for the other.
enum ProtoTag : Uint8 {
    // Client -> host (inputs)
    CL_HELLO             = 0x01,
    CL_AUTH              = 0x02,
    CL_MOUSE_FORCE       = 0x10,
    CL_ACTIVATE_ITEM     = 0x11,
    CL_ROTATE_INVENTORY  = 0x12,
    CL_COMMAND           = 0x13,
    CL_INHIBIT_PICKUP    = 0x14,

    // Host -> client (outbound EventSink stream)
    SV_AUTH_OK           = 0x7D,
    SV_AUTH_FAIL         = 0x7E,
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
    SV_ACTOR_ADDED       = 0x93,
    SV_ACTOR_KILLED      = 0x94,
    SV_PAUSE             = 0x95,
    SV_RELOAD            = 0x96,
    SV_RESIZE            = 0x97,
};

constexpr int CHANNEL_UNRELIABLE = 0;
constexpr int CHANNEL_RELIABLE   = 1;

// Commands the remote is allowed to forward to the host. Anything else
// is dropped — LAN trust model, but the door doesn't need to be open
// to cheats and arbitrary jumpto/find traffic.
const std::set<std::string> kAllowedClientCommands = {
    "suicide", "restart", "advance_strict", "advance_unsolved", "jumpback",
};

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
                     << double(pos[0]) << double(pos[1]);
    }
    void OnSparkle(const ecl::V2 &pos) override {
        m_reliable << Uint8(SV_SPARKLE) << double(pos[0]) << double(pos[1]);
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
                   << double(pos[0]) << double(pos[1])
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
    void OnActorMoved(int object_id, const ecl::V2 &pos, const ecl::V2 &vel) override {
        m_unreliable << Uint8(SV_ACTOR_MOVED) << Uint32(object_id)
                     << double(pos[0]) << double(pos[1])
                     << double(vel[0]) << double(vel[1]);
    }
    void OnActorSpriteChanged(int object_id, const std::string &model_name) override {
        m_reliable << Uint8(SV_ACTOR_SPRITE) << Uint32(object_id) << model_name;
    }
    void OnActorAdded(int object_id, const std::string &kind,
                      const ecl::V2 &pos, const ecl::V2 &vel,
                      int owner_player) override {
        // owner: -1 → 0xFF, else 0..1. Decoded via Sint8 round-trip.
        m_reliable << Uint8(SV_ACTOR_ADDED) << Uint32(object_id) << kind
                   << double(pos[0]) << double(pos[1])
                   << double(vel[0]) << double(vel[1])
                   << Uint8(Sint8(owner_player));
    }
    void OnActorKilled(int object_id) override {
        m_reliable << Uint8(SV_ACTOR_KILLED) << Uint32(object_id);
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
    void OnPause(bool onoff) override {
        m_reliable << Uint8(SV_PAUSE) << Uint8(onoff ? 1 : 0);
    }
    void OnReload(int level_idx) override {
        m_reliable << Uint8(SV_RELOAD) << Uint32(level_idx);
    }
    void OnResize(int w, int h) override {
        m_reliable << Uint8(SV_RESIZE) << Uint16(w) << Uint16(h);
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

// Look up an Actor by its host-side Object id. Returns nullptr if no
// such object exists locally or the id resolves to a non-actor.
Actor *find_actor_by_id(Uint32 id) {
    return dynamic_cast<Actor *>(Object::getObject(int(id)));
}

// Parse a packet that arrived on the host. `player` is the index
// assigned to that connection (0 or 1); inputs are tagged with it on
// the server side so the simulation knows whose marble to drive.
void dispatch_input_from_client(ecl::Buffer &b, int player) {
    Uint8 tag;
    while (b >> tag) {
        switch (tag) {
        case CL_MOUSE_FORCE: {
            double dx, dy;
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
            if (b >> cmd) {
                if (kAllowedClientCommands.count(cmd))
                    server::Msg_Command(cmd, player);
                else
                    enigma::Log << "netgame: rejected client command '" << cmd << "'\n";
            }
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
void dispatch_event_from_server(ecl::Buffer &b);

}  // anonymous namespace

//======================================================================
// Client-session state and input helpers
//======================================================================

namespace {

bool  s_in_session        = false;  // host or client
bool  s_in_client_session = false;  // client only
bool  s_paused_by_host    = false;  // remote: host has its menu open
Peer *s_client_peer       = nullptr;
ecl::Buffer s_client_out_unreliable;
ecl::Buffer s_client_out_reliable;

// Per-connection state on the host. Set by Start() before its main
// loop and read both by the dispatcher (to tag inputs with the
// correct player index) and by Service() (to flush its sink).
struct HostState {
    Peer       *peer        = nullptr;
    NetworkSink *sink       = nullptr;
    int         remote_player = 1;
};
HostState s_host;

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

// Apply the host's level-reload event on the remote. We do NOT run
// the level's Lua here — the host runs the simulation and streams
// the resulting world state via SV_RESIZE + SV_GRID_SPRITE +
// SV_ACTOR_ADDED events. We make sure a (default-sized) world
// exists locally so subsequent code that walks it (Glasses,
// SetCurrentPlayer, etc.) doesn't dereference a null `level`. The
// real size arrives in the host's SV_RESIZE.
void apply_reload(int level_idx) {
    lev::Index *ind = lev::Index::getCurrentIndex();
    if (ind && level_idx >= 0 && level_idx < ind->size())
        ind->setCurrentPosition(level_idx);
    Resize(20, 13);
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
    s_client_out_unreliable << Uint8(CL_MOUSE_FORCE) << double(f[0]) << double(f[1]);
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

// Service the network connection once: drain inbound packets and flush
// outbound. Called from inside modal GUI loops (game menu, help) so a
// host whose menu is open keeps the ENet connection alive and forwards
// pause state. Called by both host and remote.
void netgame::Service() {
    if (!s_in_session) return;
    if (s_in_client_session) {
        if (s_client_peer && s_client_peer->is_connected()) {
            ecl::Buffer buf;
            int dummy;
            while (s_client_peer->poll_message(buf, dummy))
                dispatch_event_from_server(buf);
            flush_client_outbox();
        }
    } else {
        if (s_host.peer && s_host.peer->is_connected()) {
            ecl::Buffer buf;
            int dummy;
            while (s_host.peer->poll_message(buf, dummy))
                dispatch_input_from_client(buf, s_host.remote_player);
            if (s_host.sink)
                s_host.sink->flush();
        }
    }
}

namespace {

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
            Uint8 ip; double x, y;
            if (b >> ip >> x >> y) client::Msg_PlayerPosition(ip, ecl::V2(x, y));
            break;
        }
        case SV_SPARKLE: {
            double x, y;
            if (b >> x >> y) client::Msg_Sparkle(ecl::V2(x, y));
            break;
        }
        case SV_SOUND: {
            std::string sn; double x, y, v; Uint8 global;
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
            Uint32 id; double px, py, vx, vy;
            if (b >> id >> px >> py >> vx >> vy) {
                if (Actor *a = find_actor_by_id(id)) {
                    ActorInfo *ai = a->get_actorinfo();
                    ai->pos = ecl::V2(px, py);
                    ai->vel = ecl::V2(vx, vy);
                    a->move_screen();
                } else {
                    static int miss_count = 0;
                    if (++miss_count <= 5)
                        fprintf(stderr, "CL: SV_ACTOR_MOVED for unknown id %u\n",
                                unsigned(id));
                }
            }
            break;
        }
        case SV_ACTOR_SPRITE: {
            Uint32 id; std::string model;
            if (b >> id >> model) {
                if (Actor *a = find_actor_by_id(id))
                    a->set_model(model);
            }
            break;
        }
        case SV_ACTOR_ADDED: {
            Uint32 id; std::string kind;
            double px, py, vx, vy; Uint8 owner_byte;
            if (b >> id >> kind >> px >> py >> vx >> vy >> owner_byte) {
                int owner = int(Sint8(owner_byte));
                // Force the new Object to land on the host's id by
                // pinning next_id, creating, then restoring if the
                // remote was already past that point.
                int saved = Object::getNextIdSnapshot();
                Object::setNextId(int(id));
                Actor *a = MakeActor(kind.c_str());
                if (a) {
                    if (owner >= 0)
                        a->setAttr("owner", Value(int(owner)));
                    AddActor(px, py, a);
                    ActorInfo *ai = a->get_actorinfo();
                    ai->vel = ecl::V2(vx, vy);
                }
                if (saved > int(id) + 1)
                    Object::setNextId(saved);
            }
            break;
        }
        case SV_ACTOR_KILLED: {
            Uint32 id;
            if (b >> id) {
                if (Actor *a = find_actor_by_id(id))
                    KillActor(a);
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
        case SV_PAUSE: {
            Uint8 on;
            if (b >> on) {
                s_paused_by_host = (on != 0);
                client::Msg_ShowText(
                    s_paused_by_host ? "Paused by host..." : "Resumed.",
                    false, s_paused_by_host ? 1e9 : 1.0);
            }
            break;
        }
        case SV_RELOAD: {
            Uint32 idx;
            if (b >> idx)
                apply_reload(int(idx));
            break;
        }
        case SV_RESIZE: {
            Uint16 w, h;
            if (b >> w >> h)
                Resize(int(w), int(h));
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
// Handshake and main loops
//======================================================================

namespace {

// RAII guard: clears the session flags on scope exit so any thrown
// exception leaves the netgame state coherent.
struct SessionGuard {
    bool clear_client;
    SessionGuard(bool client) : clear_client(client) {}
    ~SessionGuard() {
        if (clear_client) {
            flush_client_outbox();
            s_in_client_session = false;
            s_client_peer = nullptr;
            s_paused_by_host = false;
        } else {
            s_host = HostState{};
        }
        s_in_session = false;
    }
};

// Pair an EventSink's lifetime with the registry. Ensures a thrown
// exception inside the loop doesn't leave a freed sink pointer in
// `event_sinks`.
struct SinkGuard {
    client::EventSink *sink;
    explicit SinkGuard(client::EventSink *s) : sink(s) {
        client::RegisterEventSink(sink);
    }
    ~SinkGuard() { client::UnregisterEventSink(sink); }
};

// Wait up to `timeout_ms` for a single packet to arrive. Uses
// enet_host_service's own timeout so we don't busy-poll. Returns
// true with the packet's bytes in `out` on success, false on
// timeout or disconnect.
bool wait_for_packet(Peer *peer, ecl::Buffer &out, int timeout_ms) {
    Uint32 start = SDL_GetTicks();
    while (peer->is_connected()) {
        int dummy;
        if (peer->poll_message(out, dummy))
            return true;
        Uint32 elapsed = SDL_GetTicks() - start;
        if (elapsed >= (Uint32)timeout_ms)
            return false;
        SDL_Delay(5);
    }
    return false;
}

// Run the host's main loop: full simulation, plus broadcast every
// EventSink hit and consume the remote's inputs each tick.
void host_main_loop(Peer *peer) {
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

        netgame::Service();

        int sleeptime = 10 - int(SDL_GetTicks() - last_tick_time);
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

        netgame::Service();

        int sleeptime = 10 - int(SDL_GetTicks() - last_tick_time);
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

//======================================================================
// Host lobby (pre-game, holds the connection while the host is in the
// lobby UI, runs the access-code handshake)
//======================================================================

namespace {

struct LobbyState {
    ENetHost   *enet_host = nullptr;
    int         port = 0;
    std::string code;          // 6 digits, generated on OpenHostLobby

    // Pending: ENet-connected but not yet authenticated. Held until
    // CL_AUTH arrives or the auth timeout expires.
    ENetPeer   *pending_peer = nullptr;
    Uint32      pending_since_ms = 0;

    // Authenticated client, parked until the host clicks Start. Wrapped
    // as a Peer so the existing game code can use it unchanged.
    Peer       *ready_peer = nullptr;
    ENetPeer   *ready_raw  = nullptr;

    int         failed_attempts = 0;
    std::string last_fail_reason;
};

LobbyState s_lobby;
std::string s_last_join_error;

std::string format_addr(const ENetAddress &a) {
    return ecl::strf("%u.%u.%u.%u:%u",
        unsigned( a.host        & 0xff),
        unsigned((a.host >> 8)  & 0xff),
        unsigned((a.host >> 16) & 0xff),
        unsigned((a.host >> 24) & 0xff),
        unsigned(a.port));
}

std::string generate_code() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> d(0, 999999);
    char buf[8];
    snprintf(buf, sizeof(buf), "%06d", d(gen));
    return std::string(buf);
}

// Send a one-packet message to an ENet peer that we are about to
// disconnect, then schedule the disconnect with a short timeout so the
// packet has a chance to reach the client.
void send_and_disconnect(ENetPeer *peer, const ecl::Buffer &buf) {
    ENetPacket *pkt = enet_packet_create(buf.data(), buf.size(),
                                         ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, CHANNEL_RELIABLE, pkt);
    enet_host_flush(s_lobby.enet_host);
    enet_peer_disconnect(peer, 0);
}

void record_failed_attempt(ENetPeer *peer, const std::string &reason) {
    s_lobby.failed_attempts++;
    s_lobby.last_fail_reason =
        ecl::strf("%s from %s",
                  reason.c_str(),
                  format_addr(peer->address).c_str());
    fprintf(stderr, "SV: rejected auth attempt (#%d): %s\n",
            s_lobby.failed_attempts,
            s_lobby.last_fail_reason.c_str());
}

void drop_pending(const std::string &reason) {
    if (!s_lobby.pending_peer) return;
    record_failed_attempt(s_lobby.pending_peer, reason);
    ecl::Buffer fail;
    fail << Uint8(SV_AUTH_FAIL) << reason;
    send_and_disconnect(s_lobby.pending_peer, fail);
    s_lobby.pending_peer = nullptr;
    s_lobby.pending_since_ms = 0;
}

}  // anonymous namespace

bool netgame::OpenHostLobby(int port) {
    if (s_lobby.enet_host != nullptr) {
        fprintf(stderr, "SV: lobby already open.\n");
        return false;
    }
    if (port <= 0) port = NET_PORT;

    ENetAddress network_address;
    network_address.host = ENET_HOST_ANY;
    network_address.port = port;

    ENetHost *h =
#ifdef ENET_VER_EQ_GT_13
        enet_host_create(&network_address, 1, 2, 0, 0);
#else
        enet_host_create(&network_address, 1, 0, 0);
#endif
    if (h == nullptr) {
        fprintf(stderr, "SV: failed to create ENet host on port %d.\n", port);
        return false;
    }
    s_lobby = LobbyState{};
    s_lobby.enet_host = h;
    s_lobby.port = port;
    s_lobby.code = generate_code();
    printf("SV: lobby open on port %d, access code %s\n",
           port, s_lobby.code.c_str());
    return true;
}

void netgame::CloseHostLobby() {
    if (s_lobby.enet_host == nullptr) return;
    if (s_lobby.pending_peer) {
        enet_peer_disconnect(s_lobby.pending_peer, 0);
        s_lobby.pending_peer = nullptr;
    }
    if (s_lobby.ready_peer) {
        s_lobby.ready_peer->disconnect();
        delete s_lobby.ready_peer;
        s_lobby.ready_peer = nullptr;
        s_lobby.ready_raw  = nullptr;
    }
    // Pump one final round so disconnect packets get out.
    ENetEvent ev;
    Uint32 deadline = SDL_GetTicks() + 200;
    while (SDL_GetTicks() < deadline &&
           enet_host_service(s_lobby.enet_host, &ev, 10) > 0) {
        if (ev.type == ENET_EVENT_TYPE_RECEIVE)
            enet_packet_destroy(ev.packet);
    }
    enet_host_destroy(s_lobby.enet_host);
    s_lobby = LobbyState{};
}

void netgame::ServiceHostLobby() {
    if (s_lobby.enet_host == nullptr) return;

    // Drop pending peers that take too long to send their CL_AUTH.
    if (s_lobby.pending_peer &&
        SDL_GetTicks() - s_lobby.pending_since_ms > 5000) {
        drop_pending("auth timeout");
    }

    ENetEvent ev;
    while (enet_host_service(s_lobby.enet_host, &ev, 0) > 0) {
        switch (ev.type) {
        case ENET_EVENT_TYPE_CONNECT:
            if (s_lobby.ready_peer || s_lobby.pending_peer) {
                // Already have someone — reject this one immediately.
                fprintf(stderr, "SV: refused extra connection from %s\n",
                        format_addr(ev.peer->address).c_str());
                ecl::Buffer fail;
                fail << Uint8(SV_AUTH_FAIL) << std::string("lobby busy");
                send_and_disconnect(ev.peer, fail);
                break;
            }
            s_lobby.pending_peer = ev.peer;
            s_lobby.pending_since_ms = SDL_GetTicks();
            printf("SV: connection from %s, awaiting code...\n",
                   format_addr(ev.peer->address).c_str());
            break;

        case ENET_EVENT_TYPE_RECEIVE: {
            ecl::Buffer b;
            b.assign(reinterpret_cast<char *>(ev.packet->data),
                     ev.packet->dataLength);
            enet_packet_destroy(ev.packet);

            if (ev.peer == s_lobby.pending_peer) {
                Uint8 tag = 0;
                Uint16 proto = 0;
                std::string supplied_code;
                b >> tag >> proto >> supplied_code;
                if (tag != CL_AUTH) {
                    drop_pending("malformed auth");
                    break;
                }
                if (proto != PROTO_VERSION) {
                    drop_pending(ecl::strf("protocol mismatch (got %d, want %d)",
                                           int(proto), int(PROTO_VERSION)));
                    break;
                }
                if (supplied_code != s_lobby.code) {
                    drop_pending("wrong code");
                    break;
                }
                // Auth OK.
                ecl::Buffer ok;
                ok << Uint8(SV_AUTH_OK);
                ENetPacket *pkt = enet_packet_create(
                    ok.data(), ok.size(), ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(s_lobby.pending_peer,
                               CHANNEL_RELIABLE, pkt);
                enet_host_flush(s_lobby.enet_host);

                s_lobby.ready_raw  = s_lobby.pending_peer;
                s_lobby.ready_peer = new Peer_Enet(s_lobby.enet_host,
                                                   s_lobby.pending_peer, 2);
                printf("SV: client authenticated from %s\n",
                       format_addr(s_lobby.pending_peer->address).c_str());
                s_lobby.pending_peer = nullptr;
                s_lobby.pending_since_ms = 0;
            }
            // Stray traffic from the ready peer before game-start is
            // ignored. They won't have a sink to receive yet.
            break;
        }

        case ENET_EVENT_TYPE_DISCONNECT:
            if (ev.peer == s_lobby.pending_peer) {
                s_lobby.pending_peer = nullptr;
                s_lobby.pending_since_ms = 0;
            } else if (ev.peer == s_lobby.ready_raw) {
                fprintf(stderr, "SV: authenticated client disconnected before game start.\n");
                delete s_lobby.ready_peer;
                s_lobby.ready_peer = nullptr;
                s_lobby.ready_raw  = nullptr;
            }
            break;

        default:
            break;
        }
    }
}

int         netgame::LobbyPort()              { return s_lobby.port; }
std::string netgame::LobbyCode()              { return s_lobby.code; }
bool        netgame::LobbyHasReadyClient()    { return s_lobby.ready_peer != nullptr; }
bool        netgame::LobbyHasPendingClient()  { return s_lobby.pending_peer != nullptr; }
int         netgame::LobbyFailedAttempts()    { return s_lobby.failed_attempts; }
std::string netgame::LobbyLastFailReason()    { return s_lobby.last_fail_reason; }

//======================================================================
// Game start (host) — promote the lobbied peer to a running session
//======================================================================

void netgame::StartHostedGame(const std::string &level_pack, int level_pos) {
    if (s_lobby.enet_host == nullptr || s_lobby.ready_peer == nullptr) {
        fprintf(stderr, "SV: StartHostedGame called without a ready client.\n");
        return;
    }

    // Transfer ownership of the ENet host and peer out of the lobby so
    // CloseHostLobby() doesn't touch them after we return.
    ENetHost *network_host = s_lobby.enet_host;
    Peer     *peer         = s_lobby.ready_peer;
    s_lobby.enet_host = nullptr;
    s_lobby.ready_peer = nullptr;
    s_lobby.ready_raw  = nullptr;
    int failed_during_lobby = s_lobby.failed_attempts;
    s_lobby = LobbyState{};

    if (!lev::Index::setCurrentIndex(level_pack)) {
        fprintf(stderr, "SV: missing level pack '%s'.\n", level_pack.c_str());
        peer->disconnect();
        delete peer;
        enet_host_destroy(network_host);
        return;
    }
    lev::Index *ind = lev::Index::getCurrentIndex();
    if (level_pos < 0 || level_pos >= ind->size()) {
        fprintf(stderr, "SV: invalid level position %d in pack '%s'.\n",
                level_pos, level_pack.c_str());
        peer->disconnect();
        delete peer;
        enet_host_destroy(network_host);
        return;
    }
    ind->setCurrentPosition(level_pos);
    lev::Proxy *proxy = ind->getProxy(level_pos);
    if (proxy == nullptr) {
        fprintf(stderr, "SV: no current level selected.\n");
        peer->disconnect();
        delete peer;
        enet_host_destroy(network_host);
        return;
    }
    printf("SV: hosting level pack '%s' level %d (%s); %d failed auth attempts during lobby\n",
           ind->getName().c_str(), level_pos + 1,
           proxy->getTitle().c_str(), failed_during_lobby);

    Uint32 seed = Uint32(SDL_GetTicks()) ^ Uint32(uintptr_t(peer));
    const int remote_player = 1;

    s_in_session = true;
    SessionGuard guard(/*client=*/false);

    NetworkSink sink(peer);
    s_host.peer = peer;
    s_host.sink = &sink;
    s_host.remote_player = remote_player;

    {
        ecl::Buffer hello;
        hello << Uint8(SV_HELLO)
              << PROTO_VERSION
              << ind->getName()
              << Uint32(level_pos)
              << seed
              << Uint8(remote_player);
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

    SinkGuard sink_guard(&sink);

    server::RandomState = Sint32(seed);
    server::Msg_LoadLevel(proxy, false);

    video_engine->HideMouse();
    ScopedInputGrab grab(not enigma::Nograb);

    host_main_loop(peer);

    video_engine->ShowMouse();

    fprintf(stderr, "SV: host loop exited (connected=%d, abort=%d)\n",
            int(peer->is_connected()), int(client::AbortGameP()));

    peer->disconnect();
    delete peer;
    enet_host_destroy(network_host);
}

//======================================================================
// Client join
//======================================================================

std::string netgame::LastJoinError() {
    return s_last_join_error;
}

namespace {

// Helper to bail out of Join with cleanup and a stored error reason.
struct JoinAbort {
    Peer     **peer;
    ENetHost  *host;
    JoinAbort(Peer **p, ENetHost *h) : peer(p), host(h) {}
    void operator()(const std::string &reason) {
        s_last_join_error = reason;
        fprintf(stderr, "CL: %s\n", reason.c_str());
        if (*peer) {
            (*peer)->disconnect();
            delete *peer;
            *peer = nullptr;
        }
        if (host) enet_host_destroy(host);
    }
};

}  // namespace

void netgame::Join(std::string hostname, int port, std::string code) {
    if (port <= 0) port = NET_PORT;
    s_last_join_error.clear();
    printf("CL: connecting to %s:%d...\n", hostname.c_str(), port);

    ENetHost *network_host = enet_host_create(nullptr, 1,
#ifdef ENET_VER_EQ_GT_13
                                              2,
#endif
                                              0, 0);
    if (network_host == nullptr) {
        s_last_join_error = "failed to create ENet client host";
        fprintf(stderr, "CL: %s\n", s_last_join_error.c_str());
        return;
    }

    ENetAddress sv_address;
    if (enet_address_set_host(&sv_address, hostname.c_str()) != 0) {
        s_last_join_error = "could not resolve host '" + hostname + "'";
        fprintf(stderr, "CL: %s\n", s_last_join_error.c_str());
        enet_host_destroy(network_host);
        return;
    }
    sv_address.port = port;

    ENetPeer *raw_peer =
#ifdef ENET_VER_EQ_GT_13
        enet_host_connect(network_host, &sv_address, 2, 0);
#else
        enet_host_connect(network_host, &sv_address, 2);
#endif
    if (raw_peer == nullptr) {
        s_last_join_error = "no available peers for connection";
        fprintf(stderr, "CL: %s\n", s_last_join_error.c_str());
        enet_host_destroy(network_host);
        return;
    }

    ENetEvent event;
    if (enet_host_service(network_host, &event, 5000) <= 0 ||
        event.type != ENET_EVENT_TYPE_CONNECT) {
        s_last_join_error = ecl::strf("could not connect to %s:%d",
                                      hostname.c_str(), port);
        fprintf(stderr, "CL: %s\n", s_last_join_error.c_str());
        enet_peer_reset(raw_peer);
        enet_host_destroy(network_host);
        return;
    }
    Peer *peer = new Peer_Enet(network_host, raw_peer, 0);
    JoinAbort abort(&peer, network_host);

    // Send our access code right away. The host validates it before
    // anything else and will reply SV_AUTH_OK or SV_AUTH_FAIL.
    {
        ecl::Buffer auth;
        auth << Uint8(CL_AUTH) << PROTO_VERSION << code;
        peer->send_reliable(auth, CHANNEL_RELIABLE);
    }

    ecl::Buffer auth_reply;
    if (!wait_for_packet(peer, auth_reply, /*timeout_ms=*/10000)) {
        abort("auth response timed out");
        return;
    }
    {
        Uint8 atag = 0;
        auth_reply >> atag;
        if (atag == SV_AUTH_FAIL) {
            std::string reason;
            auth_reply >> reason;
            abort("rejected by host: " + reason);
            return;
        }
        if (atag != SV_AUTH_OK) {
            abort(ecl::strf("unexpected auth tag 0x%x", int(atag)));
            return;
        }
    }
    printf("CL: code accepted, waiting for host to start the game...\n");

    // Now wait for SV_HELLO. This may take a while — the host is
    // sitting in their lobby until they click Start. Use a long timeout.
    ecl::Buffer hello;
    if (!wait_for_packet(peer, hello, /*timeout_ms=*/120000)) {
        abort("host did not start the game in time");
        return;
    }
    Uint8 tag = 0;
    Uint16 proto = 0;
    std::string level_pack;
    Uint32 level_idx = 0;
    Uint32 seed = 0;
    Uint8 assigned_player = 1;
    hello >> tag >> proto >> level_pack >> level_idx >> seed >> assigned_player;
    if (tag != SV_HELLO || proto != PROTO_VERSION) {
        abort(ecl::strf("bad SV_HELLO (tag=0x%x proto=%d, expected %d)",
                        tag, int(proto), int(PROTO_VERSION)));
        return;
    }
    printf("CL: host wants level pack '%s' level %d, seed=0x%x, "
           "assigned player=%d\n",
           level_pack.c_str(), int(level_idx) + 1, unsigned(seed),
           int(assigned_player));

    if (!lev::Index::setCurrentIndex(level_pack)) {
        abort("missing level pack '" + level_pack + "'");
        return;
    }
    lev::Index *ind = lev::Index::getCurrentIndex();
    if (int(level_idx) >= ind->size()) {
        abort(ecl::strf("level pack only has %d levels, host asked for %d",
                        ind->size(), int(level_idx)));
        return;
    }

    s_in_session = true;
    s_in_client_session = true;
    s_client_peer = peer;
    SessionGuard guard(/*client=*/true);

    server::RandomState = Sint32(seed);
    client::Stop();
    apply_reload(int(level_idx));
    player::SetCurrentPlayer(assigned_player);

    {
        ecl::Buffer ack;
        ack << Uint8(CL_HELLO) << PROTO_VERSION;
        peer->send_reliable(ack, CHANNEL_RELIABLE);
    }

    video_engine->HideMouse();
    ScopedInputGrab grab(not enigma::Nograb);

    remote_main_loop(peer);
    fprintf(stderr, "CL: remote loop exited (connected=%d, abort=%d)\n",
            int(peer->is_connected()), int(client::AbortGameP()));

    video_engine->ShowMouse();

    peer->disconnect();
    delete peer;
    enet_host_destroy(network_host);
}

bool netgame::IsPausedByHost() {
    return s_paused_by_host;
}
