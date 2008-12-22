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

#include "stones/DeathStone.hh"

#include "items/GlassesItem.hh"
//#include "main.hh"

namespace enigma {
    DeathStone::DeathStone(bool isInvisible) : Stone () {
        if (isInvisible)
            objFlags |= OBJBIT_INVISIBLE;
            
        state = IDLE;
    }

    std::string DeathStone::getClass() const {
        return "st_death";
    }
        
    void DeathStone::setAttr(const string& key, const Value &val) {
        if (key == "invisible") {
            if (val.to_bool() != ((objFlags & OBJBIT_INVISIBLE) != 0)) {
                // change of visibility
                objFlags ^= OBJBIT_INVISIBLE;
                if (isDisplayable())
                    init_model();
            }
        } else
            Stone::setAttr(key, val);
    }
    
    Value DeathStone::getAttr(const std::string &key) const {
        if (key == "invisible") {
            return (objFlags & OBJBIT_INVISIBLE) != 0;
        } else
            return Stone::getAttr(key);
    }
    
    Value DeathStone::message(const Message &m) {
        if (m.message == "_model_reanimated") {
            state = IDLE;   // reset any running anim, be ready to bump again on new grid
            init_model();   // visibility might have changed
        } else if (m.message == "_glasses") {
            init_model();
            return Value();
        }
        return Stone::message(m);
    }
            
    void DeathStone::setState(int extState) {
        if (state == IDLE && extState == 1 && isDisplayable()) {
            state = PULSING;
            set_anim("st-death-anim");
        }
    }

    void DeathStone::init_model() {
        if (state == IDLE) {
            if ((objFlags & OBJBIT_INVISIBLE) && ((server::GlassesVisibility & Glasses::DEATH) == 0))
                set_model("invisible");
            else
                set_model("st-death");
        }
        // PULSING anim is always visible and should continue independent on init_model calls
    }
    
    void DeathStone::animcb() {
        if (state == PULSING) {
            state = IDLE;
            init_model();
        }
    }
    
    void DeathStone::actor_hit(const StoneContact &sc) {
        SendMessage(sc.actor, "_shatter");
        if (state == IDLE) {
            state = PULSING;
            set_anim("st-death-anim");
        }
    }

    void DeathStone::actor_touch(const StoneContact &sc) {
        // even a slight touch should shatter the actor: 
         actor_hit(sc);
    }

    int DeathStone::traitsIdx() const {
        return (objFlags & OBJBIT_INVISIBLE) ? 1 : 0;
    }

    
    StoneTraits DeathStone::traits[2] = {
        {"st_death", st_death, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_death_invisible", st_death_invisible, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
    };
    
    BOOT_REGISTER_START
        BootRegister(new DeathStone(false), "st_death");
        BootRegister(new DeathStone(true), "st_death_invisible");
    BOOT_REGISTER_END

} // namespace enigma
