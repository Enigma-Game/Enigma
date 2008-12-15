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

#include "items/Coin.hh"
//#include "errors.hh"
//#include "main.hh"
//#include "player.hh"
#include "world.hh"

namespace enigma {

    Coin::Coin(int type) {
        state = type;
        setAttr("coin_value", state == 0 ? 3.0 : (state == 1 ? 6.0 : 12.0));
    }
    
    std::string Coin::getClass() const {
        return "it_coin";
    }
    
    void Coin::setState(int extState) {
        // ignore any state access
    }

    void Coin::processLight(Direction d) {
        sound_event("itemtransform");
        switch (state) {
            case 0 : 
                transform("it_umbrella_new"); break;
            case 1 :
                transform("it_hammer_new"); break;
            case 2 :
                transform("it_extralife_new"); break;
        }
    }

    void Coin::on_stonehit(Stone *) {
        if (state <= 1) {
            state += 1;
            setAttr("coin_value", (state == 1 ? 6.0 : 12.0));  // API 1 compatibility
            init_model();
        }

    }
    
    ItemTraits Coin::traits[3] = {
        {"it_coin_s",  it_coin1,  itf_none, 0.0},
        {"it_coin_m",  it_coin2,  itf_none, 0.0},
        {"it_coin_l",  it_coin4,  itf_none, 0.0},
    };

    BOOT_REGISTER_START
        BootRegister(new Coin(0), "it_coin_s");
        BootRegister(new Coin(1), "it_coin_m");
        BootRegister(new Coin(2), "it_coin_l");
    BOOT_REGISTER_END

} // namespace enigma
