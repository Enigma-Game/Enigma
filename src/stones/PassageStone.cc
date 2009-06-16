/*
 * Copyright (C) 2008 Ronald Lamprecht
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

#include "stones/PassageStone.hh"

#include "errors.hh"
//#include "main.hh"

namespace enigma {

    PassageStone::PassageStone(int color, int flavor) : Stone() {
        state = color;
        objFlags |= (flavor << 24);
    }
    
    std::string PassageStone::getClass() const {
        return "st_passage";
    }
    
    void PassageStone::setAttr(const string& key, const Value &val) {
        if (key == "color") {
            Stone::setAttr("state", val);
        } else if (key == "flavor") {
            std::string fs = val.to_string();
            int fo = (objFlags & OBJBIT_FLAVOR) >> 24;
            int fi = -1;
            if (fs == "square") fi = 0;
            else if (fs == "slash") fi = 1;
            else if (fs == "cross") fi = 2;
            else if (fs == "frame") fi = 3;
            if (fi != -1 && fi !=fo) {
                objFlags = (objFlags & ~OBJBIT_FLAVOR) | (fi << 24);
                if (isDisplayable())
                    init_model();
            }
            return;
        } else
            Stone::setAttr(key, val);
    }
    
    Value PassageStone::getAttr(const std::string &key) const {
        if (key == "color") {
            return state;
        } else if (key == "flavor") {
            return flavor();
        }
        return Stone::getAttr(key);
    }
    
    Value PassageStone::message(const Message &m) {
        if (m.message == "signal" || m.message == "_trigger") {
            toggleState();
            return Value();
        }
        return Stone::message(m);
    }
    
    void PassageStone::init_model()  {
        set_model(ecl::strf("st_passage_%s_%s", (state == BLACK) ? "black" : "white", flavor().c_str()));
    }

    bool PassageStone::is_floating() const {
        return true;
    }
    
    bool PassageStone::is_transparent (Direction d) const {
        return true;
    }
    
    StoneResponse PassageStone::collision_response(const StoneContact &sc) {
        Value accolor = sc.actor->getAttr("color"); 
        if (state == BLACK) {
            return (accolor && accolor == BLACK) ?  STONE_PASS : STONE_REBOUND;
        }
        else {
            return (accolor && accolor == WHITE) ?  STONE_PASS : STONE_REBOUND;
        }
    }
    
    std::string PassageStone::flavor() const {
        switch ((objFlags & OBJBIT_FLAVOR) >> 24) {
            case 0 : return "square";
            case 1 : return "slash";
            case 2 : return "cross";
            case 3 : return "frame";
        }
        ASSERT(false, XLevelRuntime, "PassageStone unknown flavor");
    }

    int PassageStone::traitsIdx() const {
        return ((objFlags & OBJBIT_FLAVOR) >> 24) + 4 * state;
    }

    StoneTraits PassageStone::traits[8] = {
        {"st_passage_black_square", st_passage_black_square, stf_transparent, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_passage_black_slash",  st_passage_black_slash,  stf_transparent, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_passage_black_cross",  st_passage_black_cross,  stf_transparent, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_passage_black_frame",  st_passage_black_frame,  stf_transparent, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_passage_white_square", st_passage_white_square, stf_transparent, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_passage_white_slash",  st_passage_white_slash,  stf_transparent, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_passage_white_cross",  st_passage_white_cross,  stf_transparent, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_passage_white_frame",  st_passage_white_frame,  stf_transparent, material_stone, 1.0, MOVABLE_PERSISTENT},
    };
    
    BOOT_REGISTER_START
        BootRegister(new PassageStone(BLACK, 0), "st_passage");
        BootRegister(new PassageStone(BLACK, 0), "st_passage_black");
        BootRegister(new PassageStone(BLACK, 0), "st_passage_black_square");
        BootRegister(new PassageStone(BLACK, 1), "st_passage_black_slash");
        BootRegister(new PassageStone(BLACK, 2), "st_passage_black_cross");
        BootRegister(new PassageStone(BLACK, 3), "st_passage_black_frame");
        BootRegister(new PassageStone(WHITE, 0), "st_passage_white");
        BootRegister(new PassageStone(WHITE, 0), "st_passage_white_square");
        BootRegister(new PassageStone(WHITE, 1), "st_passage_white_slash");
        BootRegister(new PassageStone(WHITE, 2), "st_passage_white_cross");
        BootRegister(new PassageStone(WHITE, 3), "st_passage_white_frame");
    BOOT_REGISTER_END

} // namespace enigma
