/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007,2008 Ronald Lamprecht
 * Copyright (C) 2008 Raoul Bourquin
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

#include "stones/CoinSlot.hh"
#include "server.hh"
#include "Inventory.hh"
#include "player.hh"
#include "errors.hh"

namespace enigma {
    CoinSlot::CoinSlot() : Stone () {
        state = OFF;
        remaining_time = 0;
    }

    CoinSlot::~CoinSlot() {
        GameTimer.remove_alarm (this);
    }

    int CoinSlot::externalState() const {
        return state % 2; // 0 for OFF, TURNON, 1 for ON
    }

    void CoinSlot::setState(int extState) {
        if (isDisplayable()) {
            if (extState == 0)
                setIState(OFF);
            else if (extState == 1)
                setIState(ON);
        } else
            state = extState;
    }

    void CoinSlot::init_model() {
        set_model(state==ON ? "st-coinslot-active" : "st-coinslot");
    }

    void CoinSlot::animcb() {
        setIState(ON);
    }

    void CoinSlot::actor_hit(const StoneContact &sc) {
        if (enigma::Inventory *inv = player::GetInventory(sc.actor)) {
            if (Item *it = inv->get_item (0)) {
                ItemID id = get_id(it);
                if (id == it_coin1 || id == it_coin2 || id == it_coin4) {
                    double coin_value = it->getAttr("value");
                    remaining_time += coin_value;
                    inv->yield_first();
                    player::RedrawInventory (inv);
                    delete it;
                    setIState(TURNON);
                }
            }
        }
    }

    const char *CoinSlot::collision_sound() {
         return "metal";
    }

    void CoinSlot::tick(double dtime) {
        ASSERT(remaining_time > 0, XLevelRuntime, "CoinSlot: tick called, but no remaining time");

        remaining_time -= dtime;
        if (remaining_time <= 0)
            setIState(OFF);
    }

    void CoinSlot::setIState(iState newIState) {
        if (newIState == ON) {
            if (state == TURNON) {
                state = ON;
                performAction(state == ON);
                GameTimer.activate(this);
            }
            else if (state == ON) {
                // state is already ON, this was just a "coin-supply"
                // Do nothing but change the model
            }
            init_model();
        }
        else if (newIState == TURNON) {
            if (state == OFF) {
                state = TURNON;
            }
            else if (state == ON) {
                // state is already ON, this was just a "coin-supply"
                // Do nothing but play the anim
            }
            sound_event ("coinsloton");
            set_anim("st-coin2slot");
        }
        else if (newIState == OFF) {
            state = OFF;
            performAction(state == ON);
            GameTimer.deactivate(this);
            sound_event ("coinslotoff");
            init_model();
        }
    }

    DEF_TRAITS(CoinSlot, "st_coinslot", st_coinslot);

    BOOT_REGISTER_START
        BootRegister(new CoinSlot(), "st_coinslot");
    BOOT_REGISTER_END

} // namespace enigma
