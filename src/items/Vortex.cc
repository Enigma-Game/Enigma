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

#include "items/Vortex.hh"
#include "actors.hh"
#include "client.hh"
#include "errors.hh"
#include "server.hh"
#include "world.hh"

namespace enigma {
    Vortex::Vortex(bool open) : Item() {
        state = open ? OPEN : CLOSED;
        setAttr("$dest_idx", 0);
        setAttr("$dest_vortex", (Object *)NULL);
        setAttr("$grabbed_actor", (Object *)NULL);
    }

    Vortex::~Vortex() {
        GameTimer.remove_alarm(this);
        if (Actor *actor = dynamic_cast<Actor *>((Object *)getAttr("$grabbed_actor"))) {
            // release an actor that is grabbed on behalf of this vortex - actor state FALLING_VORTEX
            SendMessage(actor, "_rise");
        }
    }

    std::string Vortex::getClass() const {
        return "it_vortex";
    }

    Value Vortex::message(const Message &m) {
        if (m.message == "_passed" && getAttr("autoclose").to_bool()) {
            setState(0);
            performAction(getAttr("$grabbed_actor"));
            return Value();
        }
        return Item::message(m);
    }

    int Vortex::externalState() const {
        return (state >= CLOSED && state <= CLOSING) ? 0 : 1 ;
    }

    void Vortex::setState(int extState) {
        if (isDisplayable()) {
            if (extState == 1) {  // open
                if (state == CLOSING) {
                    state = OPENING;
                    get_model()->reverse(); // reverse animation
                }
                else if (state == CLOSED) {
                    state = OPENING;
                    sound_event("vortexopen");
                    init_model();
                }
            } else if (extState == 0) { // close
                if (state == OPENING) {
                    state = CLOSING;
                    get_model()->reverse(); // reverse animation
                }
                else if (state == OPEN) {
                    state = CLOSING;
                    sound_event("vortexclose");
                    init_model();
                }
            }
        } else {
            if (extState <= 1 && extState >= 0)
                state = 1 - extState;
        }
    }

    void Vortex::toggleState() {
        if (state == OPEN || state == OPENING)
            setState(0);
        else
            setState(1);
    }

    void Vortex::init_model() {
        switch(state) {
            case WARPING:
            case OPEN:
            case EMITTING:
            case SWALLOWING:
                set_model("it_vortex_open");
                break;
            case CLOSED:
                set_model("it_vortex_closed"); break;
            case OPENING:
                set_anim("it_vortex_opening"); break;
            case CLOSING:
                set_anim("it_vortex_closing"); break;
        }
    }

    void Vortex::on_removal(GridPos p) {
        Item::on_removal(p);
        ASSERT(state != WARPING && state != SWALLOWING && state != EMITTING,
            XLevelRuntime, "Tried to kill a busy vortex. Please use another way.");
    }

    void Vortex::animcb() {
        if (state == CLOSING) {
            state = CLOSED;
            init_model();
        }
        else if (state == OPENING) {
            state = OPEN;
            init_model();
            BroadcastMessage("_checkflood", true, GRID_FLOOR_BIT);
        }
    }

    bool Vortex::covers_floor(ecl::V2 position, Actor *a) const {
        if (GridPos(position) != get_pos())
            return false;
        return (length(position - get_pos().center()) < 0.25);
    }

    bool Vortex::actor_hit(Actor *actor) {
        if (state == OPEN && (length((actor->get_pos()) - get_pos().center()) < 0.25) && actor->can_be_warped())
            prepare_for_warp(actor);
        else if (state == CLOSED && (length((actor->get_pos()) - get_pos().center()) > 0.25) &&
                getAttr("autoopen").to_bool())
            toggleState();
        return false;
    }

    std::list<GridPos> Vortex::warpSpreadPos(bool isWater) {
        std::list<GridPos> results;
        if (externalState() > 0) { // open -> may spread
            ecl::V2 targetpos;
            int idx = 0;
            while (getDestinationByIndex(idx++, targetpos)) {
                GridPos  targetgpos(targetpos);
                Vortex *v = dynamic_cast<Vortex*>(GetItem(targetgpos));
                if (v != NULL) {  // Destination is also a vortex
                    if (v->externalState() == 1)   // opened
                        results.push_back(targetgpos);
                } else {
                        results.push_back(targetgpos);
                }
            }
        }
        return results;
    }

    void Vortex::alarm() {
        if (state == WARPING) {
            perform_warp();
        } else if (state == EMITTING) {
            emit_actor(dynamic_cast<Vortex *>((Object *)getAttr("$dest_vortex")));
        } else if (state == SWALLOWING) {
            state = WARPING;
            sound_event("hitfloor");
            perform_warp();
        } else
            ASSERT (0, XLevelRuntime, "Vortex: alarm called with inconsistent state");
    }

