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

#include "items/Landmine.hh"
//#include "errors.hh"
//#include "main.hh"
//#include "player.hh"
#include "world.hh"

namespace enigma {

    Landmine::Landmine() {
    }
    
    bool Landmine::actor_hit (Actor *a) {
        const double ITEM_RADIUS = 0.3;
        if (!a->is_flying()) {
            double dist = length(a->get_pos() - get_pos().center());
            if (dist < ITEM_RADIUS)
                explode();
        }
        return false;
    }
    
    void Landmine::on_stonehit(Stone *st) { 
        explode();
    }
    
    void Landmine::explode() {
        sound_event ("landmine");
        replace("it-explosion2");
    }

    DEF_ITEMTRAITSF(Landmine, "it_landmine", it_landmine, itf_static);
        

    BOOT_REGISTER_START
        BootRegister(new Landmine(), "it_landmine");
    BOOT_REGISTER_END

} // namespace enigma
