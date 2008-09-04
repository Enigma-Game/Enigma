/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2008 Ronald Lamprecht
 * Copyright (C) 2008 Andreas Lochmann
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

#include "stones/Turnstile.hh"
#include "actors.hh"
#include "errors.hh"
//#include "main.hh"
#include "player.hh"
#include "world.hh"
#include <vector>

namespace enigma {

    TurnstilePivot::TurnstilePivot(std::string flavor) : Stone () {
        setAttr("flavor", flavor);
        setAttr("orientation", NORTH);
    }

    std::string TurnstilePivot::getClass() const {
        return "st_turnstile";
    }
        
    void TurnstilePivot::setAttr(const string& key, const Value &val) {
        if (key == "flavor") {
            ASSERT((val.to_string() == "red" || val.to_string() == "green"), XLevelRuntime, 
                    ("Turnstile unknown flavor: " + val.to_string()).c_str());
            Stone::setAttr(key, val);
            if (isDisplayable())
                init_model();
        } else
            Stone::setAttr(key, val);
    }
    
    Value TurnstilePivot::message(const Message &m) {
        if (m.message == "signal") {               // turn , turnback
            if (isDisplayable())
                rotate(m.value == 0, NULL, NULL);
            return Value();
        } else if ((m.message == "turn" || m.message == "turnback")) {
            rotate((m.message == "turn" && !getAttr("counterclock").to_bool() || 
                    m.message == "turnback" && getAttr("counterclock").to_bool()), NULL, NULL);
            return Value();
        }
        return Stone::message(m);
    }
    
    int TurnstilePivot::maxState() const {
        return 0;
    }
    
    void TurnstilePivot::setState(int extState) {
        // ignore all state settings
    }

    void TurnstilePivot::init_model() {
        set_model(std::string("red") == getAttr("flavor").to_string() ? "st-turnstile" : "st-turnstile-green");
    }
    

    void TurnstilePivot::animcb() {
        state = IDLE;
        init_model();
    }
    
    DirectionBits TurnstilePivot::arms_present() const {
        int arms = NODIRBIT;
        GridPos p = get_pos();
        for (Direction dir = WEST; dir != NODIR; dir = next(dir)) {
            if (TurnstileArm * ta = dynamic_cast<TurnstileArm *>(GetStone(move(p, dir)))) {
                if (ta->getAttr("state") == dir)
                    arms |= to_bits(dir);
            }
        }
        return (DirectionBits)arms;
    }
    
    bool TurnstilePivot::no_stone(int xoff, int yoff) const {
        GridPos p = get_pos();
        p.x += xoff;
        p.y += yoff;
        return (NULL == GetStone(p));
    }

    bool TurnstilePivot::rotate(bool clockwise, Object *impulse_sender, TurnstileArm *initiator) {
        if (state == ROTATING || !isDisplayable())
            return false;
    
        DirectionBits arms = arms_present();
        bool can_rotate = true;
    
        if (clockwise)  {
            if (arms & NORTHBIT) {
                can_rotate &= no_stone(+1,-1);
                if (! (arms & EASTBIT)) can_rotate &= no_stone(+1,0);
            }
            if (arms & WESTBIT) {
                can_rotate &= no_stone(-1,-1);
                if (! (arms & NORTHBIT)) can_rotate &= no_stone(0,-1);
            }
            if (arms & SOUTHBIT) {
                can_rotate &= no_stone(-1,+1);
                if (! (arms & WESTBIT)) can_rotate &= no_stone(-1,0);
            }
            if (arms & EASTBIT) {
                can_rotate &= no_stone(+1,+1);
                if (! (arms & SOUTHBIT)) can_rotate &= no_stone(0,+1);
            }
        }
        else {
            if (arms & NORTHBIT) {
                can_rotate &= no_stone(-1,-1);
                if (! (arms & WESTBIT)) can_rotate &= no_stone(-1,0);
            }
            if (arms & WESTBIT) {
                can_rotate &= no_stone(-1,+1);
                if (! (arms & SOUTHBIT)) can_rotate &= no_stone(0,+1);
            }
            if (arms & SOUTHBIT) {
                can_rotate &= no_stone(+1,+1);
                if (! (arms & EASTBIT)) can_rotate &= no_stone(+1,0);
            }
            if (arms & EASTBIT) {
                can_rotate &= no_stone(+1,-1);
                if (! (arms & NORTHBIT)) can_rotate &= no_stone(0,-1);
            }
        }
    
        if (can_rotate) {
            int id = getId();
            sound_event (clockwise ? "turnstileright" : "turnstileleft");
            sound_event("movesmall");
    
            state = ROTATING;
            set_anim(ecl::strf("st-turnstile%s-anim", (std::string("red") == getAttr("flavor").to_string()) ? "" : "-green"));
            rotate_arms(arms, clockwise);
            handleActorsAndItems(clockwise, impulse_sender);

            if (Object::getObject(id) == NULL)   // Killed? Then forget the rest.
                return can_rotate;
        
            Direction dir = to_direction(getAttr("orientation"));
            dir = clockwise ? rotate_cw(dir) : rotate_ccw(dir);
            Stone::setAttr("orientation", dir);
            
            for (Direction dir = WEST; dir != NODIR; dir = next(dir)) {
                if (TurnstileArm * ta = dynamic_cast<TurnstileArm *>(GetStone(move(get_pos(), dir)))) {
                    if (ta != initiator) {
                        Impulse rotationImpulse(this, ta->get_pos(), dir, false);
                        ta->propagateImpulse(rotationImpulse);
                    }
                }
            }
    
            performAction(!clockwise);
            server::IncMoveCounter();
        }
        return can_rotate;
    }

