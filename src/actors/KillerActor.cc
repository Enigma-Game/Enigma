/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2008,2009 Ronald Lamprecht
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

#include "actors/KillerActor.hh"
//#include "errors.hh"
//#include "main.hh"
#include "world.hh"

namespace enigma {

/* -------------------- Killer  -------------------- */

    Killer::Killer(int color) : Actor (traits[color]) {
        setAttr("adhesion", Value(2.0));
        setAttr("color", Value(color));
        setAttr("controllers", Value(3));
    }

    std::string Killer::getClass() const {
        return "ac_killer";
    }

    bool Killer::is_dead() const {
        return false;
    }
    
    bool Killer::on_collision(Actor *a) {
        if (a->is_on_floor())
            SendMessage(a, "_shatter");
        return false;
    }
    
    int Killer::traitsIdx() const {
        return getAttr("color");
    }

    ActorTraits Killer::traits[2] = {
        {"ac_killer_white", ac_killer_white, 1<<ac_killer_white, 13.0/64, 0.7},
        {"ac_killer_black", ac_killer_black, 1<<ac_killer_black, 13.0/64, 0.7}
    };
    
    BOOT_REGISTER_START
        BootRegister(new Killer(1), "ac_killer");
        BootRegister(new Killer(1), "ac_killer_white");
        BootRegister(new Killer(0), "ac_killer_black");
    BOOT_REGISTER_END

} // namespace enigma

