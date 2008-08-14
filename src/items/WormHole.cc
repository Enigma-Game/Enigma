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

#include "items/WormHole.hh"
#include "actors.hh"
//#include "errors.hh"
//#include "main.hh"
//#include "server.hh"
#include "world.hh"

namespace enigma {
    WormHole::WormHole(bool isOn) : Item(), correctedStrength (0.6 * 30), squareRange (1000 * 1000) {
        state = isOn ? ON_IDLE : OFF_IDLE;    // includes warping false
    }
    
    std::string WormHole::getClass() const {
        return "it_wormhole";
    }
    
    void WormHole::setAttr(const std::string &key, const Value &val) {
        if (key == "range") {
            double range = (val.getType() == Value::NIL) ? server::WormholeRange : (double)val;
            squareRange = range * range;
        } else if (key == "strength") {
            correctedStrength = 0.6 * ((val.getType() == Value::NIL) ? server::WormholeForce : (double)val);
        }
        Item::setAttr(key, val);
    }
    
    Value WormHole::message(const Message &m) {
        if (m.message == "_updateglobals" && m.value.to_string() == "it_wormhole") {
            if (getAttr("range").getType() == Value::DEFAULT) {
                squareRange = server::WormholeRange * server::WormholeRange;
            }
            if (getAttr("strength").getType() == Value::DEFAULT) {
                correctedStrength = 0.6 * server::WormholeForce;
            }
            return Value(); 
        }
        return Item::message(m);
    }

    int WormHole::externalState() const {
        return state % 2;
    }
    
    void WormHole::setState(int extState) {
        // switch force on and off
        if (extState != state % 2) {          // react only on force changes
            state = (state & ~1) + extState;  // keep other flags
            if (isDisplayable()) {
                if (extState == 1)
                    AddForceField(this);
                else
                    RemoveForceField(this);
            }
        }
    }
    
    void WormHole::on_creation (GridPos p) {
        Item::on_creation (p);
        if (getAttr("range").getType() == Value::DEFAULT) {
            squareRange = server::WormholeRange * server::WormholeRange;
        }
        if (getAttr("strength").getType() == Value::DEFAULT) {
            correctedStrength = 0.6 * server::WormholeForce;
        }
        if (state % 2 == 1)
            AddForceField(this);
    }

    void WormHole::on_removal(GridPos p) {
        GameTimer.remove_alarm(this);
        state &= ~2;  // remove teleport engaged flag
        if (state % 2 == 1)
            RemoveForceField(this);
        Item::on_removal(p);
        ASSERT((state & 4) == 0, XLevelRuntime, "Tried to kill a busy wormhole. Please use another way.");
    }
    
    void WormHole::init_model() {
        if ((state & 3) == ON_IDLE)
            set_anim("it-wormhole");
        else
            set_model("it-wormhole-off");
    }
    
    bool WormHole::actor_hit(Actor *actor) {
        ASSERT((state & 4) == 0, XLevelRuntime, "WormHole:: Recursion detected!");
        if ((state & 2) == 0 && near_center_p(actor)) {   // may teleport
            client::Msg_Sparkle (get_pos().center());
            V2 targetpos;
            if (getDestinationByIndex(0, targetpos)) {
                sound_event ("warp");
                double latency = getAttr("interval");
                if(latency > 0) {
                    state |= 2;  // mark engaged
                    GameTimer.set_alarm(this, latency, false);
                    init_model();
                }
                state |= 4;  // mark warping
                bool isScissor = to_bool(getAttr("scissor")) || server::GameCompatibility != GAMET_ENIGMA;
                if (isScissor)
                    SendMessage(actor, "disconnect");
                WarpActor(actor, targetpos[0], targetpos[1], false);
                state &= ~4; // release warping
            }
        }
        return false;
    }
    
    void WormHole::add_force(Actor *a, V2 &f) {
        V2 dv = get_pos().center() - a->get_pos_force();
        double squareDist = square(dv);

        if (squareDist >= 0.025 && squareDist < squareRange)
            f += (correctedStrength / squareDist) * dv;
    }
    
    void WormHole::alarm() {
        state &= ~2;  // remove teleport engaged flag
        init_model();
    }
    
    bool WormHole::near_center_p(Actor *a) {
        return (square(a->get_pos() - get_pos().center()) < 0.015625);  // 0.125 ^ 2
    }
    
    ItemTraits WormHole::traits[2] = {
        { "it_wormhole_off", it_wormhole_off, itf_static, 0.0 },
        { "it_wormhole_on",  it_wormhole_on,  itf_static, 0.0 }
    };

    BOOT_REGISTER_START
        BootRegister(new WormHole(true), "it_wormhole");
        BootRegister(new WormHole(false), "it_wormhole_off");
        BootRegister(new WormHole(true), "it_wormhole_on");
    BOOT_REGISTER_END

} // namespace enigma
