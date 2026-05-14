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
 */

#ifndef NETGAME_HH_INCLUDED
#define NETGAME_HH_INCLUDED

#include "ecl_math.hh"

#include <string>

namespace netgame {

// Open a host listener on the given port. Generates a fresh 6-digit
// access code, returned via LobbyCode(). Returns false if the listener
// could not be created (e.g. port already in use).
bool OpenHostLobby(int port);

// Shut down the host listener. Disconnects any pending or authenticated
// peer that wasn't promoted to a game via StartHostedGame().
void CloseHostLobby();

// Drive the host's listener once: accept new connections, validate
// authentication, count failed attempts. Call from the lobby menu tick.
void ServiceHostLobby();

// Lobby state observers.
int          LobbyPort();
std::string  LobbyCode();
bool         LobbyHasReadyClient();      // an authenticated client is waiting
bool         LobbyHasPendingClient();    // someone connected but hasn't authed yet
int          LobbyFailedAttempts();
std::string  LobbyLastFailReason();

// Take the lobbied peer and run the actual game with the chosen level.
// CloseHostLobby is implicit on return.
void StartHostedGame(const std::string &level_pack, int level_pos);

// Connect to a host as a remote client. The code is sent as part of the
// authentication step; an empty code is rejected by any host that has a
// code set. On auth failure Join() returns immediately; on success it
// blocks for the duration of the game.
void Join(std::string hostname, int port, std::string code);

// On client side: error reason from the most recent Join() that failed
// before the game started. Empty if the last Join succeeded (or if no
// Join has been attempted).
std::string LastJoinError();

// True while inside a Join() session (we are a remote client of some
// host). False during Start() and during ordinary single-player play.
bool IsClient();

// True while either Start() or Join() is in flight. Used by UI code
// that wants to suppress single-player-only behaviour (mouse grab,
// auto-pause on focus loss, hide cursor) during LAN play.
bool IsActive();

// True on the remote when the host has opened its in-game menu / help
// (i.e. the simulation is paused on the host's side).
bool IsPausedByHost();

// Drain the inbound packet queue and flush the outbound batches once.
// Safe to call when no session is active (no-op). The two main loops
// already call this each tick; modal GUI loops (game menu, help) also
// call it periodically so a long-open menu doesn't time out the ENet
// connection and so SV_PAUSE state reaches the remote promptly.
void Service();

// Send-input helpers used by the local UI layer to forward the player's
// inputs to the host. No-ops outside a client session.
void SendInputMouseForce(const ecl::V2 &f);
void SendInputActivateItem();
void SendInputRotateInventory(int dir);
void SendInputCommand(const std::string &cmd);
void SendInputInhibitPickup(bool onoff);

}  // namespace netgame

#endif
