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

#include "items/GlassesItem.hh"

#include "Inventory.hh"
//#include "main.hh"
#include "player.hh"
#include "world.hh"

namespace enigma {
    void Glasses::updateGlasses() {
        Inventory *ci = player::GetInventory(player::CurrentPlayer());
        int newVisibility = 0;
        int i = -1;
        while ((i = ci->find("it_glasses", i+1)) != -1) {
            Glasses *gl = dynamic_cast<Glasses *>(ci->get_item(i));
            newVisibility |= gl->state;
        }
        if (newVisibility != server::GlassesVisibility) {
            server::GlassesVisibility = newVisibility;
            BroadcastMessage("_glasses", newVisibility, GridLayerBits(GRID_ITEMS_BIT | GRID_STONES_BIT));
        }
    }
    
    Glasses::Glasses(int initState) {
        state = initState;
    }
    
    int Glasses::maxState() const {
        return MAX;
    }
    
    void Glasses::toggleState() {
        // ignore toggle
    }
    
    void Glasses::setState(int extState) {
        state = extState;
        if (isDisplayable()) {
            init_model();
        } else {
            // maybe part of players inventory
            updateGlasses();
            player::RedrawInventory();
        }
    }
    
    void Glasses::init_model() {
        if (state > 0)
            set_model("it_glasses");
        else
            set_model("it_glasses_broken");
    }
    
    void Glasses::on_drop(Actor *a) {
        updateGlasses();
    }
    void Glasses::on_pickup(Actor *a) {
        updateGlasses();
    }
    void Glasses::on_stonehit(Stone *) {
        if (state > 0) {
            sound_event("shatter");
            setState(0);
        }
    }
        
    int Glasses::traitsIdx() const {
        return state != 0 ? 0 : 1;
    }

    ItemTraits Glasses::traits[2] = {
        {"it_glasses",  it_glasses,  itf_none},
        {"it_glasses_broken",  it_glasses_broken,  itf_none}
    };

    BOOT_REGISTER_START
        BootRegister(new Glasses(0), "it_glasses_broken");
        BootRegister(new Glasses(Glasses::DEATH + Glasses::HOLLOW + Glasses::LIGHTPASSENGER), "it_glasses");
    BOOT_REGISTER_END

} // namespace enigma
