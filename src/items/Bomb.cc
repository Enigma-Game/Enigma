/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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
 *
 */

#include "items/Bomb.hh"
//#include "errors.hh"
//#include "main.hh"
#include "server.hh"
#include "world.hh"

namespace enigma {

    Bomb::Bomb(int color, bool burning) {
        Item::setAttr("color", color);
        if (burning)
            state = BURNING;
    }
    
    std::string Bomb::getClass() const {
        return "it_bomb";
    }
    
    void Bomb::setAttr(const string& key, const Value &val) {
        if (key == "color") {
            Item::setAttr("color", val);
            if (isDisplayable())
                init_model();
            return;
        }
        Item::setAttr(key, val);
    }
    
    Value Bomb::message(const Message &m) {
        if (m.message == "ignite"  || m.message == "_explosion") {
            if (isDisplayable())
                burn();
            return Value();
        } else if (m.message == "explode" || m.message == "_cannonball") {
            if (isDisplayable())
                explode();
            return Value();
        } else if (m.message == "heat") {  // used by fire-system
            if (isDisplayable())
                burn();
            return true;  // caught message -> no fire!
        }
        return Item::message(m);
    }
    
    void Bomb::setState(int extState) {
        if (state == IDLE && extState == 1) {
            state = BURNING;
            if (isDisplayable())
                init_model();
        }
    }
    
    void Bomb::init_model() {
        std::string color = (getAttr("color") == BLACK) ? "black" : "white";
        if (state == IDLE) {
            set_model(ecl::strf("it_bomb_%s", color.c_str()));
        } else
            set_anim(ecl::strf("it_bomb_%s_burning", color.c_str()));
    }
    
    void Bomb::processLight(Direction d) {
        explode();
    }
    
    void Bomb::animcb() {
        explode();
    }
    
    std::string Bomb::get_inventory_model() {
        std::string color = (getAttr("color") == BLACK) ? "black" : "white";
        return ecl::strf("it_bomb_%s", color.c_str());        
    }
    
    void Bomb::on_stonehit(Stone *st) {
        switch (server::GameCompatibility) {
            case GAMET_OXYD1:
            case GAMET_OXYDMAGNUM:
                if (!st->isKind("st_box_wood"))
                    // st-wood does not cause bombs to explode
                    explode();
                break;
            default :
                if (!st->isKind("st_box_hay"))
                    explode();
                break;
        }
    }
    
    void Bomb::burn() {
        if (state == IDLE) {
            state = BURNING;
            init_model();
        }
    }
    
    void Bomb::explode() {
        GridPos p = get_pos();
        Value vc = getAttr("color");
        sound_event(vc == BLACK ? "bomb_black" : "bomb_white");
        SendExplosionEffect(p, vc == BLACK ? EXPLOSION_BLACKBOMB : EXPLOSION_WHITEBOMB);  // may kill the bomb by another explosion1 set by brake
        replace("it_explosion_debris");
    }

    int Bomb::traitsIdx() const {
        return state;
    }
    
    ItemTraits Bomb::traits[2] = {
        {"it_bomb", it_bomb, itf_static | itf_indestructible | itf_fireproof | itf_portable | itf_freezable, 0.0},
        {"it_bomb_burning", it_bomb_burning, itf_static | itf_indestructible | itf_fireproof | itf_freezable, 0.0},
    };

    BOOT_REGISTER_START
        BootRegister(new Bomb(0), "it_bomb");
        BootRegister(new Bomb(0), "it_bomb_black");
        BootRegister(new Bomb(0, true), "it_bomb_black_burning");
        BootRegister(new Bomb(1), "it_bomb_white");
        BootRegister(new Bomb(1, true), "it_bomb_white_burning");
    BOOT_REGISTER_END

} // namespace enigma
