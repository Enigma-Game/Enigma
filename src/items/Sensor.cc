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
    Sensor::Sensor(bool inverse, bool isFilter, bool exit) {
        Object::setAttr("inverse", inverse);
        if (isFilter) {
            objFlags |= OBJBIT_ISFILTER | OBJBIT_INVISIBLE;
        }
        if (exit) {
            objFlags |= OBJBIT_EXIT;
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
            objFlags = (objFlags & ~OBJBIT_INVISIBLE) | (val.to_bool() ? OBJBIT_INVISIBLE : 0);
            if (isDisplayable())
                init_model();
            return;
        } else if (key == "exit") {
            objFlags = (objFlags & ~OBJBIT_EXIT) | (val.to_bool() ? OBJBIT_EXIT : 0);
            if (isDisplayable())
                init_model();
            return;
        }
        Item::setAttr(key, val);
    }
    
    Value Sensor::getAttr(const std::string &key) const {
        if (key == "invisible") {
            return (objFlags & OBJBIT_INVISIBLE) != 0;
        } else if (key == "exit") {
            return (objFlags & OBJBIT_EXIT) != 0;
        }
        return Item::getAttr(key);
    }
    
    void Sensor::init_model() {
        if ((objFlags & OBJBIT_INVISIBLE) && ((server::GlassesVisibility & Glasses::SENSOR) == 0))
            set_model("invisible");
        else if (objFlags & OBJBIT_EXIT)
            set_model("it_sensor_exit");
        else
            set_model("it_sensor");
    }
    
    void Sensor::actor_enter(Actor *a) {
        if (!(objFlags & OBJBIT_EXIT) && !a->is_dead()) {        
            if (!(objFlags & OBJBIT_INVISIBLE))
                set_anim("it_sensor_hit");
            performAction(a);
        }
    }
    
    void Sensor::actor_leave(Actor *a) {
        if ((objFlags & OBJBIT_EXIT) && !a->is_dead()) {        
            if (!(objFlags & OBJBIT_INVISIBLE))
                set_anim("it_sensor_exit_hit");
            performAction(a);
        }
    }

    void Sensor::animcb() {
        init_model();
    }

    int Sensor::traitsIdx() const {
        int idx = getAttr("inverse").to_bool() ? 1 : 0;
        if (objFlags & OBJBIT_ISFILTER)
            idx +=2;
        if (objFlags & OBJBIT_EXIT)
            idx = 4;
        return idx;
    }

    ItemTraits Sensor::traits[5] = {
        {"it_sensor",  it_sensor,  itf_static | itf_indestructible}, 
        {"it_sensor_inverse",  it_inversesensor,  itf_static | itf_indestructible},
        {"it_sensor_filter1", it_signalfilter1, itf_static | itf_invisible | itf_indestructible, 0.0},  // DAT only
        {"it_sensor_filter0", it_signalfilter0, itf_static | itf_invisible | itf_indestructible, 0.0},   // DAT only
        {"it_sensor_exit",  it_exitsensor,  itf_static | itf_indestructible}
    };
    
    BOOT_REGISTER_START
        BootRegister(new Sensor(false), "it_sensor");
        BootRegister(new Sensor(true), "it_sensor_inverse");
        BootRegister(new Sensor(false, true), "it_sensor_filter1");
        BootRegister(new Sensor(true, true), "it_sensor_filter0");
        BootRegister(new Sensor(false, false, true), "it_sensor_exit");
    BOOT_REGISTER_END

} // namespace enigma
    
