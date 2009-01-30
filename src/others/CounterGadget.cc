/*
 * Copyright (C) 2009 Ronald Lamprecht
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

#include "others/CounterGadget.hh"
#include "errors.hh"
#include "main.hh"
#include "world.hh"

namespace enigma {
    CounterGadget::CounterGadget() : Other() {
    }
    
    std::string CounterGadget::getClass() const {
        return "ot_counter";
    }

    void CounterGadget::setAttr(const std::string &key, const Value &val) {
        if (key == "state") {
            state = val;
        }
        Other::setAttr(key, val);
    }
    
    Value CounterGadget::message(const Message &m) {
        if (m.message == "signal") {
            if (m.value.to_bool()) {
                state++;
            } else {
                state--;
            }
            if (Value stateTarget = getAttr(ecl::strf("target_%d", state))) {
                performAction(true);
            }
            return Value();
        }
        return Other::message(m);
    }

    BOOT_REGISTER_START
        BootRegister(new CounterGadget(), "ot_counter");
    BOOT_REGISTER_END

} // namespace enigma
