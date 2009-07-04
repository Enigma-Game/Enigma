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

#include "stones/StandardStones.hh"
#include "items/GlassesItem.hh"
//#include "world.hh"
//#include "main.hh"

namespace enigma {
    StandardStone::StandardStone(int type, bool hollow, bool movable, bool cloth) {
        state = type;
        if (hollow)
            objFlags |= OBJBIT_HOLLOW;
        if (movable)
            objFlags |= OBJBIT_MOVABLE;
        if (cloth)
            objFlags |= OBJBIT_CLOTH;
    }
    
    std::string StandardStone::getClass() const {
        return get_traits().name;
    }
    
    Value StandardStone::getAttr(const std::string &key) const {
        if (key == "movable") {
            return (bool)(objFlags & OBJBIT_MOVABLE);
        } else if (key == "hollow") {
            return (bool)(objFlags & OBJBIT_HOLLOW);
        }
        return Stone::getAttr(key);
    }
    
    Value StandardStone::message(const Message &m) {
        if ((objFlags & OBJBIT_HOLLOW) && m.message == "_glasses") {
            if ((bool)((to_int(m.value) & Glasses::HOLLOW)) != (bool)(objFlags & OBJBIT_GLASSES)) {
                objFlags ^= OBJBIT_GLASSES;
                if (isDisplayable())
                    init_model();
            }
            return Value();
        }
        return Stone::message(m);
    }
   
    void StandardStone::setState(int extState) {
        // block all state writes
    }
    
    void StandardStone::init_model() {
        if (objFlags & OBJBIT_GLASSES)
            set_model("invisible");
        else
            set_model(getClass());
    }
    
    bool StandardStone::is_floating() const {
        return objFlags & OBJBIT_HOLLOW;
    }

    StoneResponse StandardStone::collision_response(const StoneContact &sc) {
        if (objFlags & OBJBIT_HOLLOW)
            return STONE_PASS;
        return Stone::collision_response(sc);
    }

    const char *StandardStone::collision_sound() {
        return (objFlags & OBJBIT_CLOTH) ? "cloth" : "stone";
    }
    
    int StandardStone::traitsIdx() const {
        return state;
    }
        
    StoneTraits StandardStone::traits[24] = {
        {"st_granite", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_granite", st_standard, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
        {"st_darkgray", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_purplemarble", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_tigris", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_purplegray", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_greengray", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_concrete", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_ancient", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_brownmarble", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_redmarble", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_bluegray", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_brownie", st_standard, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
        {"st_pinkbumps", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_camouflage", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_camouflage", st_standard, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
        {"st_redbrown", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_redbrown", st_standard, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
        {"st_metal", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_redrock", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_woven", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_yellow", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_invisible", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_invisible", st_standard, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
    };
    
    BOOT_REGISTER_START
        BootRegister(new StandardStone(0, false, false), "st_granite");
        BootRegister(new StandardStone(0, true, false),  "st_granite_hollow");
        BootRegister(new StandardStone(1, false, true),  "st_granite_movable");
        BootRegister(new StandardStone(2, false, false), "st_darkgray");
        BootRegister(new StandardStone(2, true, false),  "st_darkgray_hollow");
        BootRegister(new StandardStone(3, false, false), "st_purplemarble");
        BootRegister(new StandardStone(4, false, false), "st_tigris");
        BootRegister(new StandardStone(5, false, false), "st_purplegray");
        BootRegister(new StandardStone(6, false, false), "st_greengray");
        BootRegister(new StandardStone(7, false, false), "st_concrete");
        BootRegister(new StandardStone(8, false, false), "st_ancient");
        BootRegister(new StandardStone(9, false, false), "st_brownmarble");
        BootRegister(new StandardStone(10, false, false), "st_redmarble");
        BootRegister(new StandardStone(11, false, false), "st_bluegray");
        BootRegister(new StandardStone(11, true, false),  "st_bluegray_hollow");
        BootRegister(new StandardStone(12, false, true, true),  "st_brownie");
        BootRegister(new StandardStone(13, false, false), "st_pinkbumps");
        BootRegister(new StandardStone(14, false, false, true), "st_camouflage");
        BootRegister(new StandardStone(14, true, false),  "st_camouflage_hollow");
        BootRegister(new StandardStone(15, false, true, true),  "st_camouflage_movable");
        BootRegister(new StandardStone(16, false, false), "st_redbrown");
        BootRegister(new StandardStone(16, true, false),  "st_redbrown_hollow");
        BootRegister(new StandardStone(17, false, true),  "st_redbrown_movable");
        BootRegister(new StandardStone(18, false, false), "st_metal");
        BootRegister(new StandardStone(18, true, false),  "st_metal_hollow");
        BootRegister(new StandardStone(19, false, false), "st_redrock");
        BootRegister(new StandardStone(20, false, false), "st_woven");
        BootRegister(new StandardStone(21, false, false), "st_yellow");
        BootRegister(new StandardStone(22, false, false), "st_invisible");
        BootRegister(new StandardStone(22, true, false),  "st_invisible_hollow");
        BootRegister(new StandardStone(23, false, true),  "st_invisible_movable");
    BOOT_REGISTER_END

} // namespace enigma
