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

#include "stones/BreakStone.hh"
#include "player.hh"
#include "world.hh"
//#include "main.hh"

namespace enigma {
    BreakStone::BreakStone(int type) {
        objFlags |= type << 24;
    }
    
    std::string BreakStone::getClass() const {
        return "st_break";
    }
    
    Value BreakStone::getAttr(const std::string &key) const {
        if (key == "flavor") {
            std::string kind = get_traits().name;
            return kind.substr(8);
        }
        return Stone::getAttr(key);
    }
    
    Value BreakStone::message(const Message &m) {
        if (m.message =="ignite" || m.message == "_explosion" || m.message == "_bombstone") {
            if (typ() != BUG)
                doBreak();
            return Value();
        } else if (m.message == "_trigger") {
            if (typ() == BOULDER)
                doBreak();
            return Value();
        }
        return Stone::message(m);
    }
   
    void BreakStone::setState(int extState) {
        // block all state writes
    }
    
    void BreakStone::init_model() {
        if (state == BREAKING)
            set_anim(std::string(get_traits().name) + "-anim");
        else
            set_model(get_traits().name);
    }
    
    void BreakStone::processLight(Direction d) {
        if (typ() != BUG)
            doBreak();
    }
    
    void BreakStone::animcb() {
        KillStone(get_pos());
    }

    void BreakStone::actor_hit(const StoneContact &sc) {
        int t = typ();
        Value color = sc.actor->getAttr("color");
        if ((player::WieldedItemIs(sc.actor, "it_hammer") &&  (t == PLAIN || t == OXYDC || t == BOULDER
                || (color && ((color == BLACK && t == ACBLACK)||(color == WHITE && t == ACWHITE)))))
                || (t == BUG && get_id(sc.actor) == ac_bug))
            doBreak();
        else 
            Stone::actor_hit(sc);
    }

    const char *BreakStone::collision_sound() {
        return "stone";
    }
    
    int BreakStone::typ() const {
        return (objFlags & OBJBIT_SUBTYP) >> 24;
    }
    
    void BreakStone::doBreak() {
        if (state == IDLE) {
            state = BREAKING;
            sound_event("stonedestroy");
            init_model();
        }
    }
    
    int BreakStone::traitsIdx() const {
        return typ();
    }
        
    StoneTraits BreakStone::traits[7] = {
        {"st_break_plain", st_break_plain, stf_none, material_stone, 1.0, MOVABLE_BREAKABLE},
        {"st_break_black", st_break_black, stf_none, material_stone, 1.0, MOVABLE_BREAKABLE},
        {"st_break_white", st_break_white, stf_none, material_stone, 1.0, MOVABLE_BREAKABLE},
        {"st_break_oxydc", st_break_oxydc, stf_none, material_stone, 1.0, MOVABLE_BREAKABLE},
        {"st_break_boulder", st_break_boulder, stf_none, material_stone, 1.0, MOVABLE_BREAKABLE},
        {"st_break_bug", st_break_bug, stf_none, material_stone, 1.0, MOVABLE_BREAKABLE},
        {"st_break_laser", st_break_laser, stf_none, material_stone, 1.0, MOVABLE_BREAKABLE},
    };
    
    BOOT_REGISTER_START
        BootRegister(new BreakStone(0), "st_break");
        BootRegister(new BreakStone(0), "st_break_plain");
        BootRegister(new BreakStone(1), "st_break_black");
        BootRegister(new BreakStone(2), "st_break_white");
        BootRegister(new BreakStone(3), "st_break_oxydc");
        BootRegister(new BreakStone(4), "st_break_boulder");
        BootRegister(new BreakStone(5), "st_break_bug");
        BootRegister(new BreakStone(6), "st_break_laser");
    BOOT_REGISTER_END

} // namespace enigma
