/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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

#include "items/TrapItem.hh"
//#include "errors.hh"
//#include "main.hh"
//#include "player.hh"
#include "items/GlassesItem.hh"
#include "world.hh"

namespace enigma {

    TrapItem::TrapItem() {
    }
    
    Value TrapItem::message(const Message &m) {
        if (m.message == "_glasses") {
            if (isDisplayable())
                init_model();            
        }
        return Item::message(m);
    }
    
    void TrapItem::setState(int extState) {
        if (isDisplayable() && state == 0) {       // no toggle back of a broken (open) trap
            state = extState;
            init_model();
        } else
            state = extState;
    }
    
    void TrapItem::init_model() {
        if (state == 0 && ((server::GlassesVisibility & Glasses::TRAP) == 0))
            set_model("invisible");
        else
            set_model("it_trap");
        
    }
    
    void TrapItem::animcb() {
        init_model();
    }
    
    bool TrapItem::actor_hit(Actor *a) {
        if (!a->is_flying()) {
            SendMessage(a, "_fall");
            if (state == 0) {
                state = 1;
                set_anim("it_trap_breaking");
            }
        }
        return false;
    }
        
    DEF_ITEMTRAITSF(TrapItem, "it_trap", it_trap, itf_static | itf_fireproof);

    BOOT_REGISTER_START
        BootRegister(new TrapItem(), "it_trap");
    BOOT_REGISTER_END

} // namespace enigma
