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
#include "errors.hh"
#include "Inventory.hh"
#include "main.hh"
#include "player.hh"
#include "server.hh"

namespace enigma {
    CoinSlot::CoinSlot(bool isInstant) : Stone() {
        if (isInstant)
            objFlags |= OBJBIT_INSTANT;
        state = OFF;
        setAttr("$addTime", 0);
    }

    CoinSlot::~CoinSlot() {
        GameTimer.remove_alarm (this);
    }

    std::string CoinSlot::getClass() const {
        return "st_coinslot";
    }

    Value CoinSlot::getAttr(const std::string &key) const {
        if (key == "instant") {
            return (bool)(objFlags & OBJBIT_INSTANT != 0);
        }
        return Stone::getAttr(key);
    }
    
    void CoinSlot::setAttr(const string& key, const Value &val) {
        if (key == "instant") {
            if (val.to_bool())
                objFlags |= OBJBIT_INSTANT;
            else
                objFlags &= ~OBJBIT_INSTANT;
            return;
        }
        Stone::setAttr(key, val);
    }

    Value CoinSlot::message(const Message &m) {
        if (m.message == "_model_reanimated") {
            // we are swapped - may be that an alarm switched off the slot meanwhile
            if (state <= ON)
                init_model();  // replace potential bogus model
        }
        return Stone::message(m);
    }

    int CoinSlot::externalState() const {
        return state % 2; 
    }

    void CoinSlot::setState(int extState) {
        return;   // ignore any write attempts
    }

    void CoinSlot::init_model() {
        // just static models
        if (state <= ON)
            set_model(state==ON ? "st_coinslot_active" : "st_coinslot");
    }

    void CoinSlot::animcb() {
        ASSERT(state >= INSERT_OFF, XLevelRuntime, "Coinslot - animcb on wrong state");
        double rest = GameTimer.remove_alarm(this);   // stop alarm
        iState newIState = ON;                        // standard follow up state
        if (!(objFlags & OBJBIT_INSTANT)) {
            // start or update timer
            rest += (double)getAttr("$addTime");
            setAttr("$addTime", 0);
        } else { // instant
            if (rest == 0)
                // the coin value is less than the animation time 
                newIState = OFF;
        }
        if (rest > 0)
            GameTimer.set_alarm(this, (double)getAttr("$addTime") + rest, false);
        setIState(newIState);
    }

    void CoinSlot::actor_hit(const StoneContact &sc) {
        // no coin insert if instant and coin is currently being inserted 
        if (state >= INSERT_OFF && !(objFlags & OBJBIT_INSTANT))
            return;
            
        if (enigma::Inventory *inv = player::GetInventory(sc.actor)) {
            if (Item *it = inv->get_item(0)) {
                ItemID id = get_id(it);
                if (id == it_coin_s || id == it_coin_m || id == it_coin_l) {
                    double interval;
                    if (server::EnigmaCompatibility < 1.10) 
                        interval = it->getAttr("coin_value");
                    else {
                        switch (id) {
                            case it_coin_s:
                                interval = getAttr("interval_s"); break;
                            case it_coin_m:
                                interval = getAttr("interval_m"); break;
                            case it_coin_l:
                                interval = getAttr("interval_l"); break;
                        }
                        if (interval == -2)  // reject coin
                            return;
                    }
                    if (interval > 0) {
                        if (objFlags & OBJBIT_INSTANT) {
                            // start or update timer
                            double rest = GameTimer.remove_alarm(this);
                            GameTimer.set_alarm(this, interval + rest, false);
                        } else {
                            // remember time value for animcb evaluation
                            setAttr("$addTime", (double)getAttr("$addTime") + interval);
                        }
                    }
                    inv->yield_first();
                    player::RedrawInventory(inv);
                    delete it;
                    
                    if (interval > 0)
                        setIState((objFlags & OBJBIT_INSTANT) ? INSERT_ON : (iState)(state + 2));
                }
            }
        }
    }

    const char *CoinSlot::collision_sound() {
         return "metal";
    }

    void CoinSlot::alarm() {
        ASSERT((state & 1) == ON, XLevelRuntime, "Coinslot - alarm of not active slot");
        setIState(state == ON ? OFF : INSERT_OFF);
    }
    
    void CoinSlot::setIState(iState newIState) {
        bool doAction = false;
        bool actionValue = false;
        bool doInit = false;
        
//        Log << "Coinslot state change from " << state << " to " << newIState << "\n";
        switch (state) {
            case OFF :
                ASSERT(newIState != ON, XLevelRuntime, "Coinslot - illegal state change from OFF to ON");
                actionValue = true;
                doAction = (newIState == INSERT_ON);
                set_anim("st_coinslot_insert");
                break;
            case ON :
                if (newIState == INSERT_ON)
                    set_anim("st_coinslot_insert");
                else if (newIState == OFF) {
                    actionValue = false;
                    doAction = true;
                    doInit = true;
                } else
                    ASSERT(false , XLevelRuntime, "Coinslot - illegal state change from ");
                break;
            case INSERT_OFF:
                ASSERT(newIState == ON, XLevelRuntime, "Coinslot - illegal state change from ");
                doInit = true;
                actionValue = true;
                doAction = true;
                break;
            case INSERT_ON :
                switch (newIState) {
                    case INSERT_ON :
                        set_anim("st_coinslot_insert");  // may restart anim!
                        break;
                    case ON :
                        doInit = true;
                        break;
                    case OFF :
                        doInit = true;
                        // fall through
                    case INSERT_OFF :  // continue insert animation
                        actionValue = false;
                        doAction = true;
                        break;
                }
                break;
        }
        
        state = newIState;

        if (doInit && isDisplayable())
            init_model();
        
        if (doAction)
           performAction(actionValue);
                
    }

    DEF_TRAITS(CoinSlot, "st_coinslot", st_coinslot);

    BOOT_REGISTER_START
        BootRegister(new CoinSlot(false), "st_coinslot");
        BootRegister(new CoinSlot(true), "st_coinslot_instant");
    BOOT_REGISTER_END

} // namespace enigma