    void TurnstilePivot::rotate_arms(DirectionBits arms, bool clockwise) {
        GridPos p = get_pos();
    
        Object * n = (arms & NORTHBIT) ? YieldStone(move (p, NORTH)) : NULL;
        Object * e = (arms & EASTBIT)  ? YieldStone(move (p, EAST))  : NULL;
        Object * s = (arms & SOUTHBIT) ? YieldStone(move (p, SOUTH)) : NULL;
        Object * w = (arms & WESTBIT)  ? YieldStone(move (p, WEST))  : NULL;
            
        if (clockwise) {
            if (arms & NORTHBIT) setArm(n, EAST);
            if (arms & EASTBIT)  setArm(e, SOUTH);
            if (arms & SOUTHBIT) setArm(s, WEST);
            if (arms & WESTBIT)  setArm(w, NORTH);
        }
        else {
            if (arms & NORTHBIT) setArm(n, WEST);
            if (arms & EASTBIT)  setArm(e, NORTH);
            if (arms & SOUTHBIT) setArm(s, EAST);
            if (arms & WESTBIT)  setArm(w, SOUTH);
        }
    }
    
    void TurnstilePivot::setArm(Object * obj, Direction dir) {
        GridPos newp = move(get_pos(), dir);
        Stone * st = dynamic_cast<Stone *>(obj);
        SetStone(newp, st);
        obj->setAttr("state", dir);
    
        if (Item *it = GetItem(newp))
            it->on_stonehit(st);
    }
   
    bool TurnstilePivot::calc_arm_seen(bool cw, DirectionBits arms, int field) {
        // for each field calculate whether an arm has passed by, first
        // counterclockwise and then clockwise:
        const DirectionBits neededArm[2][8] = {
            {WESTBIT, NORTHBIT, NORTHBIT, EASTBIT, EASTBIT, SOUTHBIT, SOUTHBIT, WESTBIT},
            {NORTHBIT, NORTHBIT, EASTBIT, EASTBIT, SOUTHBIT, SOUTHBIT, WESTBIT, WESTBIT}
        };
        return (arms & neededArm[cw][field]) != 0;
    }

