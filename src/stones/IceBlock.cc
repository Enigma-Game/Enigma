/*
 * Copyright (C) 2009,2010 Ronald Lamprecht
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

#include "stones/IceBlock.hh"

#include "items.hh"
//#include "main.hh"

namespace enigma {

    IceBlock::IceBlock() : Stone() {
    }
    
    std::string IceBlock::getClass() const {
        return "st_ice";
    }
    
    Value IceBlock::message(const Message &m) {
        if (m.message == "_explosion" || m.message == "ignite" || m.message == "_cannonball") {
            if (isDisplayable() && state != BREAKING) {  // priorise explosion over melting
                state = BREAKING;
                set_anim("st_ice_breaking");
            }
            return Value();
        } else if (m.message == "heat") {
            if (isDisplayable() && state == IDLE) {
                state = MELTING;
                set_anim("st_ice_melting");
            }
            return Value();            
        }
        return Stone::message(m);
    }
    
    void IceBlock::setState(int extState) {
        // block all state write attempts
    }
    
    void IceBlock::init_model() {
        set_model("st_ice");
    }
    
    void IceBlock::animcb() {
        GridPos p = get_pos();
        if (state == BREAKING) {
            CoverFloor(p, "fl_ice");
            CoverFloor(move(p, NORTH), "fl_ice");
            CoverFloor(move(p, EAST), "fl_ice");
            CoverFloor(move(p, SOUTH), "fl_ice");
            CoverFloor(move(p, WEST), "fl_ice");
        } else {  // MELTING
            CoverFloor(p, "fl_water");
        }
        KillStone(p);
    }
    
    bool IceBlock::is_transparent(Direction d) const {
        return true;
    }
    
    bool IceBlock::is_removable() const {
        return state == IDLE;
    }
    
    bool IceBlock::is_movable() const {
        return (state == IDLE) ? Stone::is_movable() : false;
    }
    
    bool IceBlock::on_move(const GridPos &origin) {
        bool isFrozenBomb = false;
        Item *itf = GetItem(origin);
        Item *itd = GetItem(get_pos());
        if (itd != NULL) {
            SendMessage(itd, "_freeze");
            itd = GetItem(get_pos());
        }
        if (itf != NULL && itf->isFreezable()) {
            if (itd == NULL || !itd->isStatic() || (itd->isFreezable() && !itf->isPortable())) {
                isFrozenBomb = itf->isKind("it_bomb");
                SetItem(get_pos(), YieldItem(origin));
            } else
                KillItem(origin);
        }
        Floor *fl = GetFloor(get_pos());
        if (fl != NULL && fl->isKind("fl_water")) {
            SetFloor(get_pos(), MakeFloor("fl_ice"));
        }
        Stone::on_move(origin);
        
        // deny all item actions on stone move besides bomb explosions
        itd = GetItem(get_pos());
        return (itd != NULL && itd->isKind("it_bomb") && !isFrozenBomb);
    }
    
    DEF_TRAITSM(IceBlock, "st_ice", st_ice, MOVABLE_STANDARD);

    BOOT_REGISTER_START
        BootRegister(new IceBlock(), "st_ice");
    BOOT_REGISTER_END

} // namespace enigma
