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
 */
#include "items/Hammer.hh"
#include "world.hh"

namespace enigma {

    Hammer::Hammer(bool isNew) : Item() {
        if (isNew) {
            objFlags |= OBJBIT_NEW;
        }
    }
    
    void Hammer::on_creation(GridPos p) {
        if (objFlags & OBJBIT_NEW) {
            // a new transformed hammer
            GameTimer.set_alarm(this, 0.2, false);
        } else {
            objFlags &= ~OBJBIT_LIGHTNEWDIRS;
            objFlags |= (objFlags & OBJBIT_LIGHT) >> 25;
            activatePhoto();
        }
        Item::on_creation(p);
    }
    
    void Hammer::on_removal(GridPos p) {
        // remember last enlightment for moves
        objFlags &= ~OBJBIT_LIGHT;
        objFlags |= (objFlags & OBJBIT_LIGHTNEWDIRS) << 25;
        GameTimer.remove_alarm(this);
        objFlags &= ~OBJBIT_NEW;
        Item::on_removal(p);
    }
    
    void Hammer::lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs) {
        if (added_dirs(oldDirs, newDirs) != 0 && server::GameCompatibility != enigma::GAMET_PEROXYD) {
            sound_event ("itemtransform");
            transform("it_sword_new");
        }
    }
    
    void Hammer::on_pickup(Actor *a) {
        objFlags &= ~OBJBIT_LIGHT;   // forget enlighting info        
    }
    
    void Hammer::alarm() {
        objFlags &= ~OBJBIT_NEW;
        updateCurrentLightDirs();
        activatePhoto();        
    }
    
    DEF_ITEMTRAITS(Hammer, "it_hammer", it_hammer);


    BOOT_REGISTER_START
        BootRegister(new Hammer(false), "it_hammer");
        BootRegister(new Hammer(true), "it_hammer_new");
    BOOT_REGISTER_END

} // namespace enigma
