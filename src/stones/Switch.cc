/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
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

#include "stones/Switch.hh"
//#include "main.hh"
#include "server.hh"

namespace enigma {
    Switch::Switch(Value color, bool instant) : Stone () {
        if (color.getType() != Value::NIL)
            set_attrib("color", color);
        if (instant)
            set_attrib("instant", true);
        state = OFF;
    }

    void Switch::set_attrib(const string& key, const Value &val) {
        if (isDisplayable())
            if (key == "color" && state <= ON) {   // recoloring of a switch
                Stone::set_attrib(key, val);
                init_model();
                return;
            }
        Stone::set_attrib(key, val);
    }
    
    Value Switch::message(const Message &m) {
        if (m.message == "signal" && server::GameCompatibility == GAMET_OXYD1 
                && m.value == 1) {  // other signal messages are handled in superclasses
            if (state != ON)
                setState(1);
            else
                performAction(true);  // Oxyd special
            return Value();
        }
        return Stone::message(m);
    }
    
    int Switch::externalState() const {
        if (isInstant())
            return state == ON || state == TURNON;
        else
            return state % 2;   // 0 for OFF, TURNON, 1 for ON, TURNOFF
    }
    
    void Switch::setState(int extState) {
        if (isDisplayable()) {
            if (state <= ON && extState != state) {
                sound_event ("switchon");
                state += 2;  // OFF->TURNON, ON->TURNOFF
                set_anim(ecl::strf("st-switch%s%s", colorName(), extState ? "-turnon" : "-turnoff"));
                if (isInstant())
                    performAction(externalState() == ON);   // immediate action
            } else if (state == TURNON && extState == OFF) {
                state = TURNOFF;
                get_model()->reverse();  // reverse anim
                if (isInstant())
                    performAction(false);   // immediate action
            } else if (state == TURNOFF && extState == ON) {
                state = TURNON;
                get_model()->reverse();  // reverse anim
                if (isInstant())
                    performAction(true);   // immediate action
            }
        } else  // object initialisation
            state = extState;
    }

    void Switch::init_model() {
        set_model(ecl::strf("st-switch%s%s", colorName(), state == ON ? "-on" : "-off"));
    }
    
    void Switch::animcb() {
        state = TURNOFF - state;
        init_model();
        if (!isInstant())
            performAction(state == ON);   // action after state is reached
    }
    
    void Switch::actor_hit(const StoneContact &sc) {
        int ic = iColor();
        if (ic == 0 || (ic == 1 && get_id(sc.actor) == ac_blackball)
                 || (ic == 2 && get_id (sc.actor) == ac_whiteball))  // TODO why no small whiteballs?
            setState(ON - state);  // switch on only if OFF, switch off only if ON, otherwise ignore
    }

    const char *Switch::collision_sound() {
         return "metal";
    }
    
    int Switch::iColor() const {
        Value v = getAttr("color");
        return v ? (int)v + 1 : 0;
    }
    
    const char* Switch::colorName() const {
        switch (iColor()) {
            case 1:
                return "_black";
            case 2:
                return "_white";
            default: ;
         }
        return "";
    }
    
    bool Switch::isInstant() const {
        return getDefaultedAttr("instant", false).to_bool();
    }
    
    int Switch::traitsIdx() const {
        return iColor() + (isInstant() ? 3 : 0);
    }
    
    StoneTraits Switch::traits[6] = {
        {"st_switch", st_switch, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_switch_black", st_switch, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_switch_white", st_switch, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_switch_instant", st_switch, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_switch_instantblack", st_switch, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_switch_instantwhite", st_switch, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
    };
    
    BOOT_REGISTER_START
        BootRegister(new Switch(Value()), "st_switch");
        BootRegister(new Switch(Value(0)), "st_switch_black");
        BootRegister(new Switch(Value(1)), "st_switch_white");
        BootRegister(new Switch(Value(), true), "st_switch_instant");
        BootRegister(new Switch(Value(0), true), "st_switch_black_instant");
        BootRegister(new Switch(Value(1), true), "st_switch_white_instant");
    BOOT_REGISTER_END

} // namespace enigma
