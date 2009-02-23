/*
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

#include "stones/DispenserStone.hh"
#include "errors.hh"
#include "Inventory.hh"
//#include "main.hh"
#include "player.hh"
#include "world.hh"

namespace enigma {
    DispenserStone::DispenserStone(int subtyp) : Stone() {
        objFlags |= (subtyp << 24);
    }
    
    std::string DispenserStone::getClass() const {
        return "st_dispenser";
    }
    
    Value DispenserStone::getAttr(const std::string &key) const {
        if (key == "flavor") {
            DispenserStoneTyp typ = (DispenserStoneTyp)((objFlags & OBJBIT_SUBTYP) >> 24);
            switch (typ) {
                case BOMBBLACK:
                    return "bombblack";
                case BOMBWHITE:
                    return "bombwhite";
                case DYNAMITE:
                    return "dynamite";
            }
        }
        return Stone::getAttr(key);
    }

    Value DispenserStone::message(const Message &m) {
        if (m.message =="_explosion" || m.message =="_dispenser") {
            if (isDisplayable())
                doBreak();
            return Value();
        }
        return Stone::message(m);
    }

    void DispenserStone::setState(int extState) {
        // no external states
    }
    
    void DispenserStone::init_model() {
        std::string base = get_traits().name;
        if (state == BREAKING)
            set_anim(base + "_breaking");
        else
            set_model(base);
    }
    
    void DispenserStone::animcb() {
        GridPos p = get_pos();
        SendExplosionEffect(p, EXPLOSION_DISPENSER);
        if (Item *it = GetItem(get_pos())) {
            SendMessage(it, "ignite");
        } else
            SetItem(p, MakeItem("it_explosion_nil"));
        KillStone(p);
    }

    void DispenserStone::actor_hit(const StoneContact &sc) {
        if (enigma::Inventory *inv = player::GetInventory(sc.actor)) {
            if (!inv->is_full()) {
                DispenserStoneTyp typ = (DispenserStoneTyp)((objFlags & OBJBIT_SUBTYP) >> 24);
                std::string itemkind;
                switch (typ) {
                    case BOMBBLACK:
                        itemkind = "it_bomb_black"; break;
                    case BOMBWHITE:
                        itemkind = "it_bomb_white"; break;
                    case DYNAMITE:
                        itemkind = "it_dynamite"; break;
                    default:
                        ASSERT(false, XLevelRuntime, "Dispenser - unexpected subtyp");
                }
                inv->add_item(MakeItem(itemkind.c_str()));
                player::RedrawInventory(inv);
            }
        }
    }
    
    void DispenserStone::doBreak() {
        if (state == IDLE) {
            state = BREAKING;
            sound_event ("stonedestroy");
            init_model();
        }
    }
    
    int DispenserStone::traitsIdx() const {
        return (objFlags & OBJBIT_SUBTYP) >> 24;
    }

    StoneTraits DispenserStone::traits[3] = {
        {"st_dispenser_bombblack", st_dispenser, stf_none, material_stone, 1.0, MOVABLE_BREAKABLE},
        {"st_dispenser_bombwhite", st_dispenser, stf_none, material_stone, 1.0, MOVABLE_BREAKABLE},
        {"st_dispenser_dynamite", st_dispenser, stf_none, material_stone, 1.0, MOVABLE_BREAKABLE},
    };

    BOOT_REGISTER_START
        BootRegister(new DispenserStone(0), "st_dispenser");
        BootRegister(new DispenserStone(0), "st_dispenser_bombblack");
        BootRegister(new DispenserStone(1), "st_dispenser_bombwhite");
        BootRegister(new DispenserStone(2), "st_dispenser_dynamite");
    BOOT_REGISTER_END

} // namespace enigma
