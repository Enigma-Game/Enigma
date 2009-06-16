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

#include "stones/KnightStone.hh"

#include "client.hh"
//#include "main.hh"
#include "nls.hh"
#include "player.hh"

namespace enigma {

    KnightStone::KnightStone() : Stone() {
    }
    
    std::string KnightStone::getClass() const {
        return "st_knight";
    }

    int KnightStone::maxState() const {
        return 4;  // dir representing state
    }

    void KnightStone::setState(int extState) {
        // no write access
    }
     
    void KnightStone::init_model()  {
        set_model(ecl::strf("st_knight%d", state + 1));
    }
    
    StoneResponse KnightStone::collision_response(const StoneContact &) {
        return (state == maxState()) ? STONE_PASS : STONE_REBOUND;
    }
    
    void KnightStone::actor_hit(const StoneContact &sc) {
        if (state != maxState()) {
            if (player::WieldedItemIs(sc.actor, "it_sword")) {
                state += 1;
                if (state == maxState()) {
                    client::Msg_ShowText(N_("All right, we'll call it a draw"), false, 4.0);
                }
                init_model();
            } else {
                SendMessage(sc.actor, "_shatter");
            }
        }
    }
    
    bool KnightStone::is_floating() const {
         return state == maxState();
    }

    DEF_TRAITSM(KnightStone, "st_knight", st_knight, MOVABLE_PERSISTENT);
    
    BOOT_REGISTER_START
        BootRegister(new KnightStone(), "st_knight");
    BOOT_REGISTER_END

} // namespace enigma
