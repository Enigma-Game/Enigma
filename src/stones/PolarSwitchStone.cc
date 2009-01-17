/*
 * Copyright (C) 2006 Andreas Lochmann
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

#include "stones/PolarSwitchStone.hh"
#include "laser.hh"


namespace enigma {
    PolarSwitchStone::PolarSwitchStone() : Stone () {
    }

    void PolarSwitchStone::setState(int extState) {
        if (isDisplayable()) {
            if (extState != state) {
                state = ON - state;
                MaybeRecalcLight(get_pos());
                init_model();
                performAction(state == ON);
            }
        } else
            state = extState;
    }

    void PolarSwitchStone::init_model() {
        set_model(ecl::strf("%s", state == ON ? "st_lightglass" : "st_darkglass"));
    }

    bool PolarSwitchStone::is_transparent (Direction d) const {
        return state == ON;
    }
    
    bool PolarSwitchStone::is_sticky(const Actor *actor) const {
        return !actor->is_invisible();
    }
    
    StoneResponse PolarSwitchStone::collision_response(const StoneContact &sc) {
        if (sc.actor->is_invisible())
            return STONE_PASS;
        return Stone::collision_response(sc);
    }
    
    void PolarSwitchStone::actor_hit(const StoneContact &sc) {
        toggleState();
    }


    DEF_TRAITS(PolarSwitchStone, "st_polarswitch", st_polarswitch);

    BOOT_REGISTER_START
        BootRegister(new PolarSwitchStone(), "st_polarswitch");
    BOOT_REGISTER_END

} // namespace enigma
