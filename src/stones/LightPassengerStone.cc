/*
 * Copyright (C) 2006 Andreas Lochmann
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

#include "stones/LightPassengerStone.hh"
#include "errors.hh"
#include "laser.hh"
#include "main.hh"
#include "player.hh"
#include "server.hh"

#include <algorithm>

namespace enigma {
    LightPassengerStone::LightPassengerStone(bool isActive) : Stone () {
        state =  isActive ? ON : OFF;
    }

    LightPassengerStone::~LightPassengerStone() {
        GameTimer.remove_alarm (this);
    }
            
    Value LightPassengerStone::message(const Message &m) {
        if (m.message == "_trigger") {
            setState(1 - state);
            return Value();
        } else if (m.message == "_model_reanimated") {
            objFlags &= ~OBJBIT_SKATEDIR;  // NODIR
            if (state != BREAK) {     // continue break animation
                init_model();         // adjust possible changed model in other cases
            }
            return Value();            
        } else if (m.message == "_init") {
            if (objFlags & OBJBIT_LIGHTNEWDIRS)
                lightDirChanged(NODIRBIT, (DirectionBits)(objFlags & OBJBIT_LIGHTNEWDIRS));
            return Value();            
        } else if (m.message == "glasses") {
            if (to_bool(m.value) != (objFlags & OBJBIT_VISIBLE)) {
                objFlags ^= OBJBIT_VISIBLE; // toggle visibility bit
                init_model();
            }
            return Value();
        }
        return Stone::message(m);
    }
    
    int LightPassengerStone::externalState() const {
        return state == OFF ? 0 : 1;
    }
    
    void LightPassengerStone::setState(int extState) {
        if (state == OFF && extState == 1) {
            state = ON;
            if (isDisplayable()) {
                init_model();
                if (objFlags & OBJBIT_LIGHTNEWDIRS)  // currently enlighted?
                    GameTimer.set_alarm(this, calcInterval(), false);
            }
        } else if ((state == ON || state == BLINK) && extState == 0) {
            state = OFF;
            objFlags &= ~OBJBIT_SKATEDIR;  // NODIR
            if (isDisplayable())
                init_model();
        }
    }

    void LightPassengerStone::init_model() {
        switch(state) {
            case OFF:
                set_anim(objFlags & OBJBIT_VISIBLE ?
                    "st-lightpassenger_off" : "st-lightpassenger_hidden"); break;
            case ON:
                set_anim("st-lightpassenger"); break;
            case BLINK:
                set_anim("st-lightpassenger-blink"); break;
            case BREAK:
                bool NorthSouth = objFlags & (NORTHBIT | SOUTHBIT);
                bool EastWest = objFlags & (WESTBIT | EASTBIT);
                sound_event ("stonedestroy");
                if (NorthSouth && !EastWest)
                    set_anim("st-lightpassenger-break-v");
                else if (!NorthSouth && EastWest)
                    set_anim("st-lightpassenger-break-h");
                else // and this even in case the laser recently disappeared
                    set_anim("st-lightpassenger-break-hv");
                break;
        }
    }
    
    void LightPassengerStone::on_creation(GridPos p) {
        activatePhoto();
        if (updateCurrentLightDirs() != NODIRBIT)
            GameTimer.set_alarm(this, calcInterval(), false);
        Stone::on_creation(p);
    }
    
    void LightPassengerStone::on_removal(GridPos p) {
        GameTimer.remove_alarm(this);
        objFlags &= ~OBJBIT_BLOCKED;
        if (state == BLINK)
            state = ON;
        Stone::on_removal(p);
    }

    void LightPassengerStone::lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs) {
        if (state == OFF)
           return;
        
        if (newDirs == 0) {
            // all lights switched off
            GameTimer.remove_alarm(this);
            if(state == BLINK) {                
                state = ON;
                init_model();
            }
        } else if (oldDirs == 0) {
            // enlighted
            GameTimer.set_alarm(this, calcInterval(), false);
        }
    }

    void LightPassengerStone::animcb() {
        ASSERT(state == BREAK, XLevelRuntime,
            "LightPassengerStone: animcb called with inconsistent state");
        KillStone(get_pos());
    }
    
    void LightPassengerStone::actor_hit(const StoneContact &sc) {
        Actor *a = sc.actor;
        if (a != NULL && state == BLINK && player::WieldedItemIs(a, "it_hammer")) {
            state = BREAK;
            init_model();
        }
    }

    void LightPassengerStone::on_impulse(const Impulse& impulse) {
        Actor *a = dynamic_cast<Actor*>(impulse.sender);
        if (a == NULL && ((objFlags & OBJBIT_LIGHTNEWDIRS) == NODIRBIT || state == OFF 
                || server::GameCompatibility != GAMET_ENIGMA))
            move_stone(impulse.dir);
    }
    
    void LightPassengerStone::alarm() {
        Direction skateDir = (Direction)((int)((objFlags & OBJBIT_SKATEDIR) >> 24) - 1);
        if ((objFlags & OBJBIT_LIGHTNEWDIRS) && (state == ON || state == BLINK)) {
            GridPos p = get_pos();
            if (objFlags & OBJBIT_BLOCKED) {
                send_impulse(move(p, skateDir), skateDir);
                objFlags &= ~OBJBIT_BLOCKED;
            }
            int toSouth = (objFlags & NORTHBIT ? -1 : 0) + (objFlags & SOUTHBIT ? +1 : 0);
            int toWest =  (objFlags & EASTBIT ? -1 : 0) + (objFlags & WESTBIT ? +1 : 0);
            if (toSouth * toWest != 0) {
                // Light is coming from two directions. Choose the one you are
                // *not* coming from (thus changing beams), in doubt: random.
                if (skateDir == NORTH || skateDir == SOUTH)
                    toSouth = 0;
                if (skateDir == EAST || skateDir == WEST)
                    toWest = 0;
                if (skateDir == NODIR) {
                    toSouth = IntegerRand(0,1) ? 0 : toSouth;
                    toWest = toSouth ? 0 : toWest;
                }
            }
            skateDir = (toSouth == 1) ? SOUTH : (toSouth == -1) ? NORTH :
                (toWest == 1) ? WEST : (toWest == -1) ? EAST : NODIR;
            objFlags &= ~OBJBIT_SKATEDIR;
            objFlags |= ((int)skateDir + 1 << 24); // store new dir
            if (skateDir == NODIR && state != BLINK) {
                // No direction but lighted? Seems to be lasers from
                // two opposite directions. Be sure and then start blinking.
                if (objFlags & OBJBIT_LIGHTNEWDIRS) {
                    state = BLINK;
                    init_model();
                }
            } else if (skateDir != NODIR) {
                if (state == BLINK) {
                    state = ON;
                    init_model();
                }
                if (GetStone(move(p, skateDir))) {
                    // Skipping each second turn makes the passenger stone seem
                    // slower when pushing another stone. This looks more
                    // natural. That's why impulse is delayed:
                    objFlags |= OBJBIT_BLOCKED;
                }
                move_stone(skateDir);
            }
            GameTimer.set_alarm(this, calcInterval(), false);
        }
    }

    double LightPassengerStone::calcInterval() {
        /*  Interval is calculated from
                    1 + friction_stone * friction_floor
        interval = ------------------------------------ * baseinterval
                    1 + gradient_stone * gradient_floor
            and min-maxed to sensible values. "gradient" is just
            the force resulting from floor->add_force. "baseinterval"
            is 50 ms or the interval given in "interval".
        */
        double base = getAttr("interval");
        if (Floor *floor = GetFloor(get_pos())) {
            if (Value f = getAttr("friction"))
                base *= 1.0 + (double)f * floor->get_friction();
            if (Value g = getAttr("gradient")) {
                Direction skateDir = (Direction)((int)((objFlags & OBJBIT_SKATEDIR) >> 24) - 1);
                if (skateDir != NODIR) {
                    V2 vec = V2(0.0,0.0);
                    double quot = 0;
                    floor->add_force(0, vec);
                    quot = skateDir == NORTH ? -vec[1] : skateDir == SOUTH ? vec[1] :
                        skateDir == EAST ? vec[0] : skateDir == WEST ? -vec[0] : 0;
                    base /= std::max(1.0 + (double)g * quot, 0.01);                    
                }
            }
        }
        return std::max(base, 0.02);
    }

    
    DEF_TRAITSM(LightPassengerStone,"st_lightpassenger", st_lightpassenger,
                MOVABLE_IRREGULAR);
    
    BOOT_REGISTER_START
        BootRegister(new LightPassengerStone(true), "st_lightpassenger");
        BootRegister(new LightPassengerStone(true), "st_lightpassenger_on");
        BootRegister(new LightPassengerStone(false), "st_lightpassenger_off");
    BOOT_REGISTER_END

} // namespace enigma
