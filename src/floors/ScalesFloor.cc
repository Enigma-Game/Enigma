/*
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

#include "floors/ScalesFloor.hh"

#include "errors.hh"
#include "main.hh"
#include "stones.hh"
#include "world.hh"

namespace enigma {
    
    ScalesFloor::ScalesFloor(std::string flavor) : Floor("fl_scales")  {
        Floor::setAttr("flavor", flavor);
        Floor::setAttr("$mass", 0.0);
        Floor::setAttr("burnable", flavor == "bridgewood");
        state = OFF;
    }
    
    std::string ScalesFloor::getClass() const {
        return "fl_scales";
    }
        
    void ScalesFloor::setAttr(const string& key, const Value &val) {
        if (key == "flavor") {
            std::string flavor = val.to_string();
            ASSERT(flavor == "brick" || flavor == "bridgewood" || flavor == "concrete" || flavor == "darkgray" || flavor == "gray" || flavor == "platinum", XLevelRuntime, "ScalesFloor illegal flavor value");
            Floor::setAttr("flavor", val);
            Floor::setAttr("burnable", flavor == "bridgewood");
            if (isDisplayable()) {
                init_model();    // need to redisplay after attribute set
            }
            return;
        }
        Floor::setAttr(key, val);
    }
    
    Value ScalesFloor::message(const Message &m) {
        if (m.message == "_init") {
            // the state  at init is wrong as some actors on the grid may
            // already have existed on_creation, but all are reported via
            // actor_enter due to actors on_creation.
            // Thus we need to reset and reweight the actors:
            if (Item *it = GetItem(get_pos())) {
                Value v = it->getAttr("mass");
                if (v.getType() == Value::DOUBLE)
                    Floor::setAttr("$mass", v);
            }
            updateIState(true);
            return Value();
        } else if (m.message == "_jumping" ) {
            updateIState();
        } else if (m.message == "_dying" ) {
            Actor *ac = dynamic_cast<Actor *>(m.sender);
            if (ac != NULL) {
                updateIState();
            }
        } else if (m.message == "_update_mass" ) {
            updateIState();
        } else if (m.message == "_add_mass" ) {
            if (server::WorldInitialized) {
                double mass = (double)getAttr("$mass") + (double)m.value;
                Floor::setAttr("$mass", mass);
                updateIState();
            }
        }
        return Floor::message(m);
    }

    void ScalesFloor::setState(int extState) {
        return;   // ignore any write attempts
    }
    
    std::string ScalesFloor::getModelName() const {
        return "fl_scales_" + getAttr("flavor").to_string() + ((state==ON) ? "_pressed" : "_released");
    }
    
    void ScalesFloor::on_creation(GridPos p) {
        state = OFF;
        updateIState(true);
        Floor::on_creation(p);
    }

    void ScalesFloor::actor_enter(Actor *a) {
        if (!(a->is_flying() || a->isMoribund()))
            updateIState(!server::WorldInitialized);
    }
    
    void ScalesFloor::actor_leave(Actor *a) {
        if (!(a->is_flying() || a->isMoribund()))
            updateIState(!server::WorldInitialized);
    }
    
    void ScalesFloor::stone_change(Stone *st) {
        updateIState();
    }
    
    bool ScalesFloor::weightActors() {
        double mass = getAttr("$mass");
        std::vector<Actor *> actors;
        GetActorsInsideField(get_pos(), actors);
        for (std::vector<Actor *>::iterator itr = actors.begin(); itr != actors.end(); ++itr) {
            if (!((*itr)->is_flying() || (*itr)->isMoribund())) {
                mass += (*itr)->get_actorinfo()->mass;
            }
        }
        return mass >= (double)getAttr("min");
    }
    
    void ScalesFloor::updateIState(bool refuseAction) {
        int oldState = state;
        state = OFF; 
        
        Stone *st = GetStone(get_pos());
        objFlags &= ~OBJBIT_STONE;
        if (st != NULL && (!st->is_floating() || st->getClass() == "st_puzzle")) {
            // Hack to make hollow puzzle stones press triggers
            objFlags |= OBJBIT_STONE;
            state = ON;
        }
        if (weightActors()) {
            state = ON;
        }

//Log << "Scales update old state " << oldState << " - new state " << state << " refuse action " << refuseAction << "\n";
    
        if (oldState != state) {
            init_model();
            if (!refuseAction) {
                if (state != OFF) {
                    sound_event ("triggerdown");
                    performAction(true);
                } else {
                    sound_event ("triggerup");
                    performAction(false);
                }
            }
        }
    }

    BOOT_REGISTER_START
        BootRegister(new ScalesFloor("darkgray"), "fl_scales");
        BootRegister(new ScalesFloor("brick"), "fl_scales_brick");
        BootRegister(new ScalesFloor("bridgewood"), "fl_scales_bridgewood");
        BootRegister(new ScalesFloor("concrete"), "fl_scales_concrete");
        BootRegister(new ScalesFloor("darkgray"), "fl_scales_darkgray");
        BootRegister(new ScalesFloor("gray"), "fl_scales_gray");
        BootRegister(new ScalesFloor("platinum"), "fl_scales_platinum");
    BOOT_REGISTER_END

} // namespace enigma