    void TurnstilePivot::handleActorsAndItems(bool clockwise, Object *impulse_sender) {
        const int to_index[3][3] = { // (read this transposed)
            { 0, 7, 6 }, // x == 0
            { 1,-1, 5 }, // x == 1
            { 2, 3, 4 }  // x == 2
        };
        const int to_x[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };
        const int to_y[8] = { -1, -1, -1, 0, 1, 1, 1, 0 };
    
        bool arm_seen[8];
        DirectionBits arms = arms_present(); // Note: already the rotated state
        for (int i = 0; i<8; ++i)
            arm_seen[i] = calc_arm_seen(clockwise, arms, i);
    
        // ---------- Handle items in range ----------
        GridPos pv_pos = get_pos();
        for (int i = 0; i<8; ++i) 
            if (arm_seen[i]) {
                GridPos item_pos(pv_pos.x+to_x[i], pv_pos.y+to_y[i]);
                if (Item *it = GetItem(item_pos)) { 
                    if (it->get_traits().id == it_laserbeam)
                        KillItem(item_pos); 
                    else
                        it->on_stonehit(this); // hit with pivot (shouldn't matter)
                }
            }
    
        // ---------- Handle actors in range ----------
        std::vector<Actor*> actorsInRange;
    
        // tested range is sqrt(sqr(1.5)+sqr(1.5)) 
        if (!GetActorsInRange(pv_pos.center(), 2.124, actorsInRange))
            return;
    
        std::vector<Actor*>::iterator iter = actorsInRange.begin(), end = actorsInRange.end();
        for (; iter != end; ++iter) {
            Actor *ac = *iter;
            const V2 &ac_center = ac->get_pos();
            GridPos   ac_pos(ac_center);
            int dx  = ac_pos.x-pv_pos.x;
            int dy  = ac_pos.y-pv_pos.y;
    
            // ignore if actor is not inside the turnstile square or is not
            // in distance of the the rotating arms
            if ((dx<-1 || dx>1 || dy<-1 || dy>1) || 
                    (length(ac->get_pos() - pv_pos.center()) > 1.58114 + ac->get_actorinfo()->radius))
                continue;
    
            int idx_source = to_index[dx+1][dy+1];
            if (idx_source == -1) 
                continue;       // actor inside pivot -- should not happen
    
            const int rot_index[4][8] = {
                // The warp-destinations for actors. Why different destinations
                // for oxyd/non-oxyd-type turnstiles? Imagine the actor on position
                // 1 (North of pivot), the turnstile rotates anticlockwise. Then
                // a green turnstile-arm, if at all, would push the actor one field
                // to the left (position 0). Now assume it's a red turnstile. If the
                // actor is to be warped, it has to be the one that activated the
                // turnstile. Yet it is on position 1, in principle not able to
                // hit an arm. But it can, if it hits fast enough on the edge of
                // pivot and left arm. In this case, the actor should be handled
                // as if on position 1, thus warping to 6.
                { 6,  0, 0,  2, 2,  4, 4,  6 }, // anticlockwise
                { 2,  2, 4,  4, 6,  6, 0,  0 }, // clockwise
                { 6,  6, 0,  0, 2,  2, 4,  4 }, // anticlockwise (oxyd-compatible)
                { 2,  4, 4,  6, 6,  0, 0,  2 }, // clockwise (oxyd-compatible)
            };
    
            bool compatible = std::string("red") == getAttr("flavor").to_string();   // oxyd compatibility
            int  idx_target = rot_index[clockwise+2*compatible][idx_source]; // destination index
            bool do_warp = false; // move the actor along with the turnstile?
    
            if (compatible) {
                // Move only the actor that hit the turnstile in Oxyd mode
                do_warp = (ac == dynamic_cast<Actor*>(impulse_sender));
                if (!do_warp && arm_seen[idx_source])
                    SendMessage(ac, "shatter"); // hit by an arm
            } else { // green turnstile
                // move all actors only if pushed by an arm
                do_warp = arm_seen[idx_source];
            }
    
            if (!do_warp) 
                continue;
    
            // Pushing an actor out of the level results in a shatter (no warp) instead
            GridPos ac_target_pos(pv_pos.x+to_x[idx_target], pv_pos.y+to_y[idx_target]);
    
            if(!IsInsideLevel(ac_target_pos)) {
                SendMessage(ac, "shatter");
                continue;
            }

            // Keep the actors rotated position within grid, but ensure that
            // - actors once warped keep warping on further turnstile turns
            // - small marbles end up in meditation hollows  
            ecl::V2 offset = ac->get_pos() - ac->get_gridpos().center();
            if (ecl::length(offset) > 0.5 - ac->get_actorinfo()->radius)
                offset =  (0.5 - ac->get_actorinfo()->radius) * ecl::normalize(offset);
            WarpActor(ac, ac_target_pos.x + 0.5 + (clockwise ? -offset[1] : offset[1]),
                     ac_target_pos.y + 0.5 + (clockwise ? offset[0] : -offset[0]), false);
    
            if (Stone *st = GetStone(ac_target_pos)) {
    
                // destination is blocked
    
                TurnstileArm *arm = dynamic_cast<TurnstileArm*>(st);
                if (arm && !compatible) { // if blocking stone is turnstile arm -> hit it!
                    const int impulse_dir[2][8] = {
                        // anticlockwise
                        { SOUTHBIT|WESTBIT, WESTBIT, NORTHBIT|WESTBIT, NORTHBIT,
                          NORTHBIT|EASTBIT, EASTBIT, SOUTHBIT|EASTBIT, SOUTHBIT },
                        // clockwise
                        { NORTHBIT|EASTBIT, EASTBIT, SOUTHBIT|EASTBIT, SOUTHBIT,
                          SOUTHBIT|WESTBIT, WESTBIT, NORTHBIT|WESTBIT, NORTHBIT }
                    };
    
                    DirectionBits possible_impulses =
                        static_cast<DirectionBits>(impulse_dir[clockwise][idx_target]);
    
                    for (int d = 0; d<4; ++d) 
                        if (has_dir(possible_impulses, Direction(d))) 
                            ac->send_impulse(ac_target_pos, Direction(d));
    
    //                 if (GetStone(ac_target_pos) == 0)  // arm disappeared
    //                     break;
                }
            }
        }
    }

    int TurnstilePivot::traitsIdx() const {
        return (std::string("red") == getAttr("flavor").to_string()) ? 0 : 1;
    }
    
