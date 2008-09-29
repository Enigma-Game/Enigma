/*
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

#include "stones/StoneImpulse.hh"
#include "main.hh"
#include "player.hh"

namespace enigma {
    StoneImpulse::StoneImpulse(bool isSteady, bool isHollow, bool isMovable, bool isActive) : Stone () {
        if (isSteady)
            objFlags |= OBJBIT_STEADY;
        if (isHollow)
            objFlags |= OBJBIT_HOLLOW;
        if (isMovable)
            objFlags |= OBJBIT_MOVABLE;
        if (isActive)
            state = EXPANDING;
    }

    std::string StoneImpulse::getClass() const {
        return "st_stoneimpulse";
    }
        
    void StoneImpulse::setAttr(const string& key, const Value &val) {
        if (key == "hollow") {
            if (val.to_bool()) {
                objFlags |= OBJBIT_HOLLOW;
                objFlags &= ~OBJBIT_MOVABLE;
                objFlags &= ~OBJBIT_STEADY;
            } else
                objFlags &= ~OBJBIT_HOLLOW;
        } else if (key == "movable") {
            if (val.to_bool()) {
                objFlags |= OBJBIT_MOVABLE;
                objFlags &= ~OBJBIT_HOLLOW;
                objFlags &= ~OBJBIT_STEADY;
            } else
                objFlags &= ~OBJBIT_MOVABLE;
        } else if (key == "steady") {
            if (val.to_bool()) {
                objFlags |= OBJBIT_STEADY;
                objFlags &= ~OBJBIT_HOLLOW;
                objFlags &= ~OBJBIT_MOVABLE;
            } else
                objFlags &= ~OBJBIT_STEADY;
        } else if (key == "orientation") {
            if (!isDisplayable()) {
                objFlags &= ~OBJBIT_INCOMINGDIR;
                Direction incoming = to_direction(val); 
                if (incoming != NODIR ) {
                    objFlags |= (incoming << 29) | OBJBIT_NOBACKFIRE;
                }
            }
        } else
            Stone::setAttr(key, val);
    }
    
    Value StoneImpulse::getAttr(const std::string &key) const {
        if (key == "hollow") {
            return (bool)(objFlags & OBJBIT_HOLLOW);
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
        } else if (m.message == "_model_reanimated") {
            // we are swapped in or out of a laser beam
            if ((objFlags & OBJBIT_STEADY) && (objFlags & OBJBIT_LIGHTNEWDIRS))
                init_model();     // replace potential bogus model
            else if (!(objFlags & OBJBIT_STEADY) && (objFlags & OBJBIT_LIGHTNEWDIRS) && !(objFlags & OBJBIT_LASERIDLE))
                setIState(EXPANDING);
        } else if (m.message == "_init") {
            // we may have the wrong model
            updateCurrentLightDirs();
            if (state == IDLE && (objFlags & OBJBIT_STEADY) && (objFlags & OBJBIT_LIGHTNEWDIRS))
               setIState(EXPANDING);     // replace potential bogus model
        } else
            return Stone::message(m);
    }
    
            
    void StoneImpulse::setState(int extState) {
        // block any write attempts
    }

    void StoneImpulse::init_model() {
        if (state == IDLE)
            set_model(ecl::strf("st-stoneimpulse%s", (objFlags & OBJBIT_HOLLOW) ? "-hollow" : ""));
        else
            set_anim(ecl::strf("st-stoneimpulse%s-anim%d", (objFlags & OBJBIT_HOLLOW) ? "-hollow" : "",
                    state == EXPANDING ? 1 : 2));
    }
    
    void StoneImpulse::on_creation(GridPos p) {
        updateCurrentLightDirs();
            
        if (!(objFlags & OBJBIT_HOLLOW))
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
        if (state == EXPANDING)
            setIState(SHRINKING);
        else {
            setIState(IDLE);
            if ((objFlags & OBJBIT_STEADY) && (objFlags & OBJBIT_LIGHTNEWDIRS))
                setIState(EXPANDING);
            else if (!(objFlags & OBJBIT_STEADY) && (objFlags & OBJBIT_LIGHTNEWDIRS))
                objFlags |= OBJBIT_LASERIDLE;
        }
        init_model();
    }
    
    bool StoneImpulse::is_floating() const {
        return objFlags & OBJBIT_HOLLOW;
    }
    
    StoneResponse StoneImpulse::collision_response(const StoneContact &sc) {
        return ((state != IDLE) || !(objFlags & OBJBIT_HOLLOW)) ? STONE_REBOUND : STONE_PASS;
    }
    
    void StoneImpulse::actor_inside(Actor *a) {
        if ((state != IDLE) && (objFlags & OBJBIT_HOLLOW))
            SendMessage(a, "shatter");
    }
    
    void StoneImpulse::actor_hit(const StoneContact &sc) {
        if ((objFlags & OBJBIT_MOVABLE) && maybe_push_stone(sc))
            return;                                      // stone did move on impulse
        else if (!(objFlags & OBJBIT_HOLLOW)) 
            sc.actor->send_impulse(sc.stonepos, NODIR);  // impulse on the slightest touch
    }

    void StoneImpulse::on_impulse(const Impulse& impulse) {
        if ((objFlags & OBJBIT_MOVABLE) && (impulse.dir != NODIR)) {
            // move stone without disturbing a running animation
            display::Model *yieldedModel = display::YieldModel(GridLoc(GRID_STONES, get_pos()));
            move_stone(impulse.dir);
            display::SetModel(GridLoc(GRID_STONES, get_pos()), yieldedModel);
            
            // pulse only if not pushed with a wand
            Actor *hitman = dynamic_cast<Actor*>(impulse.sender);
            if (hitman == NULL || !player::WieldedItemIs (hitman, "it_magicwand")) {
                if (state != IDLE)
                    objFlags |= OBJBIT_REPULSE;
                else
                    setIState(EXPANDING, NODIR);
            }
        } else {
            setIState(EXPANDING, impulse.dir);
        }
        
        propagateImpulse(impulse);
    }
    
    FreezeStatusBits StoneImpulse::get_freeze_bits() {
        return FREEZEBIT_NO_STONE;
    }
    
    void StoneImpulse::setIState(int newState, Direction incoming) {
        if (newState != state && isDisplayable()) {
            switch (newState) {
                case IDLE:
                    state = newState;
                    if (objFlags & OBJBIT_REPULSE) {
                        objFlags &= ~OBJBIT_REPULSE;
                        setIState(EXPANDING, NODIR);
                    } else
                        init_model();
                    break;
                case EXPANDING:
                    if (state != IDLE) {
                        return;         // do not set new state
                    }
                    objFlags &= ~OBJBIT_LASERIDLE;
                    objFlags &= ~OBJBIT_INCOMINGDIR;
                    if (incoming != NODIR)
                        objFlags |= (incoming << 29);
                    state = newState;
                    init_model();
                    sound_event("impulse");
                    break;
                case SHRINKING:
                    state = newState;
                    init_model();
                    GridPos p = get_pos();
                    Direction origin = reverse((Direction)((objFlags & OBJBIT_INCOMINGDIR) >> 29));
                    for (Direction d = NORTH; d != NODIR; d = previous(d)) {
                        if (!(objFlags & OBJBIT_NOBACKFIRE) || d != origin)
                            send_impulse(move(p, d), d);
                    }
                    break;
            }
        }
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
        BootRegister(new StoneImpulse(false, false, true,  false),  "st_stoneimpulse_movable");
        BootRegister(new StoneImpulse(false, false, false, true), "st_stoneimpulse_new");
    BOOT_REGISTER_END

} // namespace enigma
