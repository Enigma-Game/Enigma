/*
 * Copyright (C) 2008,2009 Ronald Lamprecht
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

#include "stones/BoxStone.hh"
#include "errors.hh"
#include "Inventory.hh"
#include "player.hh"
#include "main.hh"
#include "lua.hh"

namespace enigma {
    BoxStone::BoxStone(int subtyp, int initState) : Stone() {
        objFlags |= (subtyp << 25);
        if (subtyp == ROCK)
            objFlags |= OBJBIT_BLOCKFIRE;
        state = initState;
    }
    
    BoxStone* BoxStone::clone() {
        if ((BoxStoneTyp)((objFlags & OBJBIT_SUBTYP) >> 25) == WOOD) { 
             // When st_box_wood is created it randomly becomes st_box_wood1 or st_box_wood2.
             if (IntegerRand(0, 1) == 0)
                return new BoxStone(WOOD1, state);
            else
                return new BoxStone(WOOD2, state);
        } else {
            return new BoxStone(*this);
        }
    }
    
    void BoxStone::dispose() {
        delete this;
    }
    
    std::string BoxStone::getClass() const {
        return "st_box";
    }
    
    Value BoxStone::getAttr(const std::string &key) const {
        if (key == "flavor") {
            BoxStoneTyp typ = (BoxStoneTyp)((objFlags & OBJBIT_SUBTYP) >> 25);
            switch (typ) {
                case WOOD:
                case WOOD1:
                case WOOD2:
                    return "wood";
                case HAY:
                    return "hay";
                case ROCK:
                    return "rock";
            }
        }
        return Stone::getAttr(key);
    }
    

    Value BoxStone::message(const Message &m) {
        if (m.message == "_fire" && !(objFlags & OBJBIT_BLOCKFIRE)) {
            KillStone(get_pos());
            return true;  // allow fire to spread
        } else if (m.message == "heat") {
            return (objFlags & OBJBIT_BLOCKFIRE) != 0;  // block fire
        } else if (m.message == "_fall") {
            maybe_fall_or_stopfire();
            return Value();
        }
        return Stone::message(m);
    }

    void BoxStone::setState(int extState) {
        // no external states
    }
    
    void BoxStone::init_model() {
        BoxStoneTyp typ = (BoxStoneTyp)((objFlags & OBJBIT_SUBTYP) >> 25);
        switch (typ) {
            case WOOD:
                ASSERT(false, XLevelRuntime, "Wood stone init model state error");
            case WOOD1:
                if (state == GROWING)
                    set_anim("st_box_wood_growing");
                else if (state == BREAKING)
                    set_anim("st_box_wood1_breaking");
                else
                    set_model("st_box_wood1");
                break;
            case WOOD2:
                if (state == GROWING)
                    set_anim("st_box_wood_growing");
                else if (state == BREAKING)
                    set_anim("st_box_wood2_breaking");
                else
                    set_model("st_box_wood2");
                break;
            case HAY:
                if (state == GROWING)
                    set_anim("st_box_hay_growing");
                else
                    set_model("st_box_hay"); break;
                break;
            case ROCK:
                set_model("st_box_rock"); break;
        }
    }
    
    void BoxStone::animcb() {
        if (state == BREAKING) {
            enigma::GridPos pos = get_pos();
            CoverFloor(pos, "fl_wood_framed");
            CoverFloor(move(pos, NORTH), "fl_wood_framed");
            CoverFloor(move(pos, EAST), "fl_wood_framed");
            CoverFloor(move(pos, SOUTH), "fl_wood_framed");
            CoverFloor(move(pos, WEST), "fl_wood_framed");
            TokenList tl = getAttr("selection");
            if (tl.size() > 0) {
                // First decide which tile to set.
                int idx = enigma::IntegerRand(1, tl.size()) - 1;
                TokenList::iterator itr = tl.begin();
                for (int i = 0; i < idx; i++, ++itr);
                std::string name = (*itr).to_string();
                // Now kill the box.
                KillStone(get_pos());
                // Set the new tile or item. An earlier item might be overwritten.
                if (name.find('=') == 0) {
                    if (lua::CallFunc(lua::LevelState(), "enigma.settile", name.substr(1), GetFloor(pos))) {
                        throw XLevelRuntime(std::string("box set tile failed:\n")+lua::LastError(lua::LevelState()));
                    }
                } else {
                    SetItem(pos, MakeItem(name.c_str()));
                }
            } else {
                KillStone(get_pos());
            }
        } else {
            state = IDLE;
            init_model();
            maybe_fall_or_stopfire(); // instantly builds a bridge on fl_swamp etc
        }
    }

    bool BoxStone::allowsSpreading(Direction dir, bool isFlood) const {
        return isFlood;
    }
    
    void BoxStone::actor_hit(const StoneContact &sc) {
        if (state == GROWING)
            SendMessage(sc.actor, "_shatter");
        else if (state == IDLE) {
            BoxStoneTyp typ = (BoxStoneTyp)((objFlags & OBJBIT_SUBTYP) >> 25);
            if (   ((typ == WOOD) || (typ == WOOD1) || (typ == WOOD2))
                && player::WieldedItemIs(sc.actor, "it_axe")) {
                enigma::Inventory *inv = player::GetInventory(sc.actor);
                if (inv && inv->size() > 0) {
                    delete inv->yield_item(0);
                    player::RedrawInventory(inv);
                    doBreak();
                }
            } else
                Stone::actor_hit(sc);
        }
    }
    
    void BoxStone::actor_inside(Actor *a) {
        if (state == GROWING)
            SendMessage(a, "_shatter");
    }
    
    void BoxStone::actor_contact(Actor *a) {
        if (state == GROWING)
            SendMessage(a, "_shatter");
    }
    
    void BoxStone::doBreak() {
        if (state == IDLE) {
            state = BREAKING;
            sound_event("stonedestroy");
            init_model();
        }
    }

    bool BoxStone::on_move(const GridPos &origin) {
        // in oxyd1 only fall when moving
        Stone::on_move(origin);
        maybe_fall_or_stopfire(true);
        return true;
    }
    
    void BoxStone::on_floor_change() {
        // other oxyds versions: fall everytime the floor changes
        maybe_fall_or_stopfire(false, true);
    }
    
    void BoxStone::maybe_fall_or_stopfire(bool onMove, bool onFloorChange) {
        BoxStoneTyp typ = (BoxStoneTyp)((objFlags & OBJBIT_SUBTYP) >> 25);
        GridPos p = get_pos();
        if (server::GameCompatibility != GAMET_ENIGMA && IsLevelBorder(p))
            return;
        if (Floor *oldfl = GetFloor(p)) {
            std::string k = oldfl->getClass();
            if (objFlags & OBJBIT_BLOCKFIRE)
                SendMessage(oldfl, "stopfire");
            if (k == "fl_abyss" || k == "fl_water" || k == "fl_swamp") {
                if (onMove)
                    // just mark - can not kill stone yet - this will be done on floor change event
                    state = FALLING;  // keep the stone from moving any longer
                else if ((server::GameCompatibility != GAMET_OXYD1) || state == FALLING || !onFloorChange) {
                    state = FALLEN;
                    Floor *newfl = MakeFloor((typ == HAY) ? "fl_hay_framed" : ((typ == ROCK) ? "fl_rock_framed" : ((typ == WOOD1) ? "fl_wood_framed_h" : "fl_wood_framed_v")));
                    transferIdentity(newfl);
                    if (Value v = getAttr("secure"))
                        newfl->setAttr("secure", v);
                    if (k == "fl_water") {
                        newfl->setAttr("interval", oldfl->getAttr("interval"));
                        newfl->setAttr("state", oldfl->getAttr("state"));
                    }
                    SetFloor(p, newfl); 
                    KillStone(p);
                }
            }
        }
    }
    
    int BoxStone::traitsIdx() const {
        return (state == IDLE) ? 0 : 1;
    }

    StoneTraits BoxStone::traits[2] = {
        {"st_box", st_box, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
        {"st_box_wood_growing", st_box_wood_growing, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
    };

    BOOT_REGISTER_START
        BootRegister(new BoxStone(0), "st_box");
        BootRegister(new BoxStone(0), "st_box_wood");
        BootRegister(new BoxStone(0, 1), "st_box_wood_growing");
        BootRegister(new BoxStone(1), "st_box_wood_h");
        BootRegister(new BoxStone(2), "st_box_wood_v");
        BootRegister(new BoxStone(3), "st_box_hay");
        BootRegister(new BoxStone(3, 1), "st_box_hay_growing");
        BootRegister(new BoxStone(4), "st_box_rock");
    BOOT_REGISTER_END

} // namespace enigma
