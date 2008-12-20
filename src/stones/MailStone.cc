/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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

#include "stones/MailStone.hh"
#include "Inventory.hh"
#include "player.hh"
#include "world.hh"
#include "items/PipeItem.hh"
//#include "main.hh"

namespace enigma {
    MailStone::MailStone(Direction dir) : Stone("st_mail") {
        setAttr("orientation", Value(dir));
    }
    
    std::string MailStone::getClass() const {
        return "st_mail";
    }
    
    void MailStone::setAttr(const string& key, const Value &val) {
        if (isDisplayable())
            if (key == "orientation") {
                Stone::setAttr(key, val);
                init_model();
                return;
            }
        Stone::setAttr(key, val);
    }

    void MailStone::init_model() {
        string mname = "st-mail" ;
        set_model(mname + to_suffix(getOrientation()));
    }
    
    void MailStone::actor_hit(const StoneContact &sc) {
        if (enigma::Inventory *inv = player::GetInventory(sc.actor)) {
            if (Item *it = inv->get_item(0)) {
                GridPos p = PipeItem::findPipeEndpoint(get_pos(), getOrientation());
                if (IsInsideLevel(p) && it->can_drop_at(p)) {
                    it = inv->yield_first();
                    player::RedrawInventory(inv);
                    it->drop(sc.actor, p);
                }
            }
        }
    }
    
    Direction MailStone::getOrientation() const {
        return to_direction(getAttr("orientation"));
    }
        
    BOOT_REGISTER_START
        BootRegister(new MailStone(NORTH), "st_mail");
        BootRegister(new MailStone(NORTH), "st_mail_n");
        BootRegister(new MailStone(EAST),  "st_mail_e");
        BootRegister(new MailStone(SOUTH), "st_mail_s");
        BootRegister(new MailStone(WEST),  "st_mail_w");
    BOOT_REGISTER_END

} // namespace enigma
