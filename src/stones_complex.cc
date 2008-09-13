/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
 *
 * This program is free software; you can redistribute it and/ or
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

#include "errors.hh"
#include "laser.hh"
#include "SoundEffectManager.hh"
#include "server.hh"
#include "player.hh"
#include "Inventory.hh"
#include "stones_internal.hh"
#include "actors.hh"
#include "main.hh"
#include "Value.hh"

#include "ecl_util.hh"

#include <algorithm>
#include <iostream>

using namespace std;

namespace enigma { 

/* -------------------- Stone impulse stones -------------------- */

// Messages:
//
// :trigger
namespace
{
    class StoneImpulse_Base : public Stone {
    protected:
        StoneImpulse_Base() : state(IDLE), incoming(NODIR), nobackfire (false)
        {}

        enum State { IDLE, PULSING, CLOSING };
        State     state;
        Direction incoming; // direction of incoming impulse (may be NODIR)
        bool      nobackfire;

        void change_state(State st);

        virtual void on_impulse(const Impulse& impulse) {
            incoming = impulse.dir;
            change_state(PULSING);
        }

    private:

        virtual void notify_state(State st) = 0;

        virtual Value message(const Message &m) {
            if (m.message == "_trigger" && m.value.to_bool()) {
                incoming = NODIR;
                if (m.sender != NULL)
                    incoming = direction_fromto(dynamic_cast<GridObject *>(m.sender)->get_pos(), get_pos());
                
                if (state == IDLE && incoming != NODIR) {
                    nobackfire = true;
                }
                change_state(PULSING);
                return Value();
            }
            else if (m.message == "signal" && (to_double(m.value) != 0 || 
                    (server::EnigmaCompatibility < 1.10 /*&& m.value.getType() == Value::NIL*/))) { // hack for old trigger without value
                incoming = NODIR;
                change_state (PULSING);
                return Value();
            }
            return Stone::message(m);
        }

        void animcb() {
            if (state == PULSING)
                change_state (CLOSING);
            else if (state == CLOSING)
                change_state (IDLE);
        }

        void processLight(Direction dir) {
            incoming = dir;
            change_state(PULSING);
        }

        FreezeStatusBits get_freeze_bits() { return FREEZEBIT_NO_STONE; }
    };
}

void StoneImpulse_Base::change_state(State new_state) {
    if (new_state == state) return;

    GridPos p = get_pos();
    switch (new_state) {
        case IDLE: {
            state = new_state;
            notify_state(state);
            break;
        }
        case PULSING:
            if (state != IDLE) {
                return;         // do not set new state
            }
            state = new_state;
            notify_state(state);
            sound_event("impulse");
            break;
        case CLOSING: {
            GridPos targetpos[4];
            bool    haveStone[4];

            // set CLOSING model _before_ sending impulses !!!
            // (any impulse might have side effects that move this stone)

            state = new_state;
            notify_state(state);

            for (int d = 0; d < 4; ++d) {
                targetpos[d] = move(p, Direction(d));
                haveStone[d] = GetStone(targetpos[d]) != 0;
            }

            for (int d = int(incoming)+1; d <= int(incoming)+4; ++d) {
                int D = d%4;
                if (!nobackfire || reverse(incoming) != D) {
                    if (haveStone[D]) {
                        send_impulse(targetpos[D], Direction(D));
                    }
                }
            }

            incoming = NODIR;   // forget impulse direction
            nobackfire = false;
            break;
        }
    }
}


namespace
{
    class StoneImpulseStone : public StoneImpulse_Base {
        CLONEOBJ(StoneImpulseStone);
        DECL_TRAITS;
    public:
        StoneImpulseStone()
        {}

    private:
        void notify_state(State st) {
            switch (st) {
            case IDLE:
                init_model();
                break;
            case PULSING:
                set_anim("st-stoneimpulse-anim1");
                break;
            case CLOSING:
                set_anim("st-stoneimpulse-anim2");
                break;
            }
        }

        void actor_hit(const StoneContact &/*sc*/) {
            change_state(PULSING);
        }
    };
    DEF_TRAITS(StoneImpulseStone, "st-stoneimpulse", st_stoneimpulse);

    class HollowStoneImpulseStone : public StoneImpulse_Base {
        CLONEOBJ(HollowStoneImpulseStone);
        DECL_TRAITS;
    public:
        HollowStoneImpulseStone()
        {}
    private:
        void notify_state(State st) {
            switch (st) {
            case IDLE:
                init_model();
                MaybeRecalcLight(get_pos());
                break;
            case PULSING:
                MaybeRecalcLight(get_pos());
                set_anim("st-stoneimpulse-hollow-anim1");
                break;
            case CLOSING:
                set_anim("st-stoneimpulse-hollow-anim2");
                break;
            }
        }

        StoneResponse collision_response(const StoneContact &/*sc*/) {
            return (state == IDLE) ? STONE_PASS : STONE_REBOUND;
        }
        void actor_inside(Actor *a) {
            if (state == PULSING || state == CLOSING)
                SendMessage(a, "shatter");
        }

        bool is_floating () const {
            return true;
        }

        void processLight(Direction d) {
            // hollow StoneImpulseStones cannot be activated using lasers
        }
    };
    DEF_TRAITS(HollowStoneImpulseStone, "st-stoneimpulse-hollow",
               st_stoneimpulse_hollow);

    class MovableImpulseStone : public StoneImpulse_Base {
        CLONEOBJ(MovableImpulseStone);
        DECL_TRAITS;
    public:
        MovableImpulseStone()
        : repulse(false)
        {}

    private:

        void notify_state(State st) {
            switch (st) {
            case IDLE:
                if (repulse) {
                    repulse = false;
                    change_state(PULSING);
                }
                else
                    init_model();
                break;
            case PULSING:
                set_anim("st-stoneimpulse-anim1");
                break;
            case CLOSING:
                set_anim("st-stoneimpulse-anim2");
                break;
            }
        }

        void init_model() {
            set_model("st-stoneimpulse");
        }

        // Stone interface:

        void actor_hit(const StoneContact &sc) {
            if (!maybe_push_stone (sc)) {
                incoming = NODIR; // bad, but no real problem!
                if (state == IDLE)
                    change_state(PULSING);
            }
        }

        void on_impulse(const Impulse& impulse) {
            State oldstate = state;

            if (move_stone(impulse.dir)) {
                notify_state(oldstate); // restart anim if it was animated before move

                Actor *hitman = dynamic_cast<Actor*>(impulse.sender);
                if (hitman && player::WieldedItemIs (hitman, "it_magicwand")) {
                    return;     // do not change state to PULSING
                }
            }

            if (state == IDLE)
                change_state(PULSING);
        }

        void on_move() {
            if (state != PULSING)
                repulse = true; // pulse again
            Stone::on_move();
        }

        // Variables.
        bool repulse;
    };
    DEF_TRAITSM(MovableImpulseStone, "st-stoneimpulse_movable",
                st_stoneimpulse_movable, MOVABLE_STANDARD);
}

// --------------------------------------------------------------------------------

void Init_complex()
{
    Register(new HollowStoneImpulseStone);

    Register(new MovableImpulseStone);

    Register(new StoneImpulseStone);

}

} // namespace enigma
