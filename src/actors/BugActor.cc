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

#include "actors/BugActor.hh"
//#include "errors.hh"
//#include "main.hh"
#include "world.hh"

namespace enigma {

/* -------------------- Bug  -------------------- */

    Bug::Bug() : Actor (traits) {
    }

    std::string Bug::getClass() const {
        return "ac_bug";
    }

    bool Bug::is_dead() const {
        return false;
    }
        
    ActorTraits Bug::traits = {"ac_bug", ac_bug, 1<<ac_bug, 12.0/64, 0.7};
    
    BOOT_REGISTER_START
        BootRegister(new Bug(), "ac_bug");
    BOOT_REGISTER_END

} // namespace enigma

