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

#include "stones/PortalStone.hh"
//#include "errors.hh"
//#include "main.hh"

namespace enigma {
    PortalStone::PortalStone(int typ) : Stone() {
        state = typ;
    }

    std::string PortalStone::getClass() const {
        return "st_portal";
    }

    Value PortalStone::getAttr(const std::string &key) const {
        if (key == "flavor") {
            switch (state) {
                case HORSE:
                    return "horse";
                case PEARL:
                    return "pearl";
            }
        }
        return Stone::getAttr(key);
    }

    void PortalStone::setState(int extState) {
        // no external states
    }

    void PortalStone::init_model() {
        switch (state) {
            default:
            case HORSE:
            case PEARL:
                set_model("st_portal_horse");
                break;
        }
    }

    bool PortalStone::is_floating() const {
        return true;
    }

    StoneResponse PortalStone::collision_response(const StoneContact &sc) {
        ActorID theid = get_id(sc.actor);
        if ((state == HORSE && theid == ac_horse) || (state == PEARL && theid == ac_pearl_white))
            return STONE_PASS;
        else
            return STONE_REBOUND;
    }

    DEF_TRAITS(PortalStone, "st_portal", st_portal);

    BOOT_REGISTER_START
        BootRegister(new PortalStone(0), "st_portal");
        BootRegister(new PortalStone(0), "st_portal_horse");
        BootRegister(new PortalStone(1), "st_portal_pearl");
    BOOT_REGISTER_END

} // namespace enigma
