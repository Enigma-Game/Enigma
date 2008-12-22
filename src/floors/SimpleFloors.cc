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

#include "floors/SimpleFloors.hh"

//#include "errors.hh"
//#include "main.hh"
#include "world.hh"

namespace enigma {
    
/* -------------------- Abyss -------------------- */

    Abyss::Abyss() : Floor("fl_abyss", 0.0, 0.0, flf_indestructible) {
    }
    
    std::string Abyss::getClass() const {
        return "fl_abyss";
    }
    
    bool Abyss::is_destructible() const {
        return false;
    }

    void Abyss::actor_contact (Actor* a) {
        SendMessage(a, "_fall");
    }
    
/* -------------------- Ice -------------------- */


    IceFloor::IceFloor() : Floor ("fl_ice") {
    }
    
    std::string IceFloor::getClass() const {
        return "fl_ice";
    }
    
/* -------------------- Space -------------------- */

    
    Space::Space(bool withForce) : Floor("fl_space") {
        if (withForce)
            objFlags |= OBJBIT_FORCETYP;
    }
    
    std::string Space::getClass() const {
        return "fl_space";
    }
    
    void Space::add_force (Actor *a, V2 &f) {
        if (objFlags & OBJBIT_FORCETYP)
            f[1] -= server::FlatForce;
        Floor::add_force(a, f);
    }
    
/* -------------------- Swamp -------------------- */

    Swamp::Swamp() : Floor("fl_swamp", 0.0, 0.0, flf_indestructible) {
    }
    
    std::string Swamp::getClass() const {
        return "fl_swamp";
    }
    
    bool Swamp::is_destructible() const {
        return false;
    }

    void Swamp::get_sink_speed (double &sink_speed, double &raise_speed) const { 
        sink_speed = server::SwampSinkSpeed;
        raise_speed = 6.0;
    }
    
    BOOT_REGISTER_START
        BootRegister(new Abyss(), "fl_abyss");
        BootRegister(new IceFloor(), "fl_ice");
        BootRegister(new Space(), "fl_space");
        BootRegister(new Space(true), "fl_space_force");
        BootRegister(new Swamp(), "fl_swamp");
    BOOT_REGISTER_END

} // namespace enigma
