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

void Start();
void Join(std::string hostname, int port);

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