    StoneTraits TurnstilePivot::traits[2] = {
        {"st_turnstile_red", st_turnstile_red, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_turnstile_green", st_turnstile_green, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
    };
    


    TurnstileArm::TurnstileArm(Direction dir) : Stone () {
        state = dir;
    }

    std::string TurnstileArm::getClass() const {
        return "st_turnstilearm";
    }
        
    void TurnstileArm::setAttr(const string& key, const Value &val) {
        if (key == "orientation") {                // TODO connections support
            setState(val);
        } else if (key == "connections") {
            GridObject::setAttr(key, val);
            DirectionBits db = (DirectionBits)(int)getAttr("$connections");
            switch (db) {
                case WESTBIT :
                    state = WEST; break;
                case SOUTHBIT :
                    state = SOUTH; break;
                case EASTBIT :
                    state = EAST; break;
                case NORTHBIT :
                    state = NORTH; break;
                default :
                    ASSERT(false, XLevelRuntime, ("Turnstile set attribute 'connections' with illegal value '" 
                            + val.to_string() + "'").c_str());
            }
        } else
            Stone::setAttr(key, val);
    }
    
    Value TurnstileArm::getAttr(const std::string &key) const {
        if (key == "orientation") {
            return state;
        } else if (key == "connections") {
            static const char *sfx[] = { "w", "s", "e", "n" };
            return sfx[state];
        } else
            return Stone::getAttr(key);
    }
    
    Value TurnstileArm::message(const Message &m) {
        if (m.message == "orientate") {
            setAttr("state", m.value);    // do check min, max limits
        }
        return Stone::message(m);
    }
    
    int TurnstileArm::maxState() const {
        return NORTH;
    }
    
    void TurnstileArm::init_model() {
        set_model(ecl::strf("st-turnstile%s", to_suffix((Direction)state).c_str()));
    }
    
    void TurnstileArm::on_impulse(const Impulse& impulse) {
        enum Action { ROTL, ROTR, stay };
        static Action actions[4][4] = {
            { stay, ROTL, stay, ROTR }, // west arm
            { ROTR, stay, ROTL, stay }, // south arm
            { stay, ROTR, stay, ROTL }, // east arm
            { ROTL, stay, ROTR, stay }  // north arm
        };
        
        int id = getId();
        TurnstilePivot *pivot = getPivot();
    
        if (pivot != NULL) {
            Action a = actions[state][impulse.dir];
            if (a != stay) {
                bool clockwise = (a == ROTR);
                Actor *actor = dynamic_cast<Actor*>(impulse.sender);
                Log << "Turnstile impulse " << get_id(pivot) << " - "<<st_turnstile_green <<"\n";
                if (get_id(pivot) == st_turnstile_green && actor != NULL &&
                        player::WieldedItemIs(actor, "it_wrench")) {
                    Log << "Turnstile Wand\n";
                    clockwise = !clockwise;
                }
                pivot->rotate(clockwise, impulse.sender, this); // ROTR is clockwise
            }
        }
        else {
            // Move arms not attached to a pivot individually
            move_stone(impulse.dir);
        }
        
        if (Object::getObject(id) != NULL)   // not killed?
            propagateImpulse(impulse);
    }

    TurnstilePivot *TurnstileArm::getPivot() {
        return dynamic_cast<TurnstilePivot *>(GetStone(move(get_pos(), reverse((Direction)state))));
    }
    
    StoneTraits TurnstileArm::traits[4] = {
        {"st_turnstilearm_w", st_turnstilearm_w, stf_none, material_stone, 1.0, MOVABLE_IRREGULAR},
        {"st_turnstilearm_s", st_turnstilearm_s, stf_none, material_stone, 1.0, MOVABLE_IRREGULAR},
        {"st_turnstilearm_e", st_turnstilearm_e, stf_none, material_stone, 1.0, MOVABLE_IRREGULAR},
        {"st_turnstilearm_n", st_turnstilearm_n, stf_none, material_stone, 1.0, MOVABLE_IRREGULAR},
    };
    
    BOOT_REGISTER_START
        BootRegister(new TurnstilePivot("red"), "st_turnstile");
        BootRegister(new TurnstilePivot("red"), "st_turnstile_red");
        BootRegister(new TurnstilePivot("green"), "st_turnstile_green");
        BootRegister(new TurnstileArm(NORTH), "st_turnstilearm");
        BootRegister(new TurnstileArm(WEST),  "st_turnstilearm_w");
        BootRegister(new TurnstileArm(SOUTH), "st_turnstilearm_s");
        BootRegister(new TurnstileArm(EAST),  "st_turnstilearm_e");
        BootRegister(new TurnstileArm(NORTH), "st_turnstilearm_n");
    BOOT_REGISTER_END


} // namespace enigma

