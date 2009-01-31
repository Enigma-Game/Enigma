/*
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
#ifndef BURNABLEITEM_HH
#define BURNABLEITEM_HH

#include "items.hh"

//#include "enigma.hh"

namespace enigma {
    /**
     * Used for animations and interfaces of fire. Study 
     * floors.hh and floors.cc for the fire implementation.
     */

    class BurnableItem : public Item {
        CLONEOBJ(BurnableItem);
        DECL_ITEMTRAITS_ARRAY(6, traitsIdx());

    private:
        enum iState {
            IDLE       =  0,       ///< 
            OIL        =  1,       ///< 
            IGNITE     =  2,       ///<
            BURNING    =  3,       ///<  
            ASH        =  4,       ///<  
            FIREPROOF  =  5,       ///<  
        };

    public:
        BurnableItem(int type);
        
        // Object interface
        virtual std::string getClass() const;
        virtual Value message (const Message &m);

        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        
        // ModelCallback interface
        virtual void animcb();
        
        // Item interface
        virtual bool actor_hit(Actor *a);
        
    private:
        void crack();
        void spread();
        int traitsIdx() const;
    };
   
} // namespace enigma

#endif
