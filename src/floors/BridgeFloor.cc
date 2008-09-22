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

#include "floors/BridgeFloor.hh"

#include "errors.hh"
#include "main.hh"
#include "stones.hh"
#include "world.hh"

namespace enigma {
    
    BridgeFloor::BridgeFloor(std::string flavor) : Floor("fl_bridge", 5, 1,
        flf_default, flft_noash, "fl-abyss")
    {
        Floor::setAttr("flavor", flavor);
        state = OPEN;
    }
    
    std::string BridgeFloor::getClass() const {
        return "fl_bridge";
    }
        
    void BridgeFloor::setAttr(const string& key, const Value &val) {
        if (key == "flavor") {
            std::string flavor = val.to_string();
            ASSERT(flavor == "gc" || flavor == "bw" || flavor == "bn", XLevelRuntime, "BridgeFloor illegal flavor value");
            Floor::setAttr("flavor", val);
            Floor::setAttr("burnable", flavor != "gc");
            if (isDisplayable() && state <= OPEN) {
                init_model();    // need to redisplay after attribute set
            }
            return;
        }
        Floor::setAttr(key, val);
    }
    
    Value BridgeFloor::message(const Message &m) {
        if (m.message == "_init") {
            // a stone may be set or changed after floor set
            Stone *st = GetStone(get_pos());
            if ((objFlags & OBJBIT_EXPLICIT) || (st != NULL && !st->is_floating())) {
                state = CLOSED;
            } else {
                state = OPEN;
            }
            init_model();
            return Value();
        } else
            return Floor::message(m);
    }

    int BridgeFloor::externalState() const {
        return (state == CLOSED) ? 0 : 1;
    }
    
    void BridgeFloor::setState(int extState) {
        if (isDisplayable()) {
            Stone *st = GetStone(get_pos());
            if (extState == 1 && (state == CLOSED || state == CLOSING) && (st == NULL || st->is_floating()))
                set_iState(OPENING);
            else if (extState == 0 && (state == OPEN || state == OPENING))
                set_iState(CLOSING);
        } else {   // object initialisation
            state = extState;
        }
        if (extState == 1)
            objFlags &= ~OBJBIT_EXPLICIT;
        else
            objFlags |= OBJBIT_EXPLICIT;
    }
    
    void BridgeFloor::toggleState() {
        Stone *st = GetStone(get_pos());
        if ((state == CLOSED || state == CLOSING) && st != NULL && !st->is_floating())
            objFlags ^= OBJBIT_EXPLICIT;   // toggle flag
        else if (state == CLOSED || state == CLOSING)
            setState(1);
        else
            setState(0);
    }

    void BridgeFloor::init_model()  {
        set_model(model_basename() + ((state==OPEN) ? "open" : "closed"));
    }
    
    void BridgeFloor::on_creation(GridPos p) {
        // we can not rely on state attribute in case a floor in yielded and reset on anther grid
        Stone *st = GetStone(get_pos());
        if ((objFlags & OBJBIT_EXPLICIT) || (st != NULL && !st->is_floating()))
            state = CLOSED;
        else
            state = OPEN;
        Floor::on_creation(p);
    }
    
    void BridgeFloor::animcb() {
        if (state == OPENING)
            state = OPEN;
        else if (state == CLOSING)
            state = CLOSED;
        init_model();
    }

    void BridgeFloor::stone_change(Stone *st) {
        if (st != NULL && !st->is_floating() && (state == OPEN || state == OPENING)) {
            set_iState(CLOSING);
        }
        else if ((st == NULL || st->is_floating()) && (state == CLOSED || state == CLOSING) 
                && !(objFlags & OBJBIT_EXPLICIT)) {
            set_iState(OPENING);
        }
    }
    
    void BridgeFloor::actor_contact(Actor *a) {
        if (state != CLOSED) SendMessage(a, "fall");
    }
    
    std::string BridgeFloor::model_basename() const {
        std::string flavor = getAttr("flavor").to_string();
        if (flavor == "gc")
            return "fl-bridgea-";
        else if (flavor == "bw")
            return "fl-bridgex-";
        else if (flavor == "bn")
            return "fl-bridgey-";
        return "";
    }
    
    void BridgeFloor::set_iState(int newState) {
        if (newState == OPENING) {
            if (state == CLOSING)
                get_model()->reverse();
            else
                set_anim(model_basename() + "opening");
        } else if (newState == CLOSING) {
            if (state == OPENING)
                get_model()->reverse();
            else
                set_anim(model_basename() + "closing");
        }
        state = newState;
    }

    bool BridgeFloor::has_firetype(FloorFireType selector) {
        if(   (selector == flft_burnable)
           && (server::GameCompatibility == GAMET_ENIGMA)) {
            return (state == CLOSED) && Floor::has_firetype(selector);
        } else
            return Floor::has_firetype(selector);
    }

    BOOT_REGISTER_START
        BootRegister(new BridgeFloor("gc"), "fl_bridge");
        BootRegister(new BridgeFloor("gc"), "fl_bridge_gc");
        BootRegister(new BridgeFloor("bn"), "fl_bridge_bn");
        BootRegister(new BridgeFloor("bw"), "fl_bridge_bw");
    BOOT_REGISTER_END

} // namespace enigma
