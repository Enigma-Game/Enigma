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
#ifndef GLASSESITEM_HH
#define GLASSESITEM_HH

#include "items.hh"

#include "actors.hh"
#include "stones.hh"

namespace enigma {
    class Glasses : public Item {
        CLONEOBJ(Glasses);
        DECL_ITEMTRAITS_ARRAY(2, traitsIdx());

    public:
        enum Spot {
            NOTHING         =  0,   // broken glasses
            DEATH           =  1,
            HOLLOW          =  2,
            ACTORIMPULSE    =  4,
            SENSOR          =  8,
            LIGHTPASSENGER  = 16,
            TRAP            = 32,
            MAX             = 63
        };
        
        static void updateGlasses();
                
        Glasses(int initState);
        
        // StateObject interface
        virtual int maxState() const;
        virtual void toggleState();
        virtual void setState(int extState);
        
        // GridObject interface
        virtual void init_model();
        
        // Items interface
        virtual void on_drop(Actor *a);
        virtual void on_pickup(Actor *a);
        virtual void on_stonehit(Stone *st);
        
    private:
        int traitsIdx() const;
    };
    
} // namespace enigma

#endif
