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

#include "items/Extinguisher.hh"
//#include "errors.hh"
//#include "main.hh"
#include "player.hh"
#include "world.hh"

namespace enigma {

    Extinguisher::Extinguisher(int load) {
        state = load;
    }
    
    std::string Extinguisher::getClass() const {
        return "it_extinguisher";
    }
    
    int Extinguisher::maxState() const {
        return FULL;
    }
    
    void Extinguisher::toggleState() {
        if (state < FULL)
            setState(state + 1);
    }
    
    void Extinguisher::setState(int extState) {
        Item::setState(extState);
        if (!isDisplayable())
            player::RedrawInventory();            
    }
    
    void Extinguisher::init_model() {
        set_model(get_traits().name);
    }
    
    ItemAction Extinguisher::activate(Actor* a, GridPos p) {
        if (state >= MEDIUM) {
            extinguish(p);
            extinguish(move(p, NORTH));
            extinguish(move(p, SOUTH));
            extinguish(move(p, EAST));
            extinguish(move(p, WEST));
            if (state == FULL) {
                // full extinguisher has a larger range
                extinguish(move(p, NORTH, NORTH));
                extinguish(move(p, NORTH, EAST));
                extinguish(move(p, SOUTH, SOUTH));
                extinguish(move(p, SOUTH, WEST));
                extinguish(move(p, EAST, EAST));
                extinguish(move(p, EAST, SOUTH));
                extinguish(move(p, WEST, WEST));
                extinguish(move(p, WEST, NORTH));
            }
            --state;
    
            // Redraw the player's inventory, the visual representation of
            // the extinguisher has changed.
            player::RedrawInventory();
        } else
            return ITEM_DROP;
    }

    void Extinguisher::extinguish(GridPos p) {
        if (Item *it = GetItem(p)) {
            // try to extinguish fire - burnable items
            SendMessage (it, "extinguish");
        } else {
            // set extinguishing foam to prevent upcoming fire
            SetItem (p, MakeItem("it_burnable_fireproof"));
        }
    }
    
    int Extinguisher::traitsIdx() const {
        return state;
    }
    
    ItemTraits Extinguisher::traits[3] = {
        {"it_extinguisher_empty",  it_extinguisher_empty,  itf_none, 0.0},
        {"it_extinguisher_medium", it_extinguisher_medium, itf_fireproof, 0.0},
        {"it_extinguisher_full",   it_extinguisher,        itf_fireproof, 0.0},
    };

    BOOT_REGISTER_START
        BootRegister(new Extinguisher(2), "it_extinguisher");
        BootRegister(new Extinguisher(0), "it_extinguisher_empty");
        BootRegister(new Extinguisher(1), "it_extinguisher_medium");
        BootRegister(new Extinguisher(2), "it_extinguisher_full");
    BOOT_REGISTER_END

} // namespace enigma
