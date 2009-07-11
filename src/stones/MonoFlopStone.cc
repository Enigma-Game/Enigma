/*
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

#include "stones/MonoFlopStone.hh"

#include "errors.hh"
#include "main.hh"
#include "util.hh"
#include "world.hh"

namespace enigma {
    
    MonoFlopStone::MonoFlopStone(bool lightsensitive, bool isOn) : Stone("st_monoflop") {
        if (lightsensitive)
            objFlags |= OBJBIT_LASER;
        state = isOn ? ON_PENDING : OFF_NEW;
    }
    
    MonoFlopStone::~MonoFlopStone() {
        GameTimer.remove_alarm(this);
    }
    
    std::string MonoFlopStone::getClass() const {
        return objFlags & OBJBIT_LASER ? "st_laserflop" : "st_monoflop";
    }
    
    Value MonoFlopStone::message(const Message &m) {
        if (m.message == "_model_reanimated") {
            init_model();
            return Value();
        }
        return Stone::message(m);
    }
    
    int MonoFlopStone::externalState() const {
        return (state <= OFF_NEW) ? 0 : 1;
    }
    
    void MonoFlopStone::setState(int extState) {
        if (isDisplayable()) { 
            if (extState == 1) {
                if (state == ON_TIMER) {
                    // renew timer by first removing it
                    GameTimer.remove_alarm(this);
                    GameTimer.set_alarm(this, (double)getAttr("interval"), false);
                } else if (state == OFF) {
                    state = ON_TIMER;
                    GameTimer.set_alarm(this, (double)getAttr("interval"), false);
                    init_model();
                    performAction(true);
                }
            }
            // ignore requests on switch OFF
        } else {
            state = (extState == 0) ? OFF_NEW : ON_PENDING;
        }
    }
    
    void MonoFlopStone::init_model() {
        set_model(state <= OFF_NEW ? "st_monoflop" : "st_monoflop_anim");
    }
    
    void MonoFlopStone::on_creation (GridPos p) {
        if (state == ON_PENDING) {
            GameTimer.set_alarm(this, (double)getAttr("interval"), false);
            state = ON_TIMER;
            // no action
        }
        if (objFlags & OBJBIT_LASER) {
            activatePhoto();
            if (updateCurrentLightDirs() != NODIRBIT) {
                switch (state) {
                    case OFF :
                        performAction(true);
                        break;
                    case OFF_NEW :
                        // no action
                        break;
                    case ON_TIMER :
                        GameTimer.remove_alarm(this);
                        break;
                    default :
                        ASSERT(false, XLevelRuntime, ecl::strf("Monoflop set onto grid in illegal state %d", state).c_str());
                }
                state = ON_LASER;
            }
        }
        if (state == OFF_NEW)
            state = OFF;
        Stone::on_creation(p);    // init the model
    }
    
    void MonoFlopStone::on_removal(GridPos p) {
        if (state == ON_LASER) {
            // start final timer
            state = ON_TIMER;
            GameTimer.set_alarm(this, (double)getAttr("interval"), false);
        }
        Stone::on_removal(p);
    }
    
    void MonoFlopStone::lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs) {
        if (oldDirs == 0 && state != ON_LASER) {
            // new light
            int oldState = state;
            if (state == ON_TIMER)
                 GameTimer.remove_alarm(this);
            state = ON_LASER;
            init_model();
            if (oldState == OFF)
                performAction(true);
        } else if (newDirs == 0) {
            // light off
            GameTimer.set_alarm(this, (double)getAttr("interval"), false);
            state = ON_TIMER;
        }
    }
        
    const char *MonoFlopStone::collision_sound() {
        return "metal";
    }
    
    void MonoFlopStone::actor_hit(const StoneContact &sc) {
        if (sc.actor) {
            setState(1);
        }
    }
    
    void MonoFlopStone::alarm() {
        state = OFF;
        if (isDisplayable()) { 
            init_model();
        }
        performAction(false);
    }

    BOOT_REGISTER_START
        BootRegister(new MonoFlopStone(false, false), "st_monoflop");
        BootRegister(new MonoFlopStone(true, false), "st_laserflop");
        BootRegister(new MonoFlopStone(true, true), "st_laserflop_on");
    BOOT_REGISTER_END

} // namespace enigma
