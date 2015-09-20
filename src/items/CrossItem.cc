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

#include "items/CrossItem.hh"
#include "server.hh"
#include "world.hh"

namespace enigma {

    CrossItem::~CrossItem() {
        GameTimer.remove_alarm(this);
    }
    
    Value CrossItem::message(const Message &m) {
        if (server::GameCompatibility == enigma::GAMET_PEROXYD) {
            // CrossItemes can be used to invert signals in Per.Oxyd
            if (m.message == "signal") {
                performAction(!m.value.to_bool()); // convert 1/0 values to true/false
                return Value();
            }
        } else if (server::GameCompatibility == GAMET_ENIGMA) {
            if (m.message == "_brush") {
                KillItem(this->get_pos());
                return Value();
            }
        }
        return Item::message(m);
    }
    
    void CrossItem::setState(int extState) {
        return;   // ignore any write attempts
    }

    void CrossItem::actor_enter(Actor *a) {
        if ((state == 0) && a->getAttr("owner")) {
            state = 1;
            GameTimer.set_alarm (this, getAttr("interval"));
        }
    }

    void CrossItem::actor_leave(Actor *) {
        if (state == 1) {
            GameTimer.remove_alarm (this);
            state = 0;
        }
    }

    void CrossItem::alarm() {
        state = 0;
        performAction(true);
    }
    
    DEF_ITEMTRAITSF(CrossItem, "it_cross", it_cross, itf_static | itf_indestructible);

    BOOT_REGISTER_START
        BootRegister(new CrossItem(), "it_cross");
    BOOT_REGISTER_END

} // namespace enigma
