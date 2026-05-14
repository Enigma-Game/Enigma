/*
 * Copyright (C) 2003,2004,2005 Daniel Heck
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
 */
#ifndef CLIENT_HH_INCLUDED
#define CLIENT_HH_INCLUDED

#include "ecl_math.hh"
#include "lev/Index.hh"

#include <cstdint>
#include <string>
#include <vector>

namespace enigma {
namespace client {

/* -------------------- Functions -------------------- */

void ClientInit();
void ClientShutdown();

bool NetworkStart();

void Tick(double dtime);

bool AbortGameP();

void Stop();

/* -------------------- Outbound event sink --------------------

  Every cross-cutting server→client effect (Msg_* below, plus the
  Notify* hooks at engine mutation choke-points) is pushed to any
  registered EventSink in addition to taking its normal local effect.
  Used as a tap for an outbound network stream; default no-op so
  subclasses override only what they care about.
*/

class EventSink {
public:
    virtual ~EventSink() = default;

    virtual void OnCommand(const std::string &cmd) {}
    virtual void OnAdvanceLevel(lev::LevelAdvanceMode mode) {}
    virtual void OnJumpBack() {}
    virtual void OnLevelLoaded(bool isRestart) {}
    virtual void OnPlayerPosition(unsigned iplayer, const ecl::V2 &pos) {}
    virtual void OnSparkle(const ecl::V2 &pos) {}
    virtual void OnShowText(const std::string &text, bool scrolling, double duration) {}
    virtual void OnShowDocument(const std::string &text, bool scrolling, double duration) {}
    virtual void OnFinishedText() {}
    virtual void OnTeatime(bool onoff) {}
    virtual void OnError(const std::string &text) {}

    // Every sound::EmitSoundEvent on the host. global=true means the
    // sound is positionless; the client should play it without spatial
    // attenuation. Replaces the older OnPlaySound* hooks.
    virtual void OnSound(const std::string &soundname, const ecl::V2 &pos,
                         double volume, bool global) {}

    // The status bar's per-player inventory contents. Fired whenever
    // any player's inventory changes (additions, removals, reorders).
    // The host fires this for both players; each peer renders the
    // side that matches its local CurrentPlayer.
    virtual void OnInventoryChanged(int player_index,
                                    const std::vector<std::string> &model_names) {}

    // The status bar's stone-move counter (sokoban-style score).
    virtual void OnMoveCounter(int value) {}

    // Per-tick state, keyed by the actor's Object::getId(). Same id on
    // both peers because the handshake aligns Object::next_id and the
    // level load is deterministic from the shared seed.
    virtual void OnActorMoved(int object_id, const ecl::V2 &pos, const ecl::V2 &vel) {}
    virtual void OnActorSpriteChanged(int object_id, const std::string &model_name) {}

    // Actor lifecycle: fired when an actor is added to or yielded from
    // the live actor list mid-game (Drop.cc rotor swap, cannons, Lua
    // spawns, etc.). The remote mirrors the call so the actor exists
    // on its side too, with a matching Object id.
    virtual void OnActorAdded(int object_id, const std::string &kind,
                              const ecl::V2 &pos, const ecl::V2 &vel,
                              int owner_player) {}
    virtual void OnActorKilled(int object_id) {}

    // layer is GRID_FLOOR / GRID_ITEMS / GRID_STONES.
    virtual void OnGridSpriteChanged(int layer, int x, int y,
                                     const std::string &model_name) {}
    virtual void OnGridSpriteCleared(int layer, int x, int y) {}

    // Server-side pause state. The host fires this when its menu /
    // help screen opens or closes; the remote uses it to display a
    // "paused by host" overlay.
    virtual void OnPause(bool onoff) {}

    // Host has loaded a level. Used for the in-session level
    // transitions (restart, advance) — the remote uses this as the
    // signal to drop its current world state before applying the
    // grid/actor events that follow.
    virtual void OnReload(int level_idx) {}

    // Host's world was resized. Fires from World::Resize, twice per
    // load (initial 20x13 from PrepareLevel, then the actual size
    // from the level's createWorld). The remote calls Resize(w, h)
    // locally; the empty world is then populated by subsequent
    // SV_GRID_SPRITE and SV_ACTOR_ADDED events.
    virtual void OnResize(int w, int h) {}
};

void RegisterEventSink(EventSink *sink);
void UnregisterEventSink(EventSink *sink);

// Notify hooks for engine mutation points that don't pass through
// a Msg_* function. No local side effect; purely a tap for sinks.
void NotifyActorMoved(int object_id, const ecl::V2 &pos, const ecl::V2 &vel);
void NotifyActorSpriteChanged(int object_id, const std::string &model_name);
void NotifyActorAdded(int object_id, const std::string &kind,
                      const ecl::V2 &pos, const ecl::V2 &vel,
                      int owner_player);
void NotifyActorKilled(int object_id);
void NotifyGridSpriteChanged(int layer, int x, int y, const std::string &model_name);
void NotifyGridSpriteCleared(int layer, int x, int y);
void NotifySound(const std::string &soundname, const ecl::V2 &pos,
                 double volume, bool global);
void NotifyInventoryChanged(int player_index,
                            const std::vector<std::string> &model_names);
void NotifyMoveCounter(int value);
void NotifyPause(bool onoff);
void NotifyReload(int level_idx);
void NotifyResize(int w, int h);

/* -------------------- Server->Client messages -------------------- */

void Msg_Command(const std::string &cmd);

void Msg_AdvanceLevel(lev::LevelAdvanceMode mode);
void Msg_JumpBack();

void Msg_LevelLoaded(bool isRestart);

void Msg_PlayerPosition(unsigned iplayer, const ecl::V2 &pos);

void Msg_Sparkle(const ecl::V2 &pos);

void Msg_ShowText(const std::string &text, bool scrolling, double duration = -1);
void Msg_ShowDocument(const std::string &text, bool scrolling, double duration = -1);
void Msg_FinishedText();

void Msg_Teatime(bool onoff);

//! Play sound emanating from an absolute position in the world
void Msg_PlaySound(const std::string &soundname, const ecl::V2 &pos, double relative_volume);

//! Play a  sound emanating from the player's current position
void Msg_PlaySound(const std::string &soundname, double relative_volume);

//! Stop the game and display an error message instead
void Msg_Error(const std::string &text);

}  // namespace client
}  // namespace enigma

#endif
