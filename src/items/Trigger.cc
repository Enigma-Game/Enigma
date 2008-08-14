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

#include "items/Trigger.hh"
#include "actors.hh"
//#include "errors.hh"
//#include "main.hh"
#include "server.hh"
#include "world.hh"

#include <vector>

namespace enigma {
    Trigger::Trigger() {
        setAttr("invisible", false);
    }
    
    Value Trigger::message (const Message &m) {
        if (m.message == "signal" && (server::GameCompatibility != GAMET_ENIGMA || 
                server::EnigmaCompatibility < 1.10)) {
            performAction(m.value.to_bool());  // convert 1/0 values to true/false
            return Value();
        } else if (m.message == "_init") {
            // the state count at init is wrong as some actors on the grid may
            // already have existed on_creation, but all are reported via
            // actor_enter due to actors on_creation.
            // Thus we need to reset and recount the actors:
            state = 0;
            
            // old Enigma versions did issue performAction what is incompatible
            updateIState(countActors(),
                    server::EnigmaCompatibility >= 1.10 || server::GameCompatibility != GAMET_ENIGMA); 
            return Value();
        } else if (m.message == "_jumping" ) {
            updateIState(m.value.to_bool() ? -1 : +1);
        }
        return Item::message(m);
    }
        
    int Trigger::externalState() const {
        return state != 0 ? 1 : 0;
    }
    
    void Trigger::setState(int extState) {
        return;   // ignore any write attempts
    }
    
    void Trigger::on_creation(GridPos p) {
        state = 0;
        updateIState(countActors(), true);
        init_model();
    }
        
    void Trigger::init_model() {
        if (getAttr("invisible").to_bool())
            set_model("invisible");
        else if (state != 0)
            set_model("it-trigger1");
        else
            set_model("it-trigger");
    }
    
    void Trigger::actor_enter(Actor *a) {
        if (!a->is_flying())
            updateIState(+1, !server::WorldInitialized);
    }
    
    void Trigger::actor_leave(Actor *a) {
        if (!a->is_flying())
            updateIState(-1, !server::WorldInitialized);
    }
    
    void Trigger::stone_change(Stone *) {
        updateIState(0);
    }
    
    int Trigger::countActors() {
        std::vector<Actor*> actors;
        GetActorsInsideField (get_pos(), actors);
        int count = 0;
        for (std::vector<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
            if (!(*it)->is_flying()) count++;
        return count;
    }
    
    void Trigger::updateIState(int diffActors, bool refuseAction) {
        int oldState = state;
        
        state += 2 * diffActors;
        
        Stone *st = GetStone(get_pos());
        state &= ~1;  // delete stone pressure bit
        if (st != NULL && (!st->is_floating() || st->is_kind("st-puzzle*"))) {
            // Hack to make hollow puzzle stones press triggers
            state |= 1;   // add stone pressure bit
        }
                
//        Log << "Trigger update old state " << oldState << " - new state " << state << " refuse action " << refuseAction << "\n";
    
        if ((oldState == 0 && state != 0) || (oldState != 0 && state == 0)) {
            init_model();
            if (!refuseAction) {
                if (state != 0) {
                    sound_event ("triggerdown");
                    performAction(true);
                } else {
                    sound_event ("triggerup");
                    performAction(false);
                }
            }
        }
    }

    DEF_ITEMTRAITSF(Trigger, "it_trigger", it_trigger, itf_static | itf_indestructible);

    BOOT_REGISTER_START
        BootRegister(new Trigger(), "it_trigger");
    BOOT_REGISTER_END

} // namespace enigma
