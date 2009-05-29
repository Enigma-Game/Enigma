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

#include "items/BrakeItem.hh"

//#include "main.hh"
#include "world.hh"

namespace enigma {
    BrakeItem::BrakeItem() {
    }
    
    void BrakeItem::on_creation(GridPos p) {
        SetStone(p, MakeStone("st_brake"));
        kill();
    }
    
    std::string BrakeItem::get_inventory_model() {
        return "st_brake";
    }
    
    ItemAction BrakeItem::activate(Actor* a, GridPos p) {
        return ITEM_DROP;
    }
    
    bool BrakeItem::can_drop_at(GridPos p) {
        return GetStone(p) == NULL;
    }
    
    void BrakeItem::drop (Actor *a, GridPos p) {
        Stone *st = MakeStone("st_brake");
        SetStone(p, st);
        transferIdentity(st);
        kill();
    }
    
    DEF_ITEMTRAITS(BrakeItem, "it_brake", it_brake);
    
    BOOT_REGISTER_START
        BootRegister(new BrakeItem(), "it_brake");
    BOOT_REGISTER_END

} // namespace enigma
