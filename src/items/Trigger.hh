/*
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
#ifndef TRIGGERITEM_HH
#define TRIGGERITEM_HH

#include "items.hh"

#include "enigma.hh"

namespace enigma {
    /**
     * A switch that is triggered by actors crossing it on the floor and stones
     * being pushed onto it. The external state is read only - value 1 (ON) is
     * reported if the trigger is pressed either by an actor or a stone, a value
     * 0 (OFF) otherwise.<p>
     * Note that due to the Enigma 1.10 standards no actions will be performed
     * on initialization or setting of a trigger, even if actors or a stone are
     * positioned on the grid. But state and model are set according to the other
     * objects.<p>
     * The internal state stores the current number of actors and the presence
     * of a stone. The stone is flagged as the least significant bit, the actor
     * count is stored in the bits above, what is simply an addition of 2 times
     * the actor count to the state. This coding simplifies requests and avoids
     * usage of additional ivars for the complete logical state.
     */
    class Trigger : public Item {
        CLONEOBJ(Trigger);
        DECL_ITEMTRAITS;
    public:
        Trigger();
        
        // Object interface
        virtual Value message (const Message &m);
        
        // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void init_model();
        
        // Item interface
        virtual void actor_enter(Actor *a);
        virtual void actor_leave(Actor *a);
        virtual void stone_change(Stone *st);
    private:
        // Methods
        int countActors();
        void updateIState(int diffActors = 0, bool refuseAction = false);

    };
    
} // namespace enigma

#endif
