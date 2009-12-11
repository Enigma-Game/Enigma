/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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

#ifndef ITEMHOLDER_HH_INCLUDED
#define ITEMHOLDER_HH_INCLUDED

#include "items.hh"

namespace enigma
{
    /**
     * A base class for all entities that can hold multiple items
     * during the game.  Currently this is only the inventory that is
     * displayed at the bottom of the screen (there is one inventory
     * for every player) and the it-bag item.
     */
    class ItemHolder {
    public:
        virtual ~ItemHolder() {}

        //! Return true if not further object can be picked up
        virtual bool is_full() const = 0;
        virtual bool is_empty() const = 0;

        //! Add another item
        virtual void add_item (Item *it) = 0;
        virtual Item *yield_first() = 0;
        virtual void takeItemsFrom(ItemHolder *ih) {return;}
        
        virtual bool containsKind(std::string kind) const = 0;
    };
}

#endif
