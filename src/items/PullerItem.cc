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

#include "items/PullerItem.hh"
#include "world.hh"

namespace enigma {

    PullerItem::PullerItem(int dir) {
        objFlags |= (dir << 24);
    }
    
    std::string PullerItem::getClass() const {
        return "it_puller";
    }
    
    void PullerItem::setAttr(const string& key, const Value &val) {
        if (key == "orientation") {
            objFlags = (objFlags & ~OBJBIT_ORIENTATION) | (((int)(val) << 24) & OBJBIT_ORIENTATION);
        }
        Item::setAttr(key, val);
    }

    Value PullerItem::getAttr(const std::string &key) const {
        if (key == "orientation") {
            return orientation();
        }
        return Item::getAttr(key);
    }
    
    void PullerItem::animcb() {
        Direction dir = (Direction)orientation();
        
        // usage within a st-window
        Stone *stone = GetStone(get_pos());
        if (stone && (stone->getClass() == "st_window") &&
                to_bool(SendMessage(stone, "inner_pull", dir))) {
        }
        
        // usage in front of a stone
        else {
            GridPos stonepos = move(get_pos(), reverse(dir));
            send_impulse(stonepos, dir);
        }
        
        sound_event ("dynamite");
        replace("it_explosion_nil");
    }
    
    bool PullerItem::isStatic() const {
        return state == ACTIVE;  // active puller is static
    }

    void PullerItem::on_drop(Actor *) {
        // the puller needs to be dropped first before we can activate it
        state = ACTIVE;
        set_anim("it_puller_active");
        sound_event("puller");
    }
    
    int PullerItem::orientation() const {
        return (objFlags & OBJBIT_ORIENTATION) >> 24;
    }

    ItemTraits PullerItem::traits[4] = {
        { "it_puller_w", it_puller_w, itf_none, 0.0 },
        { "it_puller_s", it_puller_s, itf_none, 0.0 },
        { "it_puller_e", it_puller_e, itf_none, 0.0 },
        { "it_puller_n", it_puller_n, itf_none, 0.0 },
    };
        

    BOOT_REGISTER_START
        BootRegister(new PullerItem(0), "it_puller");
        BootRegister(new PullerItem(0), "it_puller_w");
        BootRegister(new PullerItem(1), "it_puller_s");
        BootRegister(new PullerItem(2), "it_puller_e");
        BootRegister(new PullerItem(3), "it_puller_n");
    BOOT_REGISTER_END

} // namespace enigma
