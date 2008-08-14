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

#include "items/Sensor.hh"
#include "actors.hh"
//#include "errors.hh"
//#include "main.hh"
#include "server.hh"
#include "world.hh"
#include "items/GlassesItem.hh"

namespace enigma {
    Sensor::Sensor(bool inverse, bool isFilter) {
        Object::setAttr("inverse", inverse);
        if (isFilter) {
            objFlags |= OBJBIT_ISFILTER;
            Item::setAttr("invisible", true);
        }
    }
    
    Value Sensor::message(const Message &m) {
        if (m.message == "_hit") {   // door knocking forward to black/whitballstone
            setAttr("$hitactor", m.value);
            performAction(true);
            setAttr("$hitactor", (Object *)NULL);
            return Value();
        } else if (m.message == "_hitactor") {
            return getAttr("$hitactor");
        } else if (m.message == "_glasses") {
            if (isDisplayable())
                init_model();            
        } else if (m.message == "signal" && (objFlags & OBJBIT_ISFILTER)) {
            if (m.value == 1)
                performAction(true);   // invertion done by Object
            return Value();
        }
        return Item::message(m);
    }
    
    void Sensor::setAttr(const string& key, const Value &val) {
        if (key == "invisible") {
            Item::setAttr(key, val);
            if (isDisplayable())
                init_model();
            return;
        }
        Item::setAttr(key, val);
    }
    
    void Sensor::init_model() {
        if (getAttr("invisible").to_bool() && ((server::GlassesVisibility & Glasses::SENSOR) == 0))
            set_model("invisible");
        else
            set_model("it_sensor");
    }

    void Sensor::animcb() {
        init_model();
    }

    void Sensor::actor_enter(Actor *a) {
        if (!getAttr("invisible").to_bool())
            set_anim("it_sensor_hit");
        performAction(true);
    }
    
    int Sensor::traitsIdx() const {
        int idx = getAttr("inverse").to_bool() ? 1 : 0;
        if (objFlags & OBJBIT_ISFILTER)
            idx +=2;
        return idx;
    }

    ItemTraits Sensor::traits[4] = {
        {"it_sensor",  it_sensor,  itf_static}, 
        {"it_sensor_inverse",  it_inversesensor,  itf_static},
        {"it_sensor_filter1", it_signalfilter1, itf_static | itf_invisible, 0.0},  // DAT only
        {"it_sensor_filter0", it_signalfilter0, itf_static | itf_invisible, 0.0}   // DAT only
    };
    BOOT_REGISTER_START
        BootRegister(new Sensor(true), "it_sensor");
        BootRegister(new Sensor(false), "it_sensor_inverse");
        BootRegister(new Sensor(true, true), "it_sensor_filter1");
        BootRegister(new Sensor(false, true), "it_sensor_filter0");
    BOOT_REGISTER_END

} // namespace enigma
    
