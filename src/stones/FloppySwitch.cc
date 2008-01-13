/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
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

#include "stones/FloppySwitch.hh"
#include "server.hh"
#include "Inventory.hh"
#include "player.hh"

namespace enigma {
    FloppySwitch::FloppySwitch() : Stone () {
    }

    void FloppySwitch::setState(int extState) {
        if (isDisplayable()) {
            if (extState != state) {
                sound_event ("switchon");
                state = ON - state;
                init_model();
                performAction(state == ON);
            }
        } else
            state = extState;
    }

    void FloppySwitch::init_model() {
        set_model(ecl::strf("st-floppy%s", state == ON ? "1" : "0"));
    }

    void FloppySwitch::actor_hit(const StoneContact &sc) {
        if (enigma::Inventory *inv = player::GetInventory(sc.actor)) {
            if (state == ON) {
                if (!inv->is_full()) {
                    inv->add_item (MakeItem("it-floppy"));
                    setState(OFF);
                }
            }
            else if (player::WieldedItemIs (sc.actor, "it-floppy")) {
                DisposeObject(inv->yield_first());
                setState(ON);
            }
            player::RedrawInventory (inv);
        }
    }

    const char *FloppySwitch::collision_sound() {
         return "metal";
    }

    DEF_TRAITS(FloppySwitch, "st_floppy", st_floppy);

    BOOT_REGISTER_START
        BootRegister(new FloppySwitch(), "st_floppy");
    BOOT_REGISTER_END

} // namespace enigma
