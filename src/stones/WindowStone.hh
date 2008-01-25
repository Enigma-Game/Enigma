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

/** \page st_window Breakable Stone

Hit this window heavily with your marble to blast it into smithereens.

\image html st_window.png
*/

namespace enigma {
    
    class WindowStone : public Stone {
        CLONEOBJ(WindowStone);
        DECL_TRAITS;
    private:
        enum iState { IDLE, BREAK, FINALBREAK };
        
    public:
        WindowStone(std::string faces);
        
        // Object interface
        virtual Value message(const Message &m);

        // ModelCallback interface
        void animcb();

        // Stone interface
        void actor_hit(const StoneContact &sc);
        StoneResponse collision_response(const StoneContact &sc);
        const char *collision_sound() {return "glass";}
        bool is_transparent (Direction) const { return true; }
        bool is_floating() const { return state != IDLE; }
        virtual bool is_sticky(const Actor *a) const;


    private:
        DirectionBits breakingFaces;
        bool tryInnerPull(Direction dir);
    };

} // namespace enigma

#endif
