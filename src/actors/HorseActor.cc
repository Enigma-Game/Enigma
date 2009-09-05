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

#include "actors/HorseActor.hh"
//#include "errors.hh"
//#include "main.hh"
#include "world.hh"

namespace enigma {

/* -------------------- Horse  -------------------- */

    Horse::Horse() : Actor (traits), m_targetidx(-1), m_target() {
        setAttr("strength", 10.0);
    }

    std::string Horse::getClass() const {
        return "ac_horse";
    }

    bool Horse::is_dead() const {
        return false;
    }
    
    void Horse::think(double dtime)  {
        double force = getAttr("strength");
        update_target ();
        if (m_targetidx != -1) {
            add_force(normalize(m_target - get_pos()) * force);
        }
        Actor::think(dtime);
    }

    void Horse::update_target() {
        ecl::V2 newPos;
        if (m_targetidx == -1) {
            // no target defined so far
            if (getDestinationByIndex(0, newPos)) {
                m_target = newPos;
                m_targetidx = 0;
            }
        } else if (length(m_target - get_pos()) < 0.2) {
            // target reached or? try next one
            if (getDestinationByIndex(++m_targetidx, newPos)) {
                m_target = newPos;
            } else
                m_targetidx = -1;     // failed -> start anew
        } 
    }
    
    ActorTraits Horse::traits = {"ac_horse", ac_horse, 1<<ac_horse, 24.0/64, 1.2};
    
    BOOT_REGISTER_START
        BootRegister(new Horse(), "ac_horse");
    BOOT_REGISTER_END

} // namespace enigma

