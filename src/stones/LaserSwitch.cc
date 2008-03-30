/*
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

#include "stones/LaserSwitch.hh"

#include "main.hh"
#include "util.hh"
#include "world.hh"

namespace enigma {

    LaserSwitch::LaserSwitch() : Stone("st_laserswitch") {
        state = NEW;
    }

    std::string LaserSwitch::getClass() const {
        return "st_laserswitch";
    }

    Value LaserSwitch::message(const Message &m) {
        if (m.message == "_model_reanimated") {
            // we are swapped in or out of a laser beam
            init_model();  // replace bogus model
        } else if (m.message == "_init") {
            if (updateCurrentLightDirs() != NODIRBIT) {
                state = ON;
                init_model();
            }
        }
        return Stone::message(m);
    }
    
    int LaserSwitch::externalState() const {
        return state % 2;     // report initial state as OFF
    }
    
    void LaserSwitch::setState(int extState) {
        // the state cannot be set externally - just by laserlight
    }
    
    void LaserSwitch::init_model() {
        set_model(ecl::strf("st-laserswitch%d", state == ON ? 1 : 0));
    }
    
    void LaserSwitch::on_creation(GridPos p) {
        int newState = (updateCurrentLightDirs() != NODIRBIT) ? ON : OFF;
        if (state == NEW) {
            // no actions on initial grid positioning of a laserswitch, just set state
            state = newState;
        } else if (newState != state) {
            lightDirChanged(NODIRBIT, NODIRBIT);  // perform actions and init model
        }
        activatePhoto();

        Stone::on_creation(p);
    }
    
    void LaserSwitch::lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs) {
        if (state == OFF && oldDirs == 0) {
            state = ON;
            init_model();
//            sound_event ("?");
            performAction(true);
        } else if (state == ON && newDirs == 0) {
            state = OFF;
            init_model();
//            sound_event ("?");
            performAction(false);
        }
    }

    const char *LaserSwitch::collision_sound() {
        return "metal";
    }

    BOOT_REGISTER_START
        BootRegister(new LaserSwitch(), "st_laserswitch");
    BOOT_REGISTER_END

} // namespace enigma
