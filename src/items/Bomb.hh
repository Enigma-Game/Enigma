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
#ifndef BOMBITEM_HH
#define BOMBITEM_HH

#include "items.hh"

//#include "enigma.hh"

namespace enigma {
    /**
     * 
     */

    class Bomb : public Item {
        CLONEOBJ(Bomb);
        DECL_ITEMTRAITS_ARRAY(2, traitsIdx());

    private:
        enum iState {
            IDLE       =  0,       ///< 
            BURNING    =  1,       ///<  
        };
        
    public:
        Bomb(int color, bool burning =false);
        
        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const string& key, const Value &val);
        virtual Value message (const Message &m);

        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        virtual void processLight(Direction d);
        
        // ModelCallback interface
        virtual void animcb();
        
        // Item interface
        virtual std::string get_inventory_model();
        virtual void on_stonehit(Stone *st);
        
    private:
        void burn();
        void ignite();
        void explode();
        int traitsIdx() const;
    };
   
} // namespace enigma

#endif
