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
#ifndef WORMHOLEITEM_HH
#define WORMHOLEITEM_HH

#include "items.hh"

#include "enigma.hh"

#include "world.hh"
#include "errors.hh"

namespace enigma {
    class WormHole : public Item, public ForceField, public TimeHandler {
    private:
        /**
         * warping as bit 2 of state
         */
        enum iState {
            OFF_IDLE,        ///< force off, ready to teleport
            ON_IDLE,         ///< force on,  ready to teleport
            OFF_ENGAGED,     ///< force off, rejecting teleport
            ON_ENGAGED       ///< force on,  rejecting teleport
        };
        
    public:
        CLONEOBJ(WormHole);
        DECL_ITEMTRAITS_ARRAY(2, state & 1);
        
        WormHole(bool isOn);
        
        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const std::string &key, const Value &val);
        virtual Value message(const Message &m);

        // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void init_model();
        
        // Item interface
        virtual bool actor_hit(Actor *a);

        // ForceField interface
        virtual void add_force(Actor *a, V2 &f);
        
        // TimeHandler interface
        virtual void alarm();
        
    private:
        bool near_center_p(Actor *a);

        double correctedStrength;     ///< 0.6 * strength
        double squareRange;           ///< range of the force squared
    };
    
} // namespace enigma

#endif
