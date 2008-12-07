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

#include "items/Magnet.hh"
#include "actors.hh"
//#include "errors.hh"
//#include "main.hh"
//#include "server.hh"
#include "world.hh"

namespace enigma {
    Magnet::Magnet(bool isOn) : Item(), correctedStrength (0.6 * 30), squareRange (1000 * 1000) {
        state = isOn ? ON : OFF;
    }

    std::string Magnet::getClass() const {
        return "it_magnet";
    }
    
    void Magnet::setAttr(const std::string &key, const Value &val) {
        if (key == "range") {
            double range = (val.getType() == Value::NIL) ? server::MagnetRange : (double)val;
            squareRange = range * range;
        } else if (key == "strength") {
            correctedStrength = 0.6 * ((val.getType() == Value::NIL) ? server::MagnetForce : (double)val);
        }
        Item::setAttr(key, val);
    }
    
    Value Magnet::message(const Message &m) {
        if (m.message == "_updateglobals" && m.value.to_string() == "it_magnet") {
            if (getAttr("range").getType() == Value::DEFAULT) {
                squareRange = server::MagnetRange * server::MagnetRange;
            }
            if (getAttr("strength").getType() == Value::DEFAULT) {
                correctedStrength = 0.6 * server::MagnetForce;
            }
            return Value();
        }
        return Item::message(m);
    }
    void Magnet::init_model() {
        set_model(ecl::strf("it-magnet%s", state == ON ? "-on" : "-off"));
    }
    
    void Magnet::on_creation(GridPos p) {
        if (getAttr("range").getType() == Value::DEFAULT) {
            squareRange = server::MagnetRange * server::MagnetRange;
        }
        if (getAttr("strength").getType() == Value::DEFAULT) {
            correctedStrength = 0.6 * server::MagnetForce;
        }

        AddForceField(this);
        Item::on_creation(p);
    }
    
    void Magnet::on_removal(GridPos p) {
        Item::on_removal(p);
        RemoveForceField(this);
    }
    
    void Magnet::add_force(Actor *a, V2 &f) {
        if (state == ON) {
            V2 dv = get_pos().center() - a->get_pos_force();
            double squareDist = square(dv);

            if (squareDist >= 0.04 && squareDist < squareRange)
                f += (correctedStrength / squareDist) * dv;
        }
    }
    
    ItemTraits Magnet::traits[2] = {
        { "it_magnet_off", it_magnet_off, itf_static | itf_indestructible, 0.0 },
        { "it_magnet_on",  it_magnet_on,  itf_static | itf_indestructible, 0.0 },
    };
    
    BOOT_REGISTER_START
        BootRegister(new Magnet(false), "it_magnet");
        BootRegister(new Magnet(false), "it_magnet_off");
        BootRegister(new Magnet(true), "it_magnet_on");
    BOOT_REGISTER_END

} // namespace enigma
