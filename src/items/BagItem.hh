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
#ifndef BAGITEM_HH
#define BAGITEM_HH

#include "items.hh"
#include "Inventory.hh"
#include <vector>

namespace enigma {
    /**
     */
    class BagItem : public Item, public ItemHolder {
        DECL_ITEMTRAITS;

    private:
       enum { BAGSIZE = 13 };
         
    public:
        BagItem();
        ~BagItem();

        // Object interface
        virtual BagItem *clone();
        virtual void dispose();
        virtual std::string getClass() const;
        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);
 
        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void setOwner(int player);
        virtual void setOwnerPos(GridPos po);

        // Item interface
        virtual bool actor_hit(Actor *a);
    
        // ItemHolder interface
        virtual bool is_full() const;
        virtual bool is_empty() const;
        virtual void add_item (Item *it);
        virtual Item *yield_first();
        virtual bool containsKind(std::string kind) const;
                
    private:
        std::vector<Item *> m_contents;
    };
   
} // namespace enigma

#endif
