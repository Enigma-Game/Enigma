/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007,2008,2009 Ronald Lamprecht
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

#include "items/BagItem.hh"
#include "errors.hh"
//#include "main.hh"
#include "items/GlassesItem.hh"
#include "Inventory.hh"
#include "player.hh"
#include "world.hh"

namespace enigma {

    BagItem::BagItem() {
    }
    
    BagItem::~BagItem() {
        // Bags on the grid are disposed, but bags in the inventory need to be
        // delete their contents on the destructor
        ecl::delete_sequence (m_contents.begin(), m_contents.end());
    }
    
    BagItem * BagItem::clone() {
        ASSERT(is_empty(), XLevelRuntime, "Bag:: Clone of a full bag!");
        return new BagItem(*this);
    }

    void BagItem::dispose() {
        Item * it = yield_first();
        while (it != NULL) {
            DisposeObject(it);
            it = yield_first();
        }
        delete this;
    }

    std::string BagItem::getClass() const {
        return "it_bag";
    }
    
    void BagItem::on_creation(GridPos p) {
        GridObject::on_creation(p);
        for (std::vector<Item *>::iterator itr = m_contents.begin(); itr != m_contents.end(); ++itr)
            (*itr)->setOwnerPos(p);
    }

    void BagItem::on_removal(GridPos p) {
        GridObject::on_removal(p);
        for (std::vector<Item *>::iterator itr = m_contents.begin(); itr != m_contents.end(); ++itr)
            (*itr)->setOwner(-1);            
    }
    
    void BagItem::setOwner(int player) {
        GridObject::setOwner(player);
        for (std::vector<Item *>::iterator itr = m_contents.begin(); itr != m_contents.end(); ++itr)
            (*itr)->setOwner(player);            
    }

    void BagItem::setOwnerPos(GridPos p) {
        GridObject::setOwnerPos(p);
        for (std::vector<Item *>::iterator itr = m_contents.begin(); itr != m_contents.end(); ++itr)
            (*itr)->setOwnerPos(p);
    }
        
    bool BagItem::actor_hit(Actor *a) {
        if (Item::actor_hit(a)) {
            if (Inventory *inv = player::MayPickup(a, NULL)) {
                std::vector<Item *>::size_type oldSize = m_contents.size();
                inv->takeItemsFrom(this);
                Glasses::updateGlasses();
                if (oldSize != m_contents.size() && !inv->willAddItem(this)) {
                    // some items have been picked up but the bag will not
                    // be picked up (and cause the following actions)
                    player::RedrawInventory (inv);
                    sound_event ("pickup");
                }
                return true;
            }
        }
        return false;
    }
    
    // ItemHolder interface
    bool BagItem::is_full() const {
        return m_contents.size() >= BAGSIZE;
    }
    void BagItem::add_item(Item *it) {
        // thieves may add items beyond pick up limit BAGSIZE
        m_contents.insert (m_contents.begin(), it);
        it->setOwnerPos(get_pos());  // item is at same position as bag
    }

    bool BagItem::is_empty() const {
        return m_contents.size() == 0;
    }

    Item * BagItem::yield_first() {
        if (m_contents.size() > 0) {
            Item *it = m_contents[0];
            m_contents.erase (m_contents.begin());
            it->setOwner(-1);  // no owner
            return it;
        }
        return NULL;
    }
    
    DEF_ITEMTRAITS(BagItem, "it_bag", it_bag);    

    BOOT_REGISTER_START
        BootRegister(new BagItem(), "it_bag");
    BOOT_REGISTER_END

} // namespace enigma
