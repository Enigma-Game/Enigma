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

#include "stones/RotatorStone.hh"
#include "laser.hh"
#include "main.hh"
#include "player.hh"

namespace enigma {
    RotatorStone::RotatorStone(bool isMovable, bool counterclockwise) : Stone () {
        if (isMovable)
            objFlags |= OBJBIT_MOVABLE;
            
        state = counterclockwise ? CCW : CW;
    }

    RotatorStone::~RotatorStone() {
        GameTimer.remove_alarm(this);
    }
    
    std::string RotatorStone::getClass() const {
        return "st_rotator";
    }
        
    void RotatorStone::setAttr(const string& key, const Value &val) {
        if (key == "counterclock") {
            int newstate = val.to_bool() ? CCW : CW;
            if (newstate != state) {
                state = newstate;
                if (isDisplayable())
                    init_model();
            } 
        } else if (key == "movable") {
            if (val.to_bool())
                objFlags |= OBJBIT_MOVABLE;
            else
                objFlags &= ~OBJBIT_MOVABLE;
        } else
            Stone::setAttr(key, val);
    }
    
    Value RotatorStone::getAttr(const std::string &key) const {
        if (key == "counterclock") {
            return state == CCW;
        } else if (key == "movable") {
            return (bool)(objFlags & OBJBIT_MOVABLE);
        } else
            return Stone::getAttr(key);
    }
            
    void RotatorStone::setState(int extState) {
        if (isDisplayable() && state != extState) {
            state = extState;
            init_model();
        } else  // object initialisation
            state = extState;
    }

    void RotatorStone::init_model() {
        set_anim(state == CW ? "st_rotator_cw" : "st_rotator_ccw");
    }
    
    void RotatorStone::on_creation(GridPos p) {
        // just update light status, do not react for new stones
        // swapped and moved once are handled by the on_move method
        updateCurrentLightDirs();
            
        activatePhoto();

        Stone::on_creation(p);
    }
    
    void RotatorStone::on_removal(GridPos p) {
        // remember last enlightment for stone moves and swaps
        objFlags &= ~OBJBIT_LIGHT;
        objFlags |= (objFlags & OBJBIT_LIGHTNEWDIRS) << 25;
        GameTimer.remove_alarm(this);
        Stone::on_removal(p);
    }

    void RotatorStone::lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs) {
        if (added_dirs(oldDirs, newDirs)) {
            state = 1 - state;  // change rotation direction
            init_model();
         }
    }

    void RotatorStone::animcb() {
        init_model();
        send_impulses();
    }
    
    void RotatorStone::actor_hit(const StoneContact &sc) {
        if (player::WieldedItemIs (sc.actor, "it_wrench")) {
            state = 1 - state;  // change rotation direction
            init_model();
        }

        if (objFlags & OBJBIT_MOVABLE)
            maybe_push_stone(sc);
    }

    void RotatorStone::on_impulse(const Impulse& impulse) {
        if (objFlags & OBJBIT_MOVABLE) {
            move_stone(impulse.dir);
            propagateImpulse(impulse);
        }
    }
    
    bool RotatorStone::on_move(const GridPos &origin) {
        objFlags &= ~OBJBIT_LIGHTNEWDIRS;
        objFlags |= (objFlags & OBJBIT_LIGHT) >> 25;
        RecalcLight();   // necessary for rotators swapped out of light
        
        return Stone::on_move(origin);
    }
    
    FreezeStatusBits RotatorStone::get_freeze_bits() {
       return FREEZEBIT_IRREGULAR;
    }    

    void RotatorStone::alarm() {
        GridPos p = get_pos();
        
        for (int i = WEST; i <= NORTH; i++) {
            if (Value id = getAttr(ecl::strf("$neighbor%d",i))) {
                Stone *st = GetStone(move(get_pos(), (Direction)i));
                if (st != NULL  && id == st->getId()) {
                    send_impulse(move(p, (Direction)i), state == CW ? rotate_cw((Direction)i) : rotate_ccw((Direction)i));
                }
            }
        }
    }

    int RotatorStone::traitsIdx() const {
        return (objFlags & OBJBIT_MOVABLE) ? 1 : 0;
    }

    void RotatorStone::send_impulses() {
        for (int i = WEST; i <= NORTH; i++) {
            Stone *st = GetStone(move(get_pos(), (Direction)i));
            Stone::setAttr(ecl::strf("$neighbor%d",i), (st == NULL) ? Value() : Value(st->getId()));
        }
        GameTimer.set_alarm(this, 0.1);
        
    }
    
    StoneTraits RotatorStone::traits[2] = {
        {"st_rotator", st_rotator, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_rotator", st_rotator, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
    };
    
    BOOT_REGISTER_START
        BootRegister(new RotatorStone(false, false), "st_rotator");
        BootRegister(new RotatorStone(false, false), "st_rotator_cw");
        BootRegister(new RotatorStone(false, true), "st_rotator_ccw");
        BootRegister(new RotatorStone(true, false), "st_rotator_cw_movable");
        BootRegister(new RotatorStone(true, true), "st_rotator_ccw_movable");
    BOOT_REGISTER_END

} // namespace enigma
