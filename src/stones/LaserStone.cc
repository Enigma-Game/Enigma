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
 */
#include "stones/LaserStone.hh"

#include "main.hh"
#include "timer.hh"
#include "world.hh"

namespace enigma {

    LaserStone::InstanceList LaserStone::instances;

    void LaserStone::reemit_all() {
        for (InstanceList::iterator it = instances.begin(); it != instances.end(); ++it) {
            (*it)->emit_light();
        }
    }

    LaserStone::LaserStone(Direction dir) {
        setAttr("orientation", Value(dir));
    }

    std::string LaserStone::getClass() const {
        return "st_laser";
    }

    LaserStone *LaserStone::clone() {
        LaserStone *o = new LaserStone(*this);
        instances.push_back(o);
        return o;
    }

    void LaserStone::dispose() {
        instances.erase(find(instances.begin(), instances.end(), this));
        delete this;
    }

    void LaserStone::setAttr(const string& key, const Value &val) {
        if (isDisplayable())
            if (key == "orientation") {
                Stone::setAttr(key, val);
                RecalcLight();
                init_model();
                return;
            }
        Stone::setAttr(key, val);
    }

    Value LaserStone::message(const Message &m) {
        if (m.message == "orientate" && isDisplayable()) {
            setAttr("orientation", m.value);
            return Value();
        } else if ((m.message == "turn" || m.message == "turnback") && isDisplayable()) {
            setAttr("orientation", ((m.message == "turn" && !getAttr("counterclock").to_bool()) ||
				    (m.message == "turnback" && getAttr("counterclock").to_bool()))
                    ? rotate_cw(getOrientation()) : rotate_ccw(getOrientation()));
            return Value();
        }
        return Stone::message(m);
    }

    int LaserStone::externalState() const {
        return state % 2;
    }

    void LaserStone::setState(int extState) {
        if (isDisplayable()) {
            switch (state) {
                case OFF:
                case ON:
                    if (state != extState) {
                        state = extState + 2;
                        GameTimer.set_alarm(this, 0.2, false);
                        RecalcLight();
                        init_model();
                    }
                    break;
                case NEW_OFF:
                case NEW_ON:
                    if (state != extState + 2)
                        state += 2;   // remember pending other state
                    break;
                case NEW_OFF_PENDING_ON:
                case NEW_ON_PENDING_OFF:
                    if (state == extState + 4)
                        state -= 2;   // remember pending other state
                    break;
            }
        } else  // object initialisation
            state = extState;
    }

    void LaserStone::toggleState() {
        switch (state) {
            case OFF:
            case NEW_OFF:
            case NEW_ON_PENDING_OFF:
                setState(ON);
                break;
            case ON:
            case NEW_ON:
            case NEW_OFF_PENDING_ON:
                setState(OFF);
                break;
        }
    }

    void LaserStone::init_model()  {
        string mname = externalState() == 1 ? "st-laseron" : "st-laser";
        mname += to_suffix(getOrientation());
        set_model(mname);
    }

    void LaserStone::on_creation (GridPos p) {
        if (state == ON)
            RecalcLight();
        Stone::on_creation(p);
    }

    void LaserStone::on_removal(GridPos p) {
        GameTimer.remove_alarm(this);
        state &= 1;
        Stone::on_removal(p);
    }

    DirectionBits LaserStone::emissionDirections() const {
        if (externalState() == 1) {
            return to_bits(getOrientation());
        }
        return NODIRBIT;
    }

    void LaserStone::alarm() {
        int oldExtState = externalState();
        switch (state) {
                case NEW_OFF:
                    state = OFF;
                    break;
                case NEW_ON:
                    state = ON;
                    break;
                case NEW_OFF_PENDING_ON:
                    state = NEW_ON;
                    GameTimer.set_alarm(this, 0.2, false);
                    break;
                case NEW_ON_PENDING_OFF:
                    state = NEW_OFF;
                    GameTimer.set_alarm(this, 0.2, false);
                    break;
        }
        if (oldExtState != externalState()) {
            RecalcLight();
            init_model();
        }
    }

    void LaserStone::emit_light() {
        if (externalState() == 1)
            LaserBeam::emit_from(get_pos(), getOrientation());
    }

    Direction LaserStone::getOrientation() const {
        return to_direction(getAttr("orientation"));
    }

    BOOT_REGISTER_START
        BootRegister(new LaserStone(EAST), "st_laser");
        BootRegister(new LaserStone(NORTH), "st_laser_n");
        BootRegister(new LaserStone(SOUTH), "st_laser_s");
        BootRegister(new LaserStone(EAST), "st_laser_e");
        BootRegister(new LaserStone(WEST), "st_laser_w");
    BOOT_REGISTER_END

} // namespace enigma
