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

#include "stones/OneWayStone.hh"
//#include "main.hh"
#include "player.hh"

namespace enigma {
    OneWayStone::OneWayStone(Value color, Direction dir) : Stone () {
        state = dir;
        setAttr("color", color);
    }

    std::string OneWayStone::getClass() const {
        return "st_oneway";
    }
        
    void OneWayStone::setAttr(const string& key, const Value &val) {
        if (key == "color" && isDisplayable()) {
            Stone::setAttr(key, val);
            init_model();    // need to redisplay after attribute set
            return;
        } else if (key == "orientation") {
            if (val >= minState() && val <= maxState())
                setState(val);
        }
        Stone::setAttr(key, val);
    }
    
    Value OneWayStone::getAttr(const std::string &key) const {
        if (key == "orientation") {
            return state;
        } else
            return Stone::getAttr(key);
    }
    
    Value OneWayStone::message(const Message &m) {
        if (m.message == "orientate") {
            setAttr("state", m.value);   // enforce value check
            return Value();
        } else if (m.message == "signal" || m.message == "flip") {
            setState(reverse((Direction)state));
            return Value();
        }
        return Stone::message(m);
    }
            
    int OneWayStone::maxState() const {
        return 3;  // dir representing state
    }

    void OneWayStone::init_model() {
        set_model(ecl::strf("st_oneway%s%s", colorName(), toSuffix((Direction)state).c_str()));
    }
    
    bool OneWayStone::is_floating() const {
        return true;
    }
    
    StoneResponse OneWayStone::collision_response(const StoneContact &sc) {
        Value c = getAttr("color");
        Value accolor = sc.actor->getAttr("color");
        
        if (!sc.actor->is_flying() && (!c || c == accolor)) {
            StoneResponse result = STONE_PASS;
            if ((state == WEST && (get_pos().x >= sc.actor->get_pos()[0])) ||
                    (state == SOUTH && (get_pos().y + 1 <= sc.actor->get_pos()[1])) ||
                    (state == EAST && (get_pos().x + 1 <= sc.actor->get_pos()[0])) ||
                    (state == NORTH && (get_pos().y >= sc.actor->get_pos()[1])))
                result = STONE_REBOUND;
            return result;
        }
        else
            return STONE_REBOUND;
    }
    
    void OneWayStone::actor_hit(const StoneContact &sc) {
        Value c = getAttr("color");
        
        if (!c && player::WieldedItemIs (sc.actor, "it_magicwand")) {
            setState(reverse((Direction)state));
        }
    }

    int OneWayStone::iColor() const {
        Value v = getAttr("color");
        return v ? (int)v + 1 : 0;
    }
    
    const char* OneWayStone::colorName() const {
        switch (iColor()) {
            case 1:
                return "_black";
            case 2:
                return "_white";
            default: ;
         }
        return "";
    }
    
    DEF_TRAITS(OneWayStone, "st_oneway", st_oneway);
    
    BOOT_REGISTER_START
        BootRegister(new OneWayStone(Value(), NORTH), "st_oneway");
        BootRegister(new OneWayStone(Value(), NORTH), "st_oneway_n");
        BootRegister(new OneWayStone(Value(), EAST),  "st_oneway_e");
        BootRegister(new OneWayStone(Value(), SOUTH), "st_oneway_s");
        BootRegister(new OneWayStone(Value(), WEST),  "st_oneway_w");
        BootRegister(new OneWayStone(Value(BLACK),NORTH), "st_oneway_black");
        BootRegister(new OneWayStone(Value(BLACK),NORTH), "st_oneway_black_n");
        BootRegister(new OneWayStone(Value(BLACK),EAST),  "st_oneway_black_e");
        BootRegister(new OneWayStone(Value(BLACK),SOUTH), "st_oneway_black_s");
        BootRegister(new OneWayStone(Value(BLACK),WEST),  "st_oneway_black_w");
        BootRegister(new OneWayStone(Value(WHITE),NORTH), "st_oneway_white");
        BootRegister(new OneWayStone(Value(WHITE),NORTH), "st_oneway_white_n");
        BootRegister(new OneWayStone(Value(WHITE),EAST),  "st_oneway_white_e");
        BootRegister(new OneWayStone(Value(WHITE),SOUTH), "st_oneway_white_s");
        BootRegister(new OneWayStone(Value(WHITE),WEST),  "st_oneway_white_w");
    BOOT_REGISTER_END

} // namespace enigma
