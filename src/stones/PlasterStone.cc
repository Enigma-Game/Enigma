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

#include "stones/PlasterStone.hh"
#include "items/GlassesItem.hh"
#include "player.hh"
//#include "world.hh"
//#include "main.hh"

namespace enigma {
    PlasterStone::PlasterStone(bool hollow, bool movable, bool breakable) {
        if (hollow)
            objFlags |= OBJBIT_HOLLOW;
        if (movable)
            objFlags |= OBJBIT_MOVABLE;
        if (breakable)
            objFlags |= OBJBIT_BREAKABLE;
    }
    
    std::string PlasterStone::getClass() const {
        return get_traits().name;
    }
    
    Value PlasterStone::getAttr(const std::string &key) const {
        if (key == "movable") {
            return (bool)(objFlags & OBJBIT_MOVABLE);
        } else if (key == "hollow") {
            return (bool)(objFlags & OBJBIT_HOLLOW);
        } else if (key == "breakable") {
            return (bool)(objFlags & OBJBIT_BREAKABLE);
        }
        return Stone::getAttr(key);
    }
    
    Value PlasterStone::message(const Message &m) {
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
        } else if ((objFlags & OBJBIT_BREAKABLE) && 
                (m.message =="ignite" || m.message == "_explosion" || m.message == "_bombstone")) {
            doBreak();
            return Value();
        }
        return Stone::message(m);
    }
   
    void PlasterStone::setState(int extState) {
        // block all state writes
    }
    
    void PlasterStone::init_model() {
        if (objFlags & OBJBIT_GLASSES)
            set_model("invisible");
        else if (state == BREAKING)
            set_anim("st_plaster_breaking");
        else
            set_model(getClass());
    }
    
    void PlasterStone::processLight(Direction d) {
        if (objFlags & OBJBIT_BREAKABLE)
            doBreak();
        else
            Stone::processLight(d);
    }
    
    void PlasterStone::animcb() {
        KillStone(get_pos());
    }
    
    bool PlasterStone::is_floating() const {
        return objFlags & OBJBIT_HOLLOW;
    }

    StoneResponse PlasterStone::collision_response(const StoneContact &sc) {
        if (objFlags & OBJBIT_HOLLOW)
            return STONE_PASS;
        return Stone::collision_response(sc);
    }

    const char *PlasterStone::collision_sound() {
        return "stone";
    }
    
    void PlasterStone::actor_hit(const StoneContact &sc) {
        if ((objFlags & OBJBIT_BREAKABLE) && player::WieldedItemIs(sc.actor, "it_hammer"))
            doBreak();
        else
            Stone::actor_hit(sc);
    }
    
    void PlasterStone::doBreak() {
        if (state == IDLE) {
            state = BREAKING;
            sound_event("stonedestroy");
            if (isDisplayable())
                init_model();
        }
    }
        
    int PlasterStone::traitsIdx() const {
        return ((objFlags & OBJBIT_BREAKABLE) ? 1 : 0) + ((objFlags & OBJBIT_MOVABLE) ? 2 : 0);
    }
    
    StoneTraits PlasterStone::traits[4] = {
        {"st_plaster", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_plaster", st_standard, stf_none, material_stone, 1.0, MOVABLE_BREAKABLE},
        {"st_plaster", st_standard, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
        {"st_plaster", st_standard, stf_none, material_stone, 1.0, MOVABLE_IRREGULAR},
    };
    
    BOOT_REGISTER_START
        BootRegister(new PlasterStone(false, false), "st_plaster");
        BootRegister(new PlasterStone(true, false),  "st_plaster_hollow");
        BootRegister(new PlasterStone(false, true),  "st_plaster_movable");
        BootRegister(new PlasterStone(false, false, true), "st_plaster_breakable");
        BootRegister(new PlasterStone(false, true, true),  "st_plaster_movebreakable");
    BOOT_REGISTER_END

} // namespace enigma
