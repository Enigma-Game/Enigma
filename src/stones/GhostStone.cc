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

#include "stones/GhostStone.hh"
#include "errors.hh"
//#include "main.hh"
#include "player.hh"

namespace enigma {
    GhostStone::GhostStone(int type) {
        state = type;
    }
    
    std::string GhostStone::getClass() const {
        return "st_ghost";
    }
    
    void GhostStone::setAttr(const string& key, const Value &val) {
        if (key == "flavor") {
            std::string fl = val.to_string();
            if (fl == "purplemarble") state = PURPLEMARBLE;
            else if (fl == "greenbrown") state = GREENBROWN;
            else if (fl == "break") state = BREAK;
            else
                ASSERT(false, XLevelRuntime, ecl::strf("Ghost stone with illegal flavor value: %s", fl.c_str()).c_str());
            if (isDisplayable())
                init_model();
            return;
        } else
            Stone::setAttr(key, val);
    }
    
    Value GhostStone::getAttr(const std::string &key) const {
        if (key == "flavor") {
            std::string kind = get_traits().name;
            return kind.substr(9);
        }
        return Stone::getAttr(key);
    }
    
    void GhostStone::setState(int extState) {
        // block all state writes
    }
    
    void GhostStone::init_model() {
        set_model("invisible");
    }
    
    bool GhostStone::is_transparent (Direction) const {
        return state != PURPLEMARBLE;
    }
    
    void GhostStone::actor_hit(const StoneContact &sc) {
        if (state == PURPLEMARBLE && player::WieldedItemIs(sc.actor, "it_brush")) {
            sound_event ("stonepaint");
            if (server::GameCompatibility == GAMET_PEROXYD)
                transform("st_fake_oxydc_open");
            else 
                transform("st_purplemarble");
        } else if (state == GREENBROWN && player::WieldedItemIs(sc.actor, "it_magicwand")) {
            sound_event ("stonepaint");
            transform("st_greenbrown");
        } else if (state == BREAK && player::WieldedItemIs(sc.actor, "it_brush")) {
            sound_event ("stonepaint");
            transform("st_break_oxydc");
        }
    }
    
    const char *GhostStone::collision_sound() {
        return state == GREENBROWN ? "cloth" : "stone";
    }
    
    int GhostStone::traitsIdx() const {
        return state;
    }
        
    StoneTraits GhostStone::traits[3] = {
        {"st_ghost_purplemarble", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_ghost_greenbrown", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_ghost_break", st_ghost_break, stf_none, material_stone, 1.0, MOVABLE_BREAKABLE},
    };
    
    BOOT_REGISTER_START
        BootRegister(new GhostStone(0), "st_ghost");
        BootRegister(new GhostStone(0), "st_ghost_purplemarble");
        BootRegister(new GhostStone(1), "st_ghost_greenbrown");
        BootRegister(new GhostStone(2), "st_ghost_break");
    BOOT_REGISTER_END

} // namespace enigma
