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

#include "stones/FlatStone.hh"
#include "client.hh"
#include "Inventory.hh"
#include "items/GlassesItem.hh"
#include "player.hh"
//#include "world.hh"
//#include "main.hh"

namespace enigma {
    FlatStone::FlatStone(bool hollow, bool movable, bool breakable, bool cracked, bool burnable, bool breaking) {
        if (hollow)
            objFlags |= OBJBIT_HOLLOW;
        if (movable)
            objFlags |= OBJBIT_MOVABLE;
        if (breakable)
            objFlags |= OBJBIT_BREAKABLE;
        if (burnable)
            objFlags |= OBJBIT_BURNABLE;
        if (cracked)
            state = CRACKED;
        if (breaking)
            state = BREAKING;
    }
    
    std::string FlatStone::getClass() const {
        return get_traits().name;
    }
    
    Value FlatStone::getAttr(const std::string &key) const {
        if (key == "movable") {
            return (bool)(objFlags & OBJBIT_MOVABLE);
        } else if (key == "hollow") {
            return (bool)(objFlags & OBJBIT_HOLLOW);
        } else if (key == "breakable") {
            return (bool)(objFlags & OBJBIT_BREAKABLE);
        } else if (key == "burnable") {
            return (bool)(objFlags & OBJBIT_BURNABLE);
        }
        return Stone::getAttr(key);
    }
    
    Value FlatStone::message(const Message &m) {
        if (m.message == "_glasses") {
            if ((bool)((to_int(m.value) & Glasses::HOLLOW)) != (bool)(objFlags & OBJBIT_GLASSES)) {
                objFlags ^= OBJBIT_GLASSES;
                if (isDisplayable())
                    init_model();
            }
            return Value();
        } else if (((objFlags & (OBJBIT_BREAKABLE | OBJBIT_MOVABLE)) || state == CRACKED) && 
                (m.message =="ignite" || m.message == "_explosion")) {
            doBreak();
            return Value();
        } else if ((m.message == "_trigger" || m.message == "signal") && state == IDLE) {
            if (!(objFlags & (OBJBIT_BREAKABLE | OBJBIT_MOVABLE | OBJBIT_HOLLOW | OBJBIT_BURNABLE))) {
                objFlags |= OBJBIT_HOLLOW;
            } else if (objFlags & OBJBIT_HOLLOW) {
                objFlags &= ~OBJBIT_HOLLOW;                
            }
            if (isDisplayable())
                init_model();
            return Value();
        } else if (m.message == "_fire" && (objFlags & OBJBIT_BURNABLE)) {
            doBreak();
            return true;
        } else if (m.message == "heat" && (objFlags & OBJBIT_BURNABLE) && !(objFlags & OBJBIT_MOVABLE)) {
            doBreak();
            return true;
        }
        return Stone::message(m);
    }
   
    void FlatStone::setState(int extState) {
        // block all state writes
    }
    
    void FlatStone::init_model() {
        if ((objFlags & OBJBIT_GLASSES) && (objFlags & OBJBIT_HOLLOW))
            set_model("invisible");
        else if (state == BREAKING)
            set_anim("st_flat_breaking");
        else if (state == FALLING)
            set_anim("st_flat_falling");
        else if (state == CRACKED)
            set_model("st_flat_cracked");
        else if (objFlags & OBJBIT_BURNABLE)
            set_model("st_flat_fire");
        else
            set_model("st_flat");
    }
    
    void FlatStone::processLight(Direction d) {
        if (state == IDLE && (objFlags & OBJBIT_BREAKABLE))
            doBreak();
        else if (state == IDLE && !(objFlags & (OBJBIT_BREAKABLE | OBJBIT_MOVABLE | OBJBIT_HOLLOW | OBJBIT_BURNABLE))) {
            state = CRACKED;
            init_model();
        } else
            Stone::processLight(d);
    }
    
