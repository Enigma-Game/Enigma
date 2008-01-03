/*
 * Copyright (C) 2007,2008 Ronald Lamprecht
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

#include "StateObject.hh"

#include "world.hh"
//#include "main.hh"

namespace enigma {

/* -------------------- StateObject implementation -------------------- */
    StateObject::StateObject() : Object(), state (0) {
    }
    
    StateObject::StateObject(const char * kind) : Object(kind), state (0) {
    }
    
    Value StateObject::message(const Message &m) {
        if (m.message == "toggle") {
            toggleState();
        } else if (m.message == "signal") {
            int v = m.value;    // action bools are converted to int
            if (v >= 0)
                setState(v % 2);  // allow fourswitches to send dir as signal value
            return Value();
        } else if (m.message == "on" || m.message == "open") {
            setState(1);
            return Value();
        } else if (m.message == "off" || m.message == "close") {
            setState(0);
            return Value();
        }
        return Object::message(m);
    }
    
    Value StateObject::getAttr(const string &key) const {
        if (key == "state")
            return externalState();
        return Object::getAttr(key);
    }
    
    void StateObject::set_attrib(const string& key, const Value &val) {
        if (key == "state") {
            if (val >= minState() && val <= maxState())
                setState(val);
            return;
        }
        Object::set_attrib(key, val);
    }
    
    int StateObject::maxState() {
        return 1;  // default 2 state
    }
    
    int StateObject::minState() {
        return 0;  // default 2 state
    }
    void StateObject::toggleState() {
        // default round robin on external states
        int extState = externalState();
        if (extState + 1 <= maxState())
            setState(extState + 1);
        else
            setState(minState());
    }
    
    int StateObject::externalState() const {
        return state;       // default behaviour internal = external state
    }
    
    void StateObject::setState(int extState) {
        state = extState;   // default behaviour internal = external state
    }
    
} // namespace enigma
