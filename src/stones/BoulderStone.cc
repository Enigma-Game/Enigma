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

#include "stones/BoulderStone.hh"
#include "player.hh"
#include "world.hh"
//#include "main.hh"

namespace enigma {
    BoulderStone::BoulderStone(Direction dir) {
        state = INIT;
        // do not use set_dir, because this will set the state to ACTIVE
        Object::setAttr("orientation", dir);
    }
    
    void BoulderStone::setAttr(const string& key, const Value &val) {
        if (key == "orientation") {
            if (isDisplayable())
                setDir(to_direction(val));
            else
                Object::setAttr(key, val);
            return;
        }
        Stone::setAttr(key, val);
    }

    Value BoulderStone::message(const Message &m) {
        if (m.message == "orientate" && state != FALLING && isDisplayable()) {
            setDir(to_direction(m.value));
            return Value();
        } else if ((m.message == "turn" || m.message == "turnback") && state != FALLING && isDisplayable()) {
            setDir((m.message == "turn" && !getAttr("counterclock").to_bool() || 
                    m.message == "turnback" && getAttr("counterclock").to_bool())
                    ? rotate_cw(getDir()) : rotate_ccw(getDir()));
            return Value();
        }
        return Stone::message(m);
    }

    int BoulderStone::externalState() const {
        return 0;   // no external states
    }
    
    void BoulderStone::setState(int extState) {
        // no external states
    }
    
    void BoulderStone::on_creation(GridPos p) {
        activatePhoto();
        Stone::on_creation(p);
    }
    
    void BoulderStone::init_model() {
        std::string mname  = "st-bolder" + to_suffix(getDir());
        if (state == FALLING)
            mname += "-fall-anim";
        set_anim(mname);
    }

    void BoulderStone::animcb() {
        display::Model *m = get_model();
        Direction dir = getDir();
        switch (state) {
            case FALLING:
                KillStone(get_pos());
                break;
            case INIT:
                state = IDLE;
                if (!haveObstacle(dir)) {
                    move_stone(dir);  // makes object ACTIVE
                    init_model();
                } else
                    m->restart();  // same pos and dir
                break;
            case IDLE:
                if (!haveObstacle(dir)) {
                    state = ACTIVE;
                }
                m->restart();  // same dir
                break;
            case ACTIVE:
                triggerObstacle(dir, false);
                if (!move_stone(dir)) {
                    state = IDLE;
                }
                init_model();
                break;
        }
    }

    void BoulderStone::lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs) {
        if (added_dirs(oldDirs, newDirs) != 0) {
            setDir(reverse(getDir()));
        }
    }
    
    void BoulderStone::actor_hit(const StoneContact &sc) {
        if (state == FALLING)
            return;

        if (player::WieldedItemIs (sc.actor, "it-magicwand")) {
            setDir(reverse(getDir()));
        }
    }

    void BoulderStone::on_floor_change() {
        Floor *fl = GetFloor(get_pos());
        if (fl->is_kind("fl-abyss")) {
            state = FALLING;
            init_model();
        }
    }
    
    void BoulderStone::on_move() {
        state = ACTIVE;
        triggerObstacle(getDir(), true);
        Stone::on_move();
    }
    
    void BoulderStone::on_impulse(const Impulse& impulse) {
        if (state == FALLING)
            return;

        if (impulse.sender && impulse.sender->is_kind("st-rotator")) {
            setDir(impulse.dir);  // activate
        }
        move_stone(impulse.dir);  // due to rotator and impulsestone
    }

    Direction BoulderStone::getDir() const {
        return to_direction(getAttr("orientation"));
    }
    
    void BoulderStone::setDir(Direction d) {
        if (d != getDir() && (state != FALLING)) {
            Object::setAttr("orientation", d);
            state = ACTIVE;   // if turned by it-magicwand -> allow triggering
            triggerObstacle(getDir(), true);
            init_model();
        }
    }

    bool BoulderStone::haveObstacle(Direction dir) {
        return GetStone(move(get_pos(), dir)) != NULL;
    }

    void BoulderStone::triggerObstacle(Direction dir, bool isRaising) {
        if (Stone *st = GetStone(move(get_pos(), dir))) {
            SendMessage(st, "_trigger", isRaising, this);
        }
    }

    DEF_TRAITSM(BoulderStone, "st_boulder", st_boulder, MOVABLE_IRREGULAR); // it-blocker requires all boulders to identify as st_boulder
    
    BOOT_REGISTER_START
        BootRegister(new BoulderStone(), "st_boulder");
        BootRegister(new BoulderStone(NORTH), "st_boulder_n");
        BootRegister(new BoulderStone(EAST), "st_boulder_e");
        BootRegister(new BoulderStone(SOUTH), "st_boulder_s");
        BootRegister(new BoulderStone(WEST), "st_boulder_w");
    BOOT_REGISTER_END

} // namespace enigma
