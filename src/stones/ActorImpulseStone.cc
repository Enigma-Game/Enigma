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

#include "stones/ActorImpulseStone.hh"
//#include "main.hh"
#include "player.hh"
#include "server.hh"
#include "items/GlassesItem.hh"

namespace enigma {
    ActorImpulseStone::ActorImpulseStone(bool isInvisible) : Stone () {
        if (isInvisible)
            objFlags |= OBJBIT_INVISIBLE;
            
        state = IDLE;
    }

    std::string ActorImpulseStone::getClass() const {
        return "st_actorimpulse";
    }
        
    void ActorImpulseStone::setAttr(const string& key, const Value &val) {
        if (key == "invisible") {
            if (val.to_bool() != ((objFlags & OBJBIT_INVISIBLE) != 0)) {
                // change of visibility
                objFlags ^= OBJBIT_INVISIBLE;
                if (isDisplayable())
                    init_model();
            }
        } else
            Stone::setAttr(key, val);
    }
    
    Value ActorImpulseStone::getAttr(const std::string &key) const {
        if (key == "invisible") {
            return (objFlags & OBJBIT_INVISIBLE) != 0;
        } else
            return Stone::getAttr(key);
    }
    
    Value ActorImpulseStone::message(const Message &m) {
        if (m.message == "_model_reanimated") {
            state = IDLE;   // reset any running anim, be ready to bump again on new grid
            init_model();   // visibility might have changed
        } else if (m.message == "_trigger") {
            // a boulder that we will send back
            Object *boulder = m.sender;
            if (boulder != NULL && m.value.to_bool()) {
                boulder->setAttr("orientation", reverse(to_direction(boulder->getAttr("orientation"))));
                sound_event("bumper");
                state = PULSING;
                set_anim("st_actorimpulse_anim");
            }
        } else if (m.message == "_glasses") {
            init_model();
            return Value();
        } else if (server::GameCompatibility != GAMET_ENIGMA) {
            // Oxyd* usage of ActorImpulseStone as a signal multiplier
            ObjectList ol = getAttr("$!oxyd!destinations");
            
            if (m.message == "_init" && ol.size() > 0) {
                setAttr("$signalidx", 0);
                SendMessage(ol.front(), "signal", 1);
                return Value();
            } else if (m.message == "signal") {
                int i = 0;
                int signalidx = getAttr("$signalidx");
                bool didBreak = false;
                for (ObjectList::iterator oit = ol.begin(); oit != ol.end(); ++oit, i++) {
                    if (i == signalidx) {
                        SendMessage(*oit, "signal", 0);
                    } else if (i == signalidx + 1) {
                        SendMessage(*oit, "signal", 1);
                        didBreak = true;
                        break;
                    }
                }
                if (!didBreak && ol.size() > 0) {
                    SendMessage(ol.front(), "signal", 1);
                    signalidx = 0;                        
                } else
                    signalidx++;
                
                setAttr("$signalidx", signalidx);
                return Value();
            }
        } // GameCompatibility != GAMET_ENIGMA
        return Stone::message(m);
    }
            
    void ActorImpulseStone::setState(int extState) {
        // ignore all state settings
    }

    void ActorImpulseStone::init_model() {
        if (state == IDLE) {
            if ((objFlags & OBJBIT_INVISIBLE) && ((server::GlassesVisibility & Glasses::ACTORIMPULSE) == 0))
                set_model("invisible");
            else
                set_model("st_actorimpulse");
        }
        // PULSING anim is always visible and should continue independent on init_model calls
    }
    
    void ActorImpulseStone::animcb() {
        if (state == PULSING) {
            state = IDLE;
            init_model();
        }
    }
    
    void ActorImpulseStone::actor_hit(const StoneContact &sc) {
        if ((objFlags & OBJBIT_INVISIBLE) && player::WieldedItemIs(sc.actor, "it_brush")) {
            objFlags &= ~OBJBIT_INVISIBLE;
        }
        if (state == IDLE) {
            // actor_hit is called before reflect, but the force added below
            // is applied to actor after the reflection.

            double forcefac = getDefaultedAttr("strength", server::BumperForce);

            V2 vec = normalize(sc.actor->get_pos() - get_pos().center());
            sc.actor->add_force(distortedVelocity(vec, forcefac));                

            sound_event("bumper");
            state = PULSING;
            set_anim("st_actorimpulse_anim");
        }
    }

    const char *ActorImpulseStone::collision_sound() {
        // no general sound - just a sound if really bumping
        return "";
    }

    int ActorImpulseStone::traitsIdx() const {
        return (objFlags & OBJBIT_INVISIBLE) ? 1 : 0;
    }

    
    StoneTraits ActorImpulseStone::traits[2] = {
        {"st_actorimpulse", st_actorimpulse, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_actorimpulse_invisible", st_actorimpulse_invisible, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
    };
    
    BOOT_REGISTER_START
        BootRegister(new ActorImpulseStone(false), "st_actorimpulse");
        BootRegister(new ActorImpulseStone(true), "st_actorimpulse_invisible");
    BOOT_REGISTER_END

} // namespace enigma
