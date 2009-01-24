/*
 * Copyright (C) 2007 Andreas Lochmann
 * Copyright (C) 2007 Raoul Bourquin
 * Copyright (C) 2009 Ronald Lamprecht
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
#ifndef THIEFFLOOR_HH
#define THIEFFLOOR_HH

#include "floors.hh"

namespace enigma {

    /** 
     * 
     */
    class ThiefFloor : public Floor {
        CLONEOBJ(ThiefFloor);
        
    private:
        enum iState {
            IDLE,         ///< 
            EMERGING,     ///< 
            RETREATING,   ///< 
            CAPTURED      ///< 
        };
        
        enum ObjectPrivatFlagsBits {
            OBJBIT_MODEL =   2<<24,   ///< 
        };

    public:
        ThiefFloor();
        ~ThiefFloor();

        // Object interface
        virtual std::string getClass() const;
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        virtual void on_creation(GridPos p);
        virtual void actor_enter(Actor *a);
                
        // ModelCallback interface  - Animation callback
        virtual void animcb();

    private:
        int victimId;
        Item  *bag;
        
        // Private methods.
        void doSteal();
    };

} // namespace enigma

#endif