    void Vortex::prepare_for_warp (Actor *actor) {
        SendMessage(actor, "_fallvortex");
        setAttr("$dest_idx", 0);
        setAttr("$grabbed_actor", actor);
        state = SWALLOWING;

        GameTimer.set_alarm(this, 0.4, false);
    }

    void Vortex::emit_actor(Vortex *destVortex) {
        if (destVortex == NULL)   // destination vortex got killed in meantime
            destVortex = this;    // reemit from source vortex
        ecl::V2 v(destVortex->get_pos().center());
        if (Actor *actor = dynamic_cast<Actor *>((Object *)getAttr("$grabbed_actor"))) {
            WarpActor(actor, v[0], v[1], false);
            SendMessage(actor, "_rise");
            if (destVortex != this) {
                bool isScissor = to_bool(getDefaultedAttr("scissor",
                        (server::EnigmaCompatibility >= 1.10) || server::GameCompatibility != GAMET_ENIGMA));
                if (isScissor)
                    SendMessage(actor, "disconnect");
            }
        }
        state = OPEN;
        if (this != destVortex && getAttr("autoclose").to_bool())  // do not close source vortex if destination is currently blocked
            setState(0);
        if (this != destVortex)
            performAction(getAttr("$grabbed_actor"));

        setAttr("$grabbed_actor", (Object *)NULL);
    }

    void Vortex::warp_to(const ecl::V2 &target) {
        client::Msg_Sparkle(target);
        if (Actor *actor = dynamic_cast<Actor *>((Object *)getAttr("$grabbed_actor"))) {
            WarpActor(actor, target[0], target[1], false);
            SendMessage(actor, "_appear");
            bool isScissor = to_bool(getDefaultedAttr("scissor",
                    (server::EnigmaCompatibility >= 1.10) || server::GameCompatibility != GAMET_ENIGMA));
            if (isScissor)
                SendMessage(actor, "disconnect");
        }
        state = OPEN;
        if (getAttr("autoclose").to_bool())
            setState(0);

        performAction(getAttr("$grabbed_actor"));
        setAttr("$grabbed_actor", (Object *)NULL);
    }

    void Vortex::perform_warp() {
        Actor *actor = dynamic_cast<Actor *>((Object *)getAttr("$grabbed_actor"));
        if (actor == NULL)
            return;

        ASSERT (state == WARPING, XLevelRuntime, "Vortex: perform_warp called with inconsistent state");

        ecl::V2 v_target;

        // is another target position defined?
        int dest_idx = getAttr("$dest_idx");
        if (Object::getDestinationByIndex(dest_idx, v_target)) {
            GridPos  p_target(v_target);

            Vortex *v = dynamic_cast<Vortex*>(GetItem(p_target));

            if (v) {                // Destination is also a vortex
                Stone *st = GetStone(p_target);

                if (st && st->is_sticky(actor)) {
                    // is destination vortex blocked? redirect
                    setAttr("$dest_idx", dest_idx + 1);
                    client::Msg_Sparkle(p_target.center());
                    WarpActor(actor, v_target[0], v_target[1], false);
                    GameTimer.set_alarm(this, 0.8, false);
                }
                else {
                    switch (v->state) {
                        case OPEN:
                        case OPENING:
                            // destination is open
                            emit_actor(v);
                            break;

                        case CLOSED:
                        case CLOSING:
                            // destination is closed
                            SendMessage(v, "open");
                            setAttr("$dest_vortex", v);
                            state = EMITTING;
                            GameTimer.set_alarm(this, 0.4, false);
                            break;
                        case SWALLOWING:
                        case WARPING:
                        case EMITTING:
                            // destination is busy -> don't warp actor, emit
                            // it where it has started
                            emit_actor(this);
                    }
                }
            } else {
                warp_to(v_target);
            }
        }
        else {
            // if no target defined, don't warp actor
            emit_actor(this);
        }
    }

    ItemTraits Vortex::traits[2] = {
        {"it_vortex_closed", it_vortex_closed, itf_static | itf_fireproof | itf_indestructible, 0.0},
        {"it_vortex_open", it_vortex_open,     itf_static | itf_fireproof | itf_indestructible, 0.0}
    };

    BOOT_REGISTER_START
        BootRegister(new Vortex(true), "it_vortex");
        BootRegister(new Vortex(false), "it_vortex_closed");
        BootRegister(new Vortex(true), "it_vortex_open");
    BOOT_REGISTER_END

} // namespace enigma
