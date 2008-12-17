/*
 * Copyright (C) 2007 Andreas Lochmann
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

#include "stones/ChessStone.hh"
#include "errors.hh"
#include "player.hh"

namespace enigma {
    double ChessStone::capture_interval = 0.1;
    double ChessStone::hit_threshold = 1.5;

    ChessStone::ChessStone(int color) {
        setAttr("$destination", GridPos(0,0));
        state = IDLE;
        set_color(color);
    }

    ChessStone::~ChessStone() {
        GameTimer.remove_alarm (this);
    }
    
    std::string ChessStone::getClass() const {
        return "st_chess";
    }
    
    void ChessStone::setAttr(const string& key, const Value &val) {
        if (key == "color") {
            set_color(to_int(val));
        } else
            Stone::setAttr(key, val);
    }

    Value ChessStone::message(const Message &m) {
        if (m.message == "_capture") {
            if (isDisplayable() && state == IDLE && m.value.to_string() != getKind())
                if (try_state(CAPTURED)) {
                    set_anim(get_model_name() + "-captured");
                    return true;
                }
            return Value();
        } else if(m.message == "move") {
            if (isDisplayable())
                return message_move(m.value);
            else
                return Value();
        } else if (m.message == "signal") {
            set_color(to_int(m.value));
            return Value();
        } else if (m.message == "flip") {
            set_color((objFlags & OBJBIT_NEWCOLOR) ? 0 : 1);
            return Value();
        }
        return Stone::message(m);
    }
    
    void ChessStone::setState(int extState) {
        // block any write attempts
    }

    void ChessStone::init_model() { 
        set_model(get_model_name());
    }

    void ChessStone::animcb() {
        Stone *st;
        switch (state) {
            case APPEARING:
                if (try_state(IDLE))
                // Maybe falling in the meantime? Otherwise:
                    init_model();
                break;
            case DISAPPEARING:
                // Maybe the floor has changed into swamp or abyss?
                if (!(objFlags & (OBJBIT_FALL | OBJBIT_SINK))) {
                    if (try_state(APPEARING)) {
                        st = GetStone(getAttr("$destination"));
                        if (st) {
                            // Something went wrong while killing the old
                            // stone, or maybe a third one intervened.
                            // Don't move, just reappear at old position.                
                        } else {
                            move_stone(getAttr("$destination"), "movesmall");
                            SendMessage(GetFloor(getAttr("$destination")), "_capture");
                        }
                        // maybe a floor-change has happened, but during
                        // state APPEARING this doesn't mean anything:
                        set_anim(get_model_name()+"-appearing");
                    }
                    break;
                }
                // Else: If floor is swamp or abyss, kill the stone. Do this
                // just by continuing to the next case:
            case CAPTURED:
            case FALLING:
            case SINKING:
                KillStone(get_pos());
                break;
            default:
               ASSERT(false, XLevelRuntime, "ChessStone: inconsistent state in animcb()");
        }
    }

    void ChessStone::actor_hit(const StoneContact &sc) {
         Value accolor = sc.actor->getAttr("color");
         if (player::WieldedItemIs (sc.actor, "it_magicwand")) {
             sound_event ("stonepaint");
             set_color(1 - (int)getAttr("color"));
             // If not IDLE, color will be set next time IDLE is set.
         } else if (accolor && getAttr("color") == accolor) {
            V2 v = sc.actor->get_vel();
            Direction dir1 = get_push_direction(sc);
            if(dir1 == NODIR)  return;
            Direction dir2 = NODIR;
            if (dir1 == SOUTH || dir1 == NORTH) {
                dir2 = v[0] >  hit_threshold ? EAST :
                       v[0] < -hit_threshold ? WEST : NODIR;
            } else {
                dir2 = v[1] >  hit_threshold ? SOUTH :
                       v[1] < -hit_threshold ? NORTH : NODIR;
            }
            if(dir2 == NODIR)  return;
            // maybe_move_to tests for state == IDLE by itself.
            maybe_move_to(dir1, dir2);
        }
    }

    void ChessStone::on_impulse(const Impulse& impulse) {
    }
    
    bool ChessStone::is_removable() const {    // Interface for st-swap and st-pull
        return state == IDLE;
    }

    void ChessStone::on_floor_change() {
        Floor *fl = GetFloor(get_pos());
        if (fl != NULL) {
            if (fl->is_kind("fl_abyss"))
                try_state(FALLING);
            if (fl->is_kind("fl_swamp"))
                try_state(SINKING);
        }
    }
    
    void ChessStone::alarm() {
        ASSERT(state == CAPTURING, XLevelRuntime,
            "ChessStone: inconsistent state in alarm()");
        int capture_retry = (objFlags & OBJBIT_CAPTURE_RETRY) >> 27;
        if(!GetStone(getAttr("$destination"))) {
            if(try_state(DISAPPEARING))
                set_anim(get_model_name() + "-disappearing");
        } else if(capture_retry < max_capture_retry) {
            objFlags &= ~OBJBIT_CAPTURE_RETRY;
            objFlags |= ((capture_retry + 1) << 27) & OBJBIT_CAPTURE_RETRY;
            GameTimer.set_alarm(this, capture_interval, false);
        } else {
            // Cancel efforts to capture foreign stone.
            objFlags &= ~OBJBIT_CAPTURE_RETRY;
            try_state(IDLE);
        }
    }

    std::string ChessStone::get_model_name() {
        return getClass() + ((getAttr("color") == 0) ? "_black" : "_white");
    }
    
    Value ChessStone::maybe_move_to(Direction dir1, Direction dir2) {
        if (state == IDLE) {
            // check for fire, step by step
            GridPos destination = move(get_pos(), dir1);
            if (Item *it = GetItem(destination))
                if (get_id(it) == it_burnable_burning)
                    return Value();
            destination = move(destination, dir2);
            if (Item *it = GetItem(destination))
                if (get_id(it) == it_burnable_burning)
                    return Value();
            destination = move(destination, dir1);
            if (Item *it = GetItem(destination))
                if (get_id(it) == it_burnable_burning)
                    return Value();
            // check for boundary
            if (!IsInsideLevel(destination))
                return Value();
            // safe destination to attribute, needed after animation
            setAttr("$destination", destination);
            // check for stone
            if (!GetStone(destination)) {
                // Simple case: Just move.
                if (try_state(DISAPPEARING)) {
                    set_anim(get_model_name() + "-disappearing");
                    return true;
                } else
                    return Value();
            } else {
                // Test stone. Is it opposite chess stone or totally another one?
                Stone *st = GetStone(destination);
                if(to_int(SendMessage(st, "_capture", Value(get_model_name()))) ) {
                    // Give it some time for animation, then replace it.
                    ASSERT(try_state(CAPTURING), XLevelRuntime,
                        "ChessStone: strange things happening in maybe_move_to");
                    // must work, because state is IDLE
                    GameTimer.set_alarm(this, capture_interval, false);
                    objFlags &= ~OBJBIT_CAPTURE_RETRY;
                    return true;
                }
                return Value();
            }
        }
        return Value();
    }

    Value ChessStone::message_move(Value moveDir) {
        GridPos dir = moveDir;
        if (std::abs(dir.x) == 2 && std::abs(dir.y) == 1)
            return maybe_move_to(dir.x > 0 ? EAST : WEST, dir.y > 0 ? SOUTH : NORTH);
        if (std::abs(dir.x) == 1 && std::abs(dir.y) == 2)
            return maybe_move_to(dir.y > 0 ? SOUTH : NORTH, dir.x > 0 ? EAST : WEST);

        return Value();
    }

    bool ChessStone::try_state(State newstate) {
        if (state != FALLING && state != SINKING) {
            // Switch to FALLING or SINKING only when IDLE,
            // but remember them!
            if (newstate == FALLING)
                objFlags |= OBJBIT_FALL;
            else if (newstate == SINKING)
                objFlags |= OBJBIT_SINK;
            else
                state = newstate;        
            // After changing the state to IDLE, do we
            // have to make up for something?
            if (state == IDLE && getAttr("color") != ((objFlags & OBJBIT_NEWCOLOR) ? 1 : 0))
                set_color((objFlags & OBJBIT_NEWCOLOR) ? 1 : 0);
            if (state == IDLE && (objFlags & OBJBIT_FALL)) {
                state = FALLING;
                set_anim(get_model_name() + "-disappearing");
            }
            if (state == IDLE && (objFlags & OBJBIT_SINK)) {
                state = SINKING;
                set_anim(get_model_name() + "-swamp");
            }
        }
        return state == newstate;
    }

    void ChessStone::set_color(int color) {
        if(color != 0 && color != 1) {
            ASSERT(0, XLevelRuntime, "ChessStone: argument to color not 0 or 1");
        }
        if(state == IDLE) {
            Stone::setAttr("color", color);
            objFlags &= ~OBJBIT_NEWCOLOR;
            objFlags |= (color == 1) ? OBJBIT_NEWCOLOR : false;
            if (isDisplayable())
                init_model();
        } else {
            // Remember this color and set it the next time IDLE is set.
            objFlags &= ~OBJBIT_NEWCOLOR;
            objFlags |= (color == 1) ? OBJBIT_NEWCOLOR : false;
        }
    }
    
    DEF_TRAITSM(ChessStone, "st_chess", st_chess, MOVABLE_IRREGULAR);
    
    BOOT_REGISTER_START
        BootRegister(new ChessStone(0), "st_chess");
        BootRegister(new ChessStone(0), "st_chess_black");
        BootRegister(new ChessStone(1), "st_chess_white");
    BOOT_REGISTER_END

} // namespace enigma
