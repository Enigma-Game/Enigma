/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
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
#ifndef WINDOWSTONE_HH_INCLUDED
#define WINDOWSTONE_HH_INCLUDED

#include "stones.hh"
#include "stones/ConnectiveStone.hh"

#include "actors.hh"
#include "player.hh"
#include "stones_internal.hh"
#include "world.hh"

/* -------------------- Window -------------------- */

/** \page st-window Breakable Stone

Hit this window heavily with your marble to blast it into smithereens.

\image html st-window.png
*/

namespace enigma {
    
    class WindowStone : public ConnectiveStone {
        CLONEOBJ(WindowStone);
        DECL_TRAITS;
        const char *collision_sound() {return "glass";}

        bool is_transparent (Direction) const { return true; }
        bool is_floating() const { return state != IDLE; }
        enum State { IDLE, BREAK } state;

        void actor_hit(const StoneContact &sc);
        void animcb();

    public:
        WindowStone(int connections) : ConnectiveStone(connections),
                state(IDLE), breakingFaces(NODIRBIT) {
        }
        virtual bool is_sticky(const Actor *a) const;
        StoneResponse collision_response(const StoneContact &sc);
        virtual Value message(const Message &m);
    private:
        DirectionBits breakingFaces;
        bool tryInnerPull(Direction dir);
    };

} // namespace enigma

#endif
