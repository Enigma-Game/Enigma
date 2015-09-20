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
 */
#ifndef BLOCKERITEM_HH
#define BLOCKERITEM_HH

#include "items.hh"
#include "timer.hh"

namespace enigma {
    /**
     * A door like object that can be opened and closed by a BoulderStone. This item
     * represents the open state of the door. The closed state is represented by a
     * BlockerStone.<p>
     * If a BoulderStone moves over a BlockerItem the BlockerItem starts growing and
     * replaces itself by a BlockerStone. But other Stones can be pushed over a
     * BlockerItem without causing its transformation. <p>
     * It fully supports the door messages "open", "close", "toggle", "signal" and
     * attribute "state". If the item is blocked by a stone the messages will not
     * cause an instant growing. But the message will be remembered in the internal
     * state and the item acts as soon as the stone moves away.<p>
     * An initial BlockerItem with a BoulderStone on top will grow as soon as it moves
     * away. An initial it_blocker_new with a BloulderStone on top will not grow when
     * the stone moves away.<p>
     * Note that this is the only door object that allows a stone to be pushed through.
     * A BlockerItem can be killed by a BrakeItem being dropped.
     */
    class BlockerItem : public Item, public TimeHandler {
        DECL_ITEMTRAITS;
    private:
        enum iState {
            IDLE,       ///< neutral state
            NEW,        ///< a new BlockerItem that replaced a recently shrinked BlockerStone -
                        ///< a BoulderStone is awaited, but it has to arrive in time 
            LOCKED,     ///< a stone covers the BlockerItem and locked it, so it will not grow
                        ///< when the stone moves away. This is the state that a BoulderStone causes
                        ///< on its first visit immediatly after shrinking the BlockerStone
            UNLOCKED    ///< a stone covers the BlockerItem and unlocked it, so it will grow
                        ///< when the stone moves away. This is the state that a BoulderStone causes
                        ///< on its second visit when moving onto an idle BlockerItem. The blocker
                        ///< will grow when the stones moves away
        };

    public:
        BlockerItem(bool shrinked_recently);
        ~BlockerItem();

        // Object interface
        virtual BlockerItem *clone();
        virtual void dispose();
        virtual Value message(const Message &m);

        // StateObject interface
        virtual void toggleState();
        virtual int externalState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void init_model();
        virtual void actor_leave(Actor *a);

        // Item interface
        virtual void stone_change(Stone *st);

        // TimeHandler interface
        virtual void alarm();
    
    private:
        void setIState(iState newState);
        void grow();
    };
    
} // namespace enigma

#endif
