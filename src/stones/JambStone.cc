/*
 * Copyright (C) 2008,2009,2010 Ronald Lamprecht
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

#include "stones/JambStone.hh"
#include "errors.hh"
//#include "main.hh"

namespace enigma {
    JambStone::JambStone(int color) : Stone() {
        Stone::setAttr("color", color);
    }
    
    std::string JambStone::getClass() const {
        return "st_jamb";
    }
    
    Value JambStone::message(const Message &m) {
        if (m.message == "signal" || m.message == "hit") {
            if (GridObject *sender = dynamic_cast<GridObject*>(m.sender)) {
                GridPos p = get_pos();
                Object *o;
                if (m.message == "hit")
                    o = m.value;
                else
                    o = SendMessage(m.sender, "_hitactor");
                
                Actor *a = dynamic_cast<Actor *>(o);
                int c = getAttr("color");
                if ((a != NULL && (server::GameCompatibility == GAMET_ENIGMA || a->getClass() == "ac_marble") &&
                        a->getAttr("color") && a->getAttr("color") == c) || 
                        (m.sender->getObjectType() != Object::ITEM && m.message == "signal")) {
                    if (p.y == sender->get_pos().y) {
                        SendMessage (GetStone (move (p, EAST)),  "signal", 1.0);
                        SendMessage (GetStone (move (p, WEST)),  "signal", 1.0);
                        SendMessage (GetStone (move (p, NORTH)), "signal", 0.0);
                        SendMessage (GetStone (move (p, SOUTH)), "signal", 0.0);
                    } else {
                        SendMessage (GetStone (move (p, EAST)),  "signal", 0.0);
                        SendMessage (GetStone (move (p, WEST)),  "signal", 0.0);
                        SendMessage (GetStone (move (p, NORTH)), "signal", 1.0);
                        SendMessage (GetStone (move (p, SOUTH)), "signal", 1.0);
                    }
                    return Value();
                }
            }
        }
        return Stone::message(m);
    }

    void JambStone::init_model() {
        set_model(ecl::strf("st_jamb_%s", getAttr("color") == BLACK ? "black" : "white"));
    }
    
    DEF_TRAITS(JambStone, "st_jamb", st_jamb);
    
    BOOT_REGISTER_START
        BootRegister(new JambStone(0), "st_jamb");
        BootRegister(new JambStone(0), "st_jamb_black");
        BootRegister(new JambStone(1), "st_jamb_white");
    BOOT_REGISTER_END

} // namespace enigma