    void FlatStone::animcb() {
        if (state == FALLING)
            sound_event("stonedestroy");
        KillStone(get_pos());
    }
    
    bool FlatStone::is_floating() const {
        return objFlags & OBJBIT_HOLLOW;
    }
    
    void FlatStone::on_floor_change() {
        if ((state == IDLE) && (objFlags & (OBJBIT_BREAKABLE | OBJBIT_MOVABLE))) {
            GridPos p = get_pos();
            if (Floor *fl = GetFloor(p)) {
                if (fl->isKind("fl_abyss")) {
                    state = FALLING;
                    init_model();
                }
                else if ((objFlags & OBJBIT_MOVABLE) && (fl->getClass() == "fl_swamp" || fl->getClass() =="fl_water")) {
                    sound_event("drown");
                    client::Msg_Sparkle(p.center());
                    KillStone(p);
                }
            }
        }
    }

    StoneResponse FlatStone::collision_response(const StoneContact &sc) {
        if (objFlags & OBJBIT_HOLLOW)
            return STONE_PASS;
        return Stone::collision_response(sc);
    }

    const char *FlatStone::collision_sound() {
        if (state == BREAKING || state == CRACKED || (objFlags & OBJBIT_BREAKABLE))
            return "metal";
        else
            return "stone";
    }
    
    void FlatStone::actor_hit(const StoneContact &sc) {
        if (((objFlags & OBJBIT_BREAKABLE) || state == CRACKED) && player::WieldedItemIs(sc.actor, "it_hammer"))
            doBreak();
        else if ((state == IDLE && !(objFlags & (OBJBIT_BREAKABLE | OBJBIT_HOLLOW))) &&
                (player::WieldedItemIs(sc.actor, "it_pencil"))) {
            enigma::Inventory *inv = player::GetInventory(sc.actor);
            if (inv && inv->size() > 0) {
                delete inv->yield_item(0);
                player::RedrawInventory(inv);
                sound_event("stonepaint");
                objFlags |= OBJBIT_BURNABLE;
                init_model();
            }
        } else if (state == IDLE && (objFlags & OBJBIT_BURNABLE) &&
                player::WieldedItemIs(sc.actor, "it_brush")) {
            sound_event("stonepaint");
            objFlags &= ~OBJBIT_BURNABLE;
            init_model();
        } else
            Stone::actor_hit(sc);
    }
    
    void FlatStone::doBreak() {
        if (state <= CRACKED) {
            state = BREAKING;
            sound_event("stonedestroy");
            if (isDisplayable())
                init_model();
        }
    }
        
    int FlatStone::traitsIdx() const {
        return (((objFlags & OBJBIT_BREAKABLE)|| state == CRACKED) ? 1 : 0) + ((objFlags & OBJBIT_MOVABLE) ? 2 : 0);
    }
    
    StoneTraits FlatStone::traits[4] = {
        {"st_flat", st_standard, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_flat", st_standard, stf_none, material_stone, 1.0, MOVABLE_BREAKABLE},
        {"st_flat", st_standard, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
        {"st_flat", st_standard, stf_none, material_stone, 1.0, MOVABLE_IRREGULAR},
    };
    
    BOOT_REGISTER_START
        BootRegister(new FlatStone(false, false), "st_flat");
        BootRegister(new FlatStone(true, false),  "st_flat_hollow");
        BootRegister(new FlatStone(false, true),  "st_flat_movable");
        BootRegister(new FlatStone(false, false, true), "st_flat_breakable");
        BootRegister(new FlatStone(false, false, false, true), "st_flat_cracked");
        BootRegister(new FlatStone(false, false, false, false, true), "st_flat_burnable");
        BootRegister(new FlatStone(false, true, false, false, true), "st_flat_moveburnable");
        BootRegister(new FlatStone(false, false, true, false, false, true), "st_flat_breaking");
    BOOT_REGISTER_END

} // namespace enigma
