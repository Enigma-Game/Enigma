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

#include "stones/GlassStones.hh"
#include "items/GlassesItem.hh"
#include "world.hh"
#include "main.hh"

namespace enigma {
    GlassStone::GlassStone(int type, bool hollow, bool movable, bool cloth) {
        state = type;
        if (hollow)
            objFlags |= OBJBIT_HOLLOW;
        if (movable)
            objFlags |= OBJBIT_MOVABLE;
        if (cloth)
            objFlags |= OBJBIT_CLOTH;
    }
    
    std::string GlassStone::getClass() const {
        return get_traits().name;
    }
    
    Value GlassStone::getAttr(const std::string &key) const {
        if (key == "movable") {
            return (bool)(objFlags & OBJBIT_MOVABLE);
        } else if (key == "hollow") {
            return (bool)(objFlags & OBJBIT_HOLLOW);
        }
        return Stone::getAttr(key);
    }
    
    Value GlassStone::message(const Message &m) {
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
        } else if (m.message == "signal" && server::GameCompatibility == GAMET_OXYD1) {
            if (m.value == 1) {
                KillStone(get_pos());
                return Value();
            }
        }
        return Stone::message(m);
    }
   
    void GlassStone::setState(int extState) {
        // block all state writes
    }
    
    void GlassStone::init_model() {
        if (objFlags & OBJBIT_GLASSES)
            set_model("invisible");
        else
            set_model(getClass());
    }
    
    bool GlassStone::is_floating() const {
        return objFlags & OBJBIT_HOLLOW;
    }
    
    bool GlassStone::is_sticky(const Actor *a) const {
        return !a->is_invisible();
    }
    
    bool GlassStone::is_transparent(Direction d) const {
        return (objFlags & OBJBIT_HOLLOW) || (state != BEADS && state != DARK);
    }

    StoneResponse GlassStone::collision_response(const StoneContact &sc) {
        if ((objFlags & OBJBIT_HOLLOW) || (sc.actor->is_invisible() && 
                (state != LIGHT_MOVABLE && state != DARK_MOVABLE && state != RED)))
            return STONE_PASS;
        return Stone::collision_response(sc);
    }

    const char *GlassStone::collision_sound() {
        return (objFlags & OBJBIT_CLOTH) ? "stone" : "cloth";
    }
    
    int GlassStone::traitsIdx() const {
        return state;
    }
        
    StoneTraits GlassStone::traits[8] = {
        {"st_rawglass", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_rawglass", st_standard, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
        {"st_lightglass", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_lightglass", st_standard, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
        {"st_darkglass", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_darkglass", st_standard, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
        {"st_redfiber", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_beads", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
    };
    
    BOOT_REGISTER_START
        BootRegister(new GlassStone(0, false, false), "st_rawglass");
        BootRegister(new GlassStone(1, false, true),  "st_rawglass_movable");
        BootRegister(new GlassStone(2, false, false), "st_lightglass");
        BootRegister(new GlassStone(2, true, false),  "st_lightglass_hollow");
        BootRegister(new GlassStone(3, false, true),  "st_lightglass_movable");
        BootRegister(new GlassStone(4, false, false), "st_darkglass");
        BootRegister(new GlassStone(4, true, false),  "st_darkglass_hollow");
        BootRegister(new GlassStone(5, false, true),  "st_darkglass_movable");
        BootRegister(new GlassStone(6, false, false), "st_redfiber");
        BootRegister(new GlassStone(7, false, false), "st_beads");
    BOOT_REGISTER_END

} // namespace enigma
