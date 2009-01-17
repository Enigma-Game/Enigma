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

#include "stones/GreenBrownStone.hh"
#include "items/GlassesItem.hh"
//#include "world.hh"
//#include "main.hh"

namespace enigma {
    GreenBrownStone::GreenBrownStone(bool hollow, bool movable, int initState) {
        state = initState;
        if (hollow)
            objFlags |= OBJBIT_HOLLOW;
        if (movable) {
            objFlags |= OBJBIT_MOVABLE;
            objFlags |= OBJBIT_CLOTH;
        }
    }
    
    std::string GreenBrownStone::getClass() const {
        return get_traits().name;
    }
    
    Value GreenBrownStone::getAttr(const std::string &key) const {
        if (key == "movable") {
            return (bool)(objFlags & OBJBIT_MOVABLE);
        } else if (key == "hollow") {
            return (bool)(objFlags & OBJBIT_HOLLOW);
        }
        return Stone::getAttr(key);
    }
    
    Value GreenBrownStone::message(const Message &m) {
        if ((objFlags & OBJBIT_HOLLOW) && m.message == "_glasses") {
            if ((to_int(m.value) & Glasses::HOLLOW) && !(objFlags & OBJBIT_GLASSES)) {  // hollow
                objFlags |= OBJBIT_GLASSES;
                if (isDisplayable())
                    init_model();
            } else if (objFlags & OBJBIT_GLASSES) {
                objFlags &= ~OBJBIT_GLASSES;
                if (isDisplayable())
                    init_model();
            }
            return Value();
        }
        return Stone::message(m);
    }
   
    void GreenBrownStone::setState(int extState) {
        // block all state writes
    }
    
    void GreenBrownStone::init_model() {
        if (objFlags & OBJBIT_GLASSES)
            set_model("invisible");
        else if (state == GROWING)
            set_anim("st-wood-growing");   // missing model
        else
            set_model(getClass());
    }
    
    void GreenBrownStone::animcb() {
        state = IDLE;
        init_model();
    }

    bool GreenBrownStone::is_floating() const {
        return objFlags & OBJBIT_HOLLOW;
    }

    void GreenBrownStone::actor_hit(const StoneContact &sc) {
        if (state == GROWING)
            SendMessage(sc.actor, "_shatter");
        else
            Stone::actor_hit(sc);
    }
    
    void GreenBrownStone::actor_inside(Actor *a) {
        if (state == GROWING)
            SendMessage(a, "_shatter");
    }
    
    void GreenBrownStone::actor_contact(Actor *a) {
        if (state == GROWING)
            SendMessage(a, "_shatter");
    }
    
    StoneResponse GreenBrownStone::collision_response(const StoneContact &sc) {
        if (objFlags & OBJBIT_HOLLOW)
            return STONE_PASS;
        return Stone::collision_response(sc);
    }

    const char *GreenBrownStone::collision_sound() {
        return (objFlags & OBJBIT_CLOTH) ? "stone" : "cloth";
    }
    
    int GreenBrownStone::traitsIdx() const {
        return (objFlags & OBJBIT_MOVABLE) ? 1 : 0;
    }
        
    StoneTraits GreenBrownStone::traits[2] = {
        {"st_greenbrown", st_greenbrown, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_greenbrown", st_greenbrown, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
    };
    
    BOOT_REGISTER_START
        BootRegister(new GreenBrownStone(false, false), "st_greenbrown");
        BootRegister(new GreenBrownStone(true, false),  "st_greenbrown_hollow");
        BootRegister(new GreenBrownStone(true, true),  "st_greenbrown_movable");
        BootRegister(new GreenBrownStone(false, false, true),  "st_greenbrown_growing");
    BOOT_REGISTER_END

} // namespace enigma
