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

#include "stones/MagicStone.hh"
//#include "main.hh"
#include "player.hh"

namespace enigma {
    MagicStone::MagicStone(int type) {
        state = type;
    }
    
    std::string MagicStone::getClass() const {
        return "st_magic";
    }
    
    Value MagicStone::getAttr(const std::string &key) const {
        if (key == "flavor") {
            std::string kind = get_traits().name;
            return kind.substr(9);
        }
        return Stone::getAttr(key);
    }
    
    void MagicStone::setState(int extState) {
        // block all state writes
    }
    
    void MagicStone::init_model() {
        set_model(get_traits().name);
    }
    
    void MagicStone::actor_hit(const StoneContact &sc) {
        if (state == BRICK && player::WieldedItemIs(sc.actor, "it_magicwand")) {
            sound_event ("stonepaint");
            transform("st_rawglass");
        } else if (state == OXYDA) {
            sound_event ("stonetransform");
            transform("st_lightglass_movable");
        } else if (state == OXYDC) {
            sound_event ("stonetransform");
            transform("st_rawglass_movable");
        }
    }
    
   int MagicStone::traitsIdx() const {
        return state;
    }
        
    StoneTraits MagicStone::traits[3] = {
        {"st_magic_brick", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_magic_oxyda", st_magic_oxyda, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_magic_oxydc", st_magic_oxydc, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
    };
    
    BOOT_REGISTER_START
        BootRegister(new MagicStone(0), "st_magic");
        BootRegister(new MagicStone(0), "st_magic_brick");
        BootRegister(new MagicStone(1), "st_magic_oxyda");
        BootRegister(new MagicStone(2), "st_magic_oxydc");
    BOOT_REGISTER_END

} // namespace enigma
