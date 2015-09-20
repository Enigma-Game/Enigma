/*
 * Copyright (C) 2008,2009 Ronald Lamprecht
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

#include "stones/StoneImpulse.hh"
#include "main.hh"
#include "player.hh"
#include "server.hh"
#include "world.hh"

namespace enigma {
    StoneImpulse::StoneImpulse(bool isSteadyStone, bool isHollowStone, bool isMovableStone, bool isActiveStone) : Stone () {
        if (isSteadyStone)
            objFlags |= OBJBIT_STEADY;
        if (isHollowStone)
            objFlags |= OBJBIT_HOLLOW;
        if (isMovableStone)
            objFlags |= OBJBIT_MOVABLE;
        if (isActiveStone)
            state = EXPANDING;
    }

    std::string StoneImpulse::getClass() const {
        return "st_stoneimpulse";
    }

void StoneImpulse::setAttr(const std::string& key, const Value &val) {
        if (key == "hollow") {
            if (!isDisplayable()) {
                if (val.to_bool()) {
                    objFlags |= OBJBIT_HOLLOW;
                    objFlags &= ~OBJBIT_MOVABLE;
                    objFlags &= ~OBJBIT_STEADY;
                } else
                    objFlags &= ~OBJBIT_HOLLOW;
            }
        } else if (key == "movable") {
            if (!isDisplayable()) {
                if (val.to_bool()) {
                    objFlags |= OBJBIT_MOVABLE;
                    objFlags &= ~OBJBIT_HOLLOW;
                } else
                    objFlags &= ~OBJBIT_MOVABLE;
            }
        } else if (key == "steady") {
            if (!isDisplayable()) {
                if (val.to_bool()) {
                    objFlags |= OBJBIT_STEADY;
                    objFlags &= ~OBJBIT_HOLLOW;
                } else
                    objFlags &= ~OBJBIT_STEADY;
            }
        } else if (key == "orientation") {
            if (!isDisplayable()) {
                Stone::setAttr("$incoming", reverse(to_direction(val)));
                objFlags |= OBJBIT_NOBACKFIRE;
            }
        } else
            Stone::setAttr(key, val);
    }

    Value StoneImpulse::getAttr(const std::string &key) const {
        if (key == "hollow") {
            return isHollow();
        } else if (key == "movable") {
            return (bool)(objFlags & OBJBIT_MOVABLE);
        } else if (key == "steady") {
            return (bool)(objFlags & OBJBIT_STEADY);
        } else
            return Stone::getAttr(key);
    }

    Value StoneImpulse::message(const Message &m) {
        if (m.message == "_trigger" && m.value.to_bool()) {
            Direction incoming = NODIR;
            if (m.sender != NULL)
                incoming = direction_fromto(dynamic_cast<GridObject *>(m.sender)->get_pos(), get_pos());

            if (state == IDLE && incoming != NODIR) {
                objFlags |= OBJBIT_NOBACKFIRE;
            }
            setIState(EXPANDING, incoming);
            return Value();
        } else if (m.message == "signal" && (to_double(m.value) != 0 ||
                (server::EnigmaCompatibility < 1.10 /*&& m.value.getType() == Value::NIL*/))) { // hack for old trigger without value
            setIState(EXPANDING);
            return Value();
        } else if (m.message == "_model_reanimated" && state != BREAKING) {
            // we are swapped in or out of a laser beam
            updateCurrentLightDirs();
            if ((objFlags & OBJBIT_STEADY) && (objFlags & OBJBIT_LIGHTNEWDIRS))
                init_model();     // replace potential bogus model
            else if (!(objFlags & OBJBIT_STEADY) && (objFlags & OBJBIT_LIGHTNEWDIRS) && !(objFlags & OBJBIT_LASERIDLE))
                setIState(EXPANDING);
            return Value();
        } else if (m.message == "_init") {
            // we may have the wrong model
            updateCurrentLightDirs();
            if (state == IDLE && (objFlags & OBJBIT_STEADY) && (objFlags & OBJBIT_LIGHTNEWDIRS))
               setIState(EXPANDING);     // replace potential bogus model
            return Value();
        } else if (m.message == "ignite") {
            updateCurrentLightDirs();
            if ((server::GameCompatibility != GAMET_ENIGMA && state == IDLE) || (objFlags & OBJBIT_LIGHTNEWDIRS)) {
                state = BREAKING;
                init_model();
            }
            return Value();
        } else if (m.message == "_cannonball") {
            if ((server::GameCompatibility != GAMET_ENIGMA && state == IDLE)) {
                state = BREAKING;
                init_model();
            } else {
                // Enigma mode do pulse
                setIState(EXPANDING);
            }
            return Value();
        } else
            return Stone::message(m);
    }


    void StoneImpulse::setState(int extState) {
        // block any write attempts
    }

    void StoneImpulse::init_model() {
        if (state == IDLE)
            set_model(ecl::strf("st_stoneimpulse%s", isHollow() ? "_hollow" : ((objFlags & OBJBIT_STEADY) ? "_steady" : "")));
        else if (state == BREAKING)
            set_anim("st_stoneimpulse_breaking");
        else
            set_anim(ecl::strf("st_stoneimpulse%s_anim%d", isHollow() ? "_hollow" : "",
                    state == EXPANDING ? 1 : 2));
    }

    void StoneImpulse::on_creation(GridPos p) {
        updateCurrentLightDirs();

        if (!isHollow())
            activatePhoto();

        if (state == IDLE && (objFlags & OBJBIT_STEADY) && (objFlags & OBJBIT_LIGHTNEWDIRS))
            setIState(EXPANDING);
        else if (!(objFlags & OBJBIT_STEADY) && !(objFlags & OBJBIT_LIGHTNEWDIRS))  // being swapped out of light
            objFlags &= ~OBJBIT_LASERIDLE;

        Stone::on_creation(p);
    }

    void StoneImpulse::lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs) {
        if (added_dirs(oldDirs, newDirs) != NODIRBIT) {
            setIState(EXPANDING);
        }
    }

    void StoneImpulse::animcb() {
        if (state == BREAKING)
            KillStone(get_pos());
        else if (state == EXPANDING)
            setIState(SHRINKING);
        else {
            setIState(IDLE);
            if ((objFlags & OBJBIT_STEADY) && (objFlags & OBJBIT_LIGHTNEWDIRS))
                setIState(EXPANDING);
            else if (!(objFlags & OBJBIT_STEADY) && (objFlags & OBJBIT_LIGHTNEWDIRS))
                objFlags |= OBJBIT_LASERIDLE;
        }
    }

    bool StoneImpulse::is_floating() const {
        return isHollow();
    }

    StoneResponse StoneImpulse::collision_response(const StoneContact &sc) {
        return ((state != IDLE) || !isHollow()) ? STONE_REBOUND : STONE_PASS;
    }

    void StoneImpulse::actor_inside(Actor *a) {
        if ((state != IDLE) && isHollow())
            SendMessage(a, "_shatter");
    }

    void StoneImpulse::actor_hit(const StoneContact &sc) {
        if (state != BREAKING) {
            if ((objFlags & OBJBIT_MOVABLE) && maybe_push_stone(sc))
                return;                                      // stone did move on impulse
            else if (!isHollow())
                sc.actor->send_impulse(sc.stonepos, NODIR);  // impulse on the slightest touch
        }
    }

    void StoneImpulse::on_impulse(const Impulse& impulse) {
        if (state == BREAKING)
            return;

        Actor *hitman = NULL;
        if ((objFlags & OBJBIT_MOVABLE) && (impulse.dir != NODIR)) {
            // move stone without disturbing a running animation
            display::Model *yieldedModel = display::YieldModel(GridLoc(GRID_STONES, get_pos()));
            int oldState = state;
            bool didMove = move_stone(impulse.dir);
            state = oldState;
            display::SetModel(GridLoc(GRID_STONES, get_pos()), yieldedModel);

            // pulse only if not pushed with a wand
            hitman = dynamic_cast<Actor*>(impulse.sender);
            if (hitman == NULL || !player::WieldedItemIs(hitman, "it_magicwand")) {
                if (state == IDLE)
                    setIState(EXPANDING, impulse.dir);
                else if (didMove && state != EXPANDING) {
                    // ensure that an impulse to neighbors will be emitted when moved
                    Stone::setAttr("$incoming", impulse.dir);
                    objFlags |= OBJBIT_REPULSE;
                }
            } else if (((objFlags & OBJBIT_STEADY) && (objFlags & OBJBIT_LIGHTNEWDIRS) && state == IDLE)) {
                setIState(EXPANDING, impulse.dir);
            }
        } else {
            setIState(EXPANDING, impulse.dir);
        }

        // direct impulse propagation
        if (objFlags & OBJBIT_MOVABLE && (impulse.dir != NODIR)) {
            if (hitman != NULL) {
                objFlags &= ~OBJBIT_PROPAGATE;
                propagateImpulse(impulse);
            } else
                objFlags |= OBJBIT_PROPAGATE;
        }
        setAttr("$impulse_source", impulse.sender->getId());
    }

    FreezeStatusBits StoneImpulse::get_freeze_bits() {
        return FREEZEBIT_NO_STONE;
    }

    void StoneImpulse::setIState(int newState, Direction incoming) {
        if (newState != state && isDisplayable() && state != BREAKING) {
            switch (newState) {
                case IDLE:
                    state = newState;
                    if (objFlags & OBJBIT_REPULSE) {
                        objFlags &= ~OBJBIT_REPULSE;
                        setIState(EXPANDING, to_direction(getAttr("$incoming")));
                    } else
                        init_model();
                    break;
                case EXPANDING:
                    if (state != IDLE) {
                        return;         // do not set new state
                    }
                    objFlags &= ~OBJBIT_LASERIDLE;
                    Stone::setAttr("$incoming", incoming);
                    state = newState;
                    init_model();
                    sound_event("impulse");
                    break;
                case SHRINKING:
                    if (state != EXPANDING) {
                        return;         // do not set new state
                    }
                    state = newState;
                    init_model();
                    GridPos p = get_pos();
                    Direction origin = reverse(to_direction(getAttr("$incoming")));
                    for (Direction d = WEST; d != NODIR; d = next(d)) {
                        if (!(objFlags & OBJBIT_NOBACKFIRE) || d != origin)
                            send_impulse(move(p, d), d);
                    }
                    if (!(objFlags & OBJBIT_MOVABLE) || (objFlags & OBJBIT_PROPAGATE)) {
                        propagateImpulse(Impulse(this, GridPos(-1,-1),
                                to_direction(getAttr("$incoming"))));
                    }
                    objFlags &= ~OBJBIT_PROPAGATE;
                    break;
            }
        }
    }

    void StoneImpulse::propagateImpulse(const Impulse& impulse) {
        if (!impulse.byWire && impulse.dir != NODIR) {
            ObjectList olist = getAttr("fellows");
            int sourceId = getDefaultedAttr("$impulse_source", 0);
            for (ObjectList::iterator it = olist.begin(); it != olist.end(); ++it) {
                Stone *fellow = dynamic_cast<Stone *>(*it);
                if (fellow != NULL && fellow->getId() != sourceId) {
                    Impulse wireImpulse(this, fellow->get_pos(), impulse.dir, true);
                    fellow->on_impulse(wireImpulse);
                }
            }
        }
        setAttr("$impulse_source", 0);
    }

    bool StoneImpulse::isHollow() const {
        return (bool)(objFlags & OBJBIT_HOLLOW) && !(bool)(objFlags & OBJBIT_MOVABLE);
    }

    int StoneImpulse::traitsIdx() const {
        if (objFlags & OBJBIT_MOVABLE)
            return 3;
        else if (objFlags & OBJBIT_HOLLOW)
            return 2;
        else if (objFlags & OBJBIT_STEADY)
            return 1;
        else
            return 0;
    }


    StoneTraits StoneImpulse::traits[4] = {
        {"st_stoneimpulse", st_stoneimpulse, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_stoneimpulse_steady", st_stoneimpulse_steady, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_stoneimpulse_hollow", st_stoneimpulse_hollow, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_stoneimpulse_movable", st_stoneimpulse_movable, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
    };

    BOOT_REGISTER_START
        BootRegister(new StoneImpulse(false, false, false, false), "st_stoneimpulse");
        BootRegister(new StoneImpulse(true,  false, false, false), "st_stoneimpulse_steady");
        BootRegister(new StoneImpulse(false, true,  false, false), "st_stoneimpulse_hollow");
        BootRegister(new StoneImpulse(false, false, true,  false), "st_stoneimpulse_movable");
        BootRegister(new StoneImpulse(false, false, false, true),  "st_stoneimpulse_new");
    BOOT_REGISTER_END

} // namespace enigma
