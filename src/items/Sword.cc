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

#include "items/Sword.hh"
//#include "errors.hh"
//#include "main.hh"
#include "world.hh"

namespace enigma {

    Sword::Sword(bool isNew) : Item() {
        if (isNew) {
            objFlags |= ALL_DIRECTIONS;
        }
    }

    void Sword::on_creation(GridPos p) {
        if ((objFlags & ALL_DIRECTIONS) == ALL_DIRECTIONS) {
            // a new transformed hammer
            GameTimer.set_alarm(this, 0.2, false);
        } else {
            updateCurrentLightDirs();
            activatePhoto();
        }
        Item::on_creation(p);
    }
    
    void Sword::on_removal(GridPos p) {
        GameTimer.remove_alarm(this);
        objFlags &= ~ALL_DIRECTIONS;
        Item::on_removal(p);
    }

    
    void Sword::lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs) {
        if (added_dirs(oldDirs, newDirs) != 0) {
            sound_event ("itemtransform");
            transform("it_hammer_new");
        }
    }
    
    void Sword::alarm() {
            updateCurrentLightDirs();
            activatePhoto();        
    }
    
    DEF_ITEMTRAITS(Sword, "it_sword", it_sword);


    BOOT_REGISTER_START
        BootRegister(new Sword(false), "it_sword");
        BootRegister(new Sword(true), "it_sword_new");
    BOOT_REGISTER_END

} // namespace enigma
