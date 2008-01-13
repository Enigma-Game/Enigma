/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2008 Ronald Lamprecht
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
#ifndef BLOCKERSTONE_HH
#define BLOCKERSTONE_HH

#include "stones.hh"
#include "laser.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * A door like object that can be opened and closed by a BoulderStone. This
     * stone represents the closed state of the door. The open state is represented
     * by a BlockerItem.
     * 
     * TODO autonaming for keeping identity in groups
     * TODO performAction on opening (as notification of boulder triggering)
     * TODO animation slowed down by a major factor for PerOxyd compatibility
     */
    class BlockerStone : public Stone {
        CLONEOBJ(BlockerStone);
        DECL_TRAITS;
    private:
        enum iState {
            SOLID,       ///<  
            SHRINKING,   ///< 
            GROWING      ///< 
        };
    public:
        BlockerStone(bool solid);
        
        // Object interface
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual void toggleState();
        virtual int externalState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        
        // ModelCallback interface
        virtual void animcb();
        
        // Stone interface
        virtual StoneResponse collision_response(const StoneContact &sc);
//        virtual void actor_hit(const StoneContact &sc);
        virtual void actor_contact (Actor * a);
        virtual void actor_inside (Actor * a);
//        virtual const char *collision_sound();
        
    private:
        void setIState(iState newState);
    };

} // namespace enigma

#endif
