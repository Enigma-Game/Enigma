/*
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
 */
#include "others/TimerGadget.hh"
#include "world.hh"
#include "timer.hh"

namespace enigma {
    TimerGadget::TimerGadget() : Other() {
        state = ON;
    }
    
    TimerGadget::~TimerGadget() {
        GameTimer.remove_alarm(this);
    }
    
    std::string TimerGadget::getClass() const {
        return "ot_timer";
    }

    int TimerGadget::externalState() const {
        return state == OFF ? 0 : 1;
    }
    
    void TimerGadget::setState(int extState) {
        if (objFlags & OBJBIT_ADDED) {
            if (extState != externalState()) {
                state = extState;
                if (extState == 1) {
                    updateAlarm();
                } else {
                    GameTimer.remove_alarm(this);
                }
            }
        } else {
            state = extState;
        }
    }
    
    void TimerGadget::postAddition() {
        objFlags |= OBJBIT_ADDED;
        updateAlarm();
        Other::postAddition();
    }
    
    void TimerGadget::preRemoval() {
        objFlags &= ~OBJBIT_ADDED;
        Other::preRemoval();
    }
        
    void TimerGadget::alarm() {
        bool actionValue = (state == ON_TRUE);
        state ^= 1;   // toggle between ON_TRUE and ON_FALSE
        if(!getAttr("loop").to_bool()) {
            setState(OFF);
        }
        performAction(actionValue);
    }
    
    void TimerGadget::updateAlarm() {
        if (state == ON) {
            state = ON_TRUE;
            GameTimer.set_alarm(this, (double)getAttr("interval"), getAttr("loop").to_bool());
        }
    }

    BOOT_REGISTER_START
        BootRegister(new TimerGadget(), "ot_timer");
    BOOT_REGISTER_END

} // namespace enigma
