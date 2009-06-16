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

#include "stones/FourSwitch.hh"
#include "main.hh"
#include "server.hh"

namespace enigma {
    FourSwitch::FourSwitch() : Stone (), m_inactive_so_far (true) {
        state = NORTH;
    }
    
    Value FourSwitch::message(const Message &m) {
        if (m.message == "signal") {
            if ((((int)(m.value)) % 2 == 1) || (server::GameCompatibility == enigma::GAMET_ENIGMA
                    && (server::EnigmaCompatibility < 1.10))) // Enigma 1.00 did turn on 0, too
                turn();
            return Value();
        } else if (m.message == "_trigger") {
            if (m.value.to_bool() == false)
                turn();
            return Value();
        }
        return Stone::message(m);
    }
    
    Value FourSwitch::invertActionValue(const Value &val) const {
        if (val.getType() == Value::DOUBLE) {
            return 3 - (int)val;     // invert direction for Enigma 1.00 like signal values
        }
        return Stone::invertActionValue(val);
    }
    
    int FourSwitch::maxState() const {
        return 3;  // dir representing state
    }
    
    void FourSwitch::toggleState() {
        turn();
    }
    
    void FourSwitch::setState(int newState) {
        // just explicit set attribute "state" will set an arbitrary new state
        turn((state - newState + 4) % 4, true);  // the count is clockwise
    }
    
    void FourSwitch::init_model() {
        set_model(std::string("st_fourswitch") + to_suffix((Direction)state));
    }
    
    void FourSwitch::actor_hit(const StoneContact &sc) {
        turn();
    }

    const char *FourSwitch::collision_sound() {
         return "metal";
    }
    
    void FourSwitch::turn(int num, bool fixedClockwise) {
        if (num == 0)   // do not perform actions if not turning at all - may occur on set state
            return;
        
        bool isGlobalTarget = getAttr("target");   // destinguish state dependent target_0,... 
        
        if (isDisplayable()) {
            if (!m_inactive_so_far && !isGlobalTarget) {
                performAction(false);  // signal off for old direction state on first turn
            } else
                m_inactive_so_far = false;
        }

        bool clockwise = !(getDefaultedAttr("counterclock", false).to_bool());
        for (int i = 0; i < num; i++)
            if (clockwise || fixedClockwise)
                state = rotate_cw((Direction)state);
            else
                state = rotate_ccw((Direction)state);
        
        if (isDisplayable()) {
            init_model();
            sound_event("fourswitch");
    
            if (isGlobalTarget) {
                // state independent callback function on old API with bool true for WEST, EAST
                // and direction on new API - the direction value suits bolder, oneway orientate 
                // messages and can be inverted to for old API like signal usage.
                performAction((server::EnigmaCompatibility < 1.10) ? (state % 2) == 0 : state);
            } else {
                performAction(true);  // signal on for new direction state
            }
        }
    }
    
    DEF_TRAITS(FourSwitch, "st_fourswitch", st_fourswitch);
    
    BOOT_REGISTER_START
        BootRegister(new FourSwitch(), "st_fourswitch");
    BOOT_REGISTER_END

} // namespace enigma
