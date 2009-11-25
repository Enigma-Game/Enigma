/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007,2008,2009 Ronald Lamprecht
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

#include "stones/KeySwitch.hh"
#include "server.hh"
#include "Inventory.hh"
#include "player.hh"

namespace enigma {
    KeySwitch::KeySwitch() : Stone () {
    }

    void KeySwitch::setAttr(const string& key, const Value &val) {
        if (key == "target") {
            Stone::setAttr("destination", val);
        } else if (key == "invisible") {
            if (isDisplayable() && (getAttr("invisible").to_bool() != val.to_bool())) {
                Stone::setAttr(key, val);
                init_model();
                return; 
            }
        } else if (key == "code") {
            Stone::setAttr(key, val);
            if (isDisplayable())
                init_model();
            return;            
        }
        Stone::setAttr(key, val);
    }
    
    void KeySwitch::setState(int extState) {
        if (isDisplayable()) {
            if (state == OFF && extState != state) {
                sound_event ("switchon");
                state = ON;
                init_model();
                performAction(state);
            } else if (state == ON && extState != state) {
                sound_event ("switchon");
                state = OFF;
                init_model();
                performAction(state);
            }
        } else
            state = extState;
    }

    void KeySwitch::init_model() {
        bool showCode = !getAttr("invisible").to_bool();
        int code = getAttr("code");
        if (showCode && code >= 1 && code <= 8)
            set_model(ecl::strf("st_key_%d_%s", code, state == ON ? "on" : "off"));
        else
            set_model(ecl::strf("st_key_%s", state == ON ? "on" : "off"));
    }

    void KeySwitch::actor_hit(const StoneContact &sc) {
        enigma::Inventory *inv = player::GetInventory(sc.actor);
        if (!inv)
            return;

        bool toggle = false;

        if (server::GameCompatibility == enigma::GAMET_ENIGMA) {
            const GridPos apos = sc.actor->get_gridpos();
            bool safe = true;
            ecl::V2 dest;
            int idx = 0;
            if (server::EnigmaCompatibility >= 1.10 && getAttr("secure").to_bool()) {
                while (getDestinationByIndex(idx++, dest)) {
                    if (apos == (GridPos)dest)
                        safe = false;
                }
            }
            if (safe) {
                if (state == ON) {
                    if (!inv->is_full()) {
                        Item *key = MakeItem("it_key");
                        key->setAttr("code", getAttr("code"));
                        key->setAttr("invisible", getAttr("invisible"));
                        inv->add_item(key);
                        toggle = true;
                    }
                } else if (check_matching_key(inv)) {
                    Item * it  = inv->yield_first();
                    if (!(it->getAttr("invisible").to_bool()))
                        setAttr("invisible", false);
                    DisposeObject(it);
                    toggle = true;
                }
                player::RedrawInventory (inv);
            }
        } else {
            if (check_matching_key(inv))
                toggle = true;
        }

        if (toggle)
            setState(ON - state);
    }

    const char *KeySwitch::collision_sound() {
         return "metal";
    }

    bool KeySwitch::check_matching_key(enigma::Inventory *inv) {
        Item *it = inv->get_item(0);
        return (it
             && it->isKind("it_key")
             && it->getAttr("code") == getAttr("code"));
    }

    DEF_TRAITS(KeySwitch, "st_key", st_key);

    BOOT_REGISTER_START
        BootRegister(new KeySwitch(), "st_key");
    BOOT_REGISTER_END

} // namespace enigma
