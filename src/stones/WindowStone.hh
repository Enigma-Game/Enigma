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

#include "actors.hh"
#include "stones_internal.hh"

/* -------------------- Window -------------------- */

/**  st_window Breakable Stone

Hit this window heavily with your marble to blast it into smithereens.

*/

namespace enigma {
    
    class WindowStone : public Stone {
        CLONEOBJ(WindowStone);
        DECL_TRAITS;
    private:
        enum iState { IDLE, BREAK, FINALBREAK };

        enum ObjectPrivatFlagsBits {
            OBJBIT_SCRATCHDIRS  =   ALL_DIRECTIONS<<24,   ///< faces that are scratched
            OBJBIT_SECURE       =   1<<29    ///<  
        };
        
    public:
        WindowStone(std::string faces);
        
        // Object interface
        virtual void setAttr(const std::string &key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);
        
         // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);
       
        // GridObject interface
        virtual DirectionBits getFaces(bool actorInvisible = false) const;
        virtual void init_model();

        // ModelCallback interface
        virtual void animcb();

        // Stone interface
        virtual bool allowsSpreading(Direction dir, bool isFlood = false) const;
        virtual void actor_hit(const StoneContact &sc);
        virtual StoneResponse collision_response(const StoneContact &sc);
        virtual const char *collision_sound() {return "glass";}
        virtual bool is_transparent (Direction) const { return true; }
        virtual bool is_floating() const { return state != IDLE; }
        virtual bool is_sticky(const Actor *a) const;
        virtual void on_move();

    private:
        void breakFaces(DirectionBits faces);
        bool tryInnerPull(Direction dir, Actor *initiator = NULL);
        void postFaceChange();
    };

} // namespace enigma

#endif
