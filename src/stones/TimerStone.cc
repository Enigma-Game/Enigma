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

#include "stones/TimerStone.hh"

//#include "main.hh"
#include "util.hh"
#include "world.hh"

namespace enigma {
    
    TimerStone::TimerStone() : Stone("st_timer") {
        state = ON_TRUE;
    }
    
    std::string TimerStone::getClass() const {
        return "st_timer";
    }
    
    int TimerStone::externalState() const {
        return state == 0 ? 0 : 1;
    }
    
    void TimerStone::setState(int extState) {
        if (isDisplayable()) {
            if (extState != externalState()) {
                state = extState;
                if (extState == 1) {
                    updateAlarm();
                } else {
                    GameTimer.remove_alarm(this);
                }
            }
        } else
            state = extState;
    }
    
    void TimerStone::init_model() {
        if (getAttr("invisible").to_bool()) {
            set_model("invisible");
        } else {
            set_model(state == 0 ? "st-timeroff" : "st-timer");
        }
    }
    
    void TimerStone::on_creation (GridPos p) {
        updateAlarm();
        Stone::on_creation(p);
    }
    
    void TimerStone::on_removal(GridPos p) {
        GameTimer.remove_alarm(this);
        state = OFF;
        Stone::on_removal(p);
    }
    
    void TimerStone::alarm() {
        bool actionValue = (state == ON_TRUE);
        state = 3 - state;   // toggle between ON_TRUE and ON_FALSE
        if(!getAttr("loop").to_bool())
            setState(OFF);
            
        performAction(actionValue);
    }
    
    void TimerStone::updateAlarm() {
        if (state > 0)
            GameTimer.set_alarm(this, (double)getAttr("interval"), getAttr("loop").to_bool());
    }


    BOOT_REGISTER_START
        BootRegister(new TimerStone(), "st_timer");
    BOOT_REGISTER_END

} // namespace enigma
