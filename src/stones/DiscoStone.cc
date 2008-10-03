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

#include "stones/DiscoStone.hh"
#include "main.hh"

namespace enigma {
    DiscoStone::DiscoStone(int initState) : Stone () {
        state = initState;
    }

    std::string DiscoStone::getClass() const {
        return "st_disco";
    }
        
    Value DiscoStone::message(const Message &m) {
        bool handled = false;
        bool oldBehaviour = server::EnigmaCompatibility < 1.10;
        int orgState = state;
        if (m.message == "lighten") {
            setAttr("state", state - (oldBehaviour ? 2 : 1));
            handled = true;
        } else if (m.message == "darken") {
            setAttr("state", state + (oldBehaviour ? 2 : 1));
            handled = true;
        } else if (m.message == "signal") {
            setAttr("state", ecl::Clamp(state + ((m.value != 0) ? -2 : +2), (int)LIGHT, (int)DARK));
            handled = true;
        } else
            return Stone::message(m);
        
        if (handled && oldBehaviour && orgState != state) {
            // recursive distribution of light change
            for (Direction d = NORTH; d != NODIR; d = previous(d)) {
                Stone * st = GetStone(move(get_pos(), d));
                if (st != NULL && st->getClass() == "st_disco")
                    SendMessage(st, "signal", (state < orgState) ? 1 : 0, this);
            }
        }
        return Value();
    }

    int DiscoStone::maxState() const {
        return DARK;
    }
    
    void DiscoStone::init_model() {
        set_model(ecl::strf("st_disco%d", state));
    }

    bool DiscoStone::is_floating() const {
        return true;
    }
    
    StoneResponse DiscoStone::collision_response(const StoneContact &sc) {
        return STONE_PASS;
    }
    
    DEF_TRAITS(DiscoStone, "st_disco", st_disco);
    
    BOOT_REGISTER_START
        BootRegister(new DiscoStone(0), "st_disco");
        BootRegister(new DiscoStone(0), "st_disco_light");
        BootRegister(new DiscoStone(1), "st_disco_medium");
        BootRegister(new DiscoStone(2), "st_disco_dark");
    BOOT_REGISTER_END

} // namespace enigma
