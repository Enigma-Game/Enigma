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

#include "items/BurnableItem.hh"
//#include "errors.hh"
//#include "main.hh"
#include "world.hh"

namespace enigma {

    BurnableItem::BurnableItem(int type) {
        state = type;
    }
    
    std::string BurnableItem::getClass() const {
        return "it_burnable";
    }
    
    Value BurnableItem::message(const Message &m) {
        if (m.message == "extinguish") {   // stop / never start burning
            state = FIREPROOF;
            init_model();
            return Value();
        } else if (m.message == "_brush" && (state == ASH || state == FIREPROOF)) {
            kill();   // The brush cleans the floor
            return Value();
        } else if (Floor *fl = GetFloor(get_pos())) {
            if (m.message == "ignite" || m.message == "_explosion")
                return SendMessage(fl, "ignite");
        }
        return Item::message(m);
    }
    
    void BurnableItem::setState(int extState) {
        // ignore set state attempts
    }
    
    void BurnableItem::init_model() {
        if (state == OIL) {
            set_model(ecl::strf("it_burnable_oil%d", IntegerRand(1, 4)));  // TODO store and keep model subtyp!
        } else
            Item::init_model();
    }
    
    void BurnableItem::animcb() {
        if(Floor *fl = GetFloor(get_pos()))
            fl->on_burnable_animcb(state == IGNITE);
    }
    
    bool BurnableItem::actor_hit(Actor *a) {
        if (state == IGNITE || state == BURNING)
            SendMessage(a, "_shatter");
        return false;
    }

    int BurnableItem::traitsIdx() const {
        return state;
    }
    
    ItemTraits BurnableItem::traits[6] = {
        {"it_burnable_invisible", it_burnable_invisible, itf_static, 0.0},
        {"it_burnable_oil",       it_burnable_oil,       itf_static | itf_freezable, 0.0},
        {"it_burnable_ignited",   it_burnable_ignited,   itf_static | itf_animation | itf_fireproof, 0.0},
        {"it_burnable_burning",   it_burnable_burning,   itf_static | itf_animation | itf_fireproof, 0.0},
        {"it_burnable_ash",       it_burnable_ash,       itf_static | itf_fireproof, 0.0},
        {"it_burnable_fireproof", it_burnable_fireproof, itf_static | itf_fireproof, 0.0},
    };

    BOOT_REGISTER_START
        BootRegister(new BurnableItem(0), "it_burnable");
        BootRegister(new BurnableItem(0), "it_burnable_invisible");
        BootRegister(new BurnableItem(1), "it_burnable_oil");
        BootRegister(new BurnableItem(2), "it_burnable_ignited");
        BootRegister(new BurnableItem(3), "it_burnable_burning");
        BootRegister(new BurnableItem(4), "it_burnable_ash");
        BootRegister(new BurnableItem(5), "it_burnable_fireproof");
    BOOT_REGISTER_END

} // namespace enigma
