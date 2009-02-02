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
#ifndef BOTTLEITEM_HH
#define BOTTLEITEM_HH

#include "items.hh"

namespace enigma {
    /**
     */
    class Bottle : public Item {
        CLONEOBJ(Bottle);
        DECL_ITEMTRAITS_ARRAY(2, traitsIdx());

    private:
        enum iState {
            IDLE,       ///< 
            BROKEN      ///<  
        };

    public:
        Bottle(int initState);

        // Object interface
        virtual std::string getClass() const;
        virtual Value message(const Message &m);
        
        // Item interface
        virtual void on_stonehit(Stone *st);
        virtual bool actor_hit(Actor *a);
        virtual ItemAction activate(Actor* a, GridPos p);

    private:
        int traitsIdx() const;
    };
   
} // namespace enigma

#endif
