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

#include "items/ExtraLife.hh"
//#include "errors.hh"
//#include "main.hh"
//#include "server.hh"
#include "player.hh"
#include "world.hh"

namespace enigma {

    ExtraLife::ExtraLife(bool isNew) : Item() {
        if (isNew) {
            objFlags |= OBJBIT_NEW;
        }
    }
    
    void ExtraLife::on_creation(GridPos p) {
        if (objFlags & OBJBIT_NEW) {
            // a new transformed extralife
            GameTimer.set_alarm(this, 0.2, false);
        } else {
            objFlags &= ~OBJBIT_LIGHTNEWDIRS;
            objFlags |= (objFlags & OBJBIT_LIGHT) >> 25;
            activatePhoto();
        }
        Item::on_creation(p);
    }
    
    void ExtraLife::on_removal(GridPos p) {
        // remember last enlightment for moves
        objFlags &= ~OBJBIT_LIGHT;
        objFlags |= (objFlags & OBJBIT_LIGHTNEWDIRS) << 25;
        GameTimer.remove_alarm(this);
        objFlags &= ~OBJBIT_NEW;
       Item::on_removal(p);
    }
    
    void ExtraLife::lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs) {
        if (added_dirs(oldDirs, newDirs) != 0) {
            sound_event ("itemtransform");
            replace("it_glasses");
        }
    }
    
    void  ExtraLife::setup_successor(Item *newitem) {
        newitem->setState(server::ExtralifeGlasses);
    }
    
    void ExtraLife::on_pickup(Actor *a) {
        objFlags &= ~OBJBIT_LIGHT;   // forget enlighting info        
    }
    
    void ExtraLife::alarm() {
        objFlags &= ~OBJBIT_NEW;
        DirectionBits db = updateCurrentLightDirs();
        activatePhoto();        
    }

    DEF_ITEMTRAITS(ExtraLife, "it_extralife", it_extralife);

    BOOT_REGISTER_START
        BootRegister(new ExtraLife(false), "it_extralife");
        BootRegister(new ExtraLife(true), "it_extralife_new");
    BOOT_REGISTER_END

} // namespace enigma
