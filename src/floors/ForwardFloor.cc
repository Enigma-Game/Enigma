/*
 * Copyright (C) 2019 Andreas Lochmann
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

#include "floors/ForwardFloor.hh"

#include "errors.hh"
#include "main.hh"
#include "world.hh"

namespace enigma {
    
    ForwardFloor::ForwardFloor(Direction dir, std::string flavor) : Floor("fl_forward")  {
        Floor::setAttr("flavor", flavor);
        Floor::setAttr("burnable", flavor == "bridgewood");
        state = dir;
    }
    
    ForwardFloor::~ForwardFloor() {
        GameTimer.remove_alarm(this);
    }

    std::string ForwardFloor::getClass() const {
        return "fl_forward";
    }
    
    void ForwardFloor::setAttr(const std::string& key, const Value &val) {
        if (key == "flavor") {
            std::string flavor = val.to_string();
            ASSERT(flavor == "darkgray" || flavor == "bridgewood" ||
                   flavor == "platinum" || flavor == "bright" || flavor == "rough",
                   XLevelRuntime, "ScalesFloor illegal flavor value");
            Floor::setAttr("flavor", val);
            Floor::setAttr("burnable", flavor == "bridgewood");
            if (isDisplayable()) {
                init_model();    // need to redisplay after attribute set
            }
        } else if (key == "orientation") {
            if (val >= minState() && val <= maxState())
                setState(val);
        } else if (key == "interval") {
            Floor::setAttr(key, val);
            update_alarm();
        } else
            Floor::setAttr(key, val);
    }

    Value ForwardFloor::getAttr(const std::string &key) const {
        if (key == "orientation") {
            return state;
        } else
            return Floor::getAttr(key);
    }

    Value ForwardFloor::message(const Message &m) {
        if (m.message == "orientate") {
            setAttr("state", m.value);   // enforce value check
            return Value();
        } else if (m.message == "signal" || m.message == "flip") {
            setState(reverse((Direction)state));
            return Value();
        }
        return Floor::message(m);
    }

    int ForwardFloor::maxState() const {
        return 3;  // dir representing state
    }

    void ForwardFloor::init_model() {
        std::string suffix = toSuffix((Direction)state);
        std::string animname = "fl_forward_" + getAttr("flavor").to_string() + suffix;
        set_anim(animname);
    }

    void ForwardFloor::animcb() {
        init_model();
    }

    void ForwardFloor::on_creation (GridPos p) {
        update_alarm();
        Floor::on_creation(p);
    }

    void ForwardFloor::update_alarm() {
        double i = (double)getAttr("interval");
        GameTimer.remove_alarm(this, ALARM_PREPARE);
        if (i == 0.0)
            return;  // interval 0 means: off.
        if (i >= 0.2) {
            GameTimer.set_alarm(this, i, true, ALARM_PREPARE);
        } else {
            throw XLevelRuntime(std::string("fl_forward needs interval >= 0.2"));
        }
    }

    void ForwardFloor::distinguished_alarm(int alarmnr) {
        if (alarmnr == ALARM_PREPARE) {
            send_impulses();
        } else {  // alarmnr == ALARM_PUSH
            if (Value theid = getAttr("$stoneabove")) {
                GridPos p = get_pos();
                Stone *st = GetStone(p);
                if (st != NULL && theid == st->getId())
                    send_impulse(p, (Direction)state);
            }
        }
    }

    void ForwardFloor::send_impulses() {
        Stone *st = GetStone(get_pos());
        Floor::setAttr("$stoneabove", (st == NULL) ? Value() : Value(st->getId()));
        GameTimer.set_alarm(this, 0.08, false, ALARM_PUSH);
    }

    BOOT_REGISTER_START
        BootRegister(new ForwardFloor( WEST, "darkgray"), "fl_forward");
        BootRegister(new ForwardFloor( WEST, "darkgray"), "fl_forward_darkgray");
        BootRegister(new ForwardFloor( WEST, "darkgray"), "fl_forward_darkgray_w");
        BootRegister(new ForwardFloor(SOUTH, "darkgray"), "fl_forward_darkgray_s");
        BootRegister(new ForwardFloor( EAST, "darkgray"), "fl_forward_darkgray_e");
        BootRegister(new ForwardFloor(NORTH, "darkgray"), "fl_forward_darkgray_n");

        BootRegister(new ForwardFloor( WEST, "platinum"), "fl_forward_platinum");
        BootRegister(new ForwardFloor( WEST, "platinum"), "fl_forward_platinum_w");
        BootRegister(new ForwardFloor(SOUTH, "platinum"), "fl_forward_platinum_s");
        BootRegister(new ForwardFloor( EAST, "platinum"), "fl_forward_platinum_e");
        BootRegister(new ForwardFloor(NORTH, "platinum"), "fl_forward_platinum_n");

        BootRegister(new ForwardFloor( WEST, "rough"), "fl_forward_rough");
        BootRegister(new ForwardFloor( WEST, "rough"), "fl_forward_rough_w");
        BootRegister(new ForwardFloor(SOUTH, "rough"), "fl_forward_rough_s");
        BootRegister(new ForwardFloor( EAST, "rough"), "fl_forward_rough_e");
        BootRegister(new ForwardFloor(NORTH, "rough"), "fl_forward_rough_n");

        BootRegister(new ForwardFloor( WEST, "bright"), "fl_forward_bright");
        BootRegister(new ForwardFloor( WEST, "bright"), "fl_forward_bright_w");
        BootRegister(new ForwardFloor(SOUTH, "bright"), "fl_forward_bright_s");
        BootRegister(new ForwardFloor( EAST, "bright"), "fl_forward_bright_e");
        BootRegister(new ForwardFloor(NORTH, "bright"), "fl_forward_bright_n");

        BootRegister(new ForwardFloor( WEST, "bridgewood"), "fl_forward_bridgewood");
        BootRegister(new ForwardFloor( WEST, "bridgewood"), "fl_forward_bridgewood_w");
        BootRegister(new ForwardFloor(SOUTH, "bridgewood"), "fl_forward_bridgewood_s");
        BootRegister(new ForwardFloor( EAST, "bridgewood"), "fl_forward_bridgewood_e");
        BootRegister(new ForwardFloor(NORTH, "bridgewood"), "fl_forward_bridgewood_n");
    BOOT_REGISTER_END

} // namespace enigma
