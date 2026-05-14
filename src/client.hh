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

#include <string>

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
    virtual void OnPlaySound(const std::string &soundname, const ecl::V2 &pos,
                             double relative_volume) {}
    virtual void OnPlaySoundRelative(const std::string &soundname, double relative_volume) {}
    virtual void OnError(const std::string &text) {}

    virtual void OnActorMoved(int actor_id, const ecl::V2 &pos, const ecl::V2 &vel) {}
    virtual void OnActorSpriteChanged(int actor_id, const std::string &model_name) {}
    // layer is GRID_FLOOR / GRID_ITEMS / GRID_STONES.
    virtual void OnGridSpriteChanged(int layer, int x, int y,
                                     const std::string &model_name) {}
    virtual void OnGridSpriteCleared(int layer, int x, int y) {}
};

void RegisterEventSink(EventSink *sink);
void UnregisterEventSink(EventSink *sink);

// Notify hooks for engine mutation points that don't pass through
// a Msg_* function. No local side effect; purely a tap for sinks.
void NotifyActorMoved(int actor_id, const ecl::V2 &pos, const ecl::V2 &vel);
void NotifyActorSpriteChanged(int actor_id, const std::string &model_name);
void NotifyGridSpriteChanged(int layer, int x, int y, const std::string &model_name);
void NotifyGridSpriteCleared(int layer, int x, int y);

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
