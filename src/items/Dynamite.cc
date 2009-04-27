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

#include "items/Dynamite.hh"
//#include "errors.hh"
//#include "main.hh"
#include "world.hh"

namespace enigma {

    Dynamite::Dynamite(int initState) {
        state = initState;
    }
    
    std::string Dynamite::getClass() const {
        return "it_dynamite";
    }
    
    Value Dynamite::message(const Message &m) {
        if (m.message == "ignite" || m.message == "_explosion" || m.message == "_dispenser") {
            if (isDisplayable())
                burn();
            return Value();
        } else if (m.message == "explode" || m.message == "_cannonball") { // currently unused in c++ code
            if (isDisplayable())
                explode();
            return Value();
        } else if (m.message == "heat") {  // used by fire-system
            if (isDisplayable())
                burn();
            return true;  // caught message -> no fire!
        } else if (m.message == "_freeze") {
            if (state == BURNING) {
                state = IDLE;
                init_model();
            }
        }
        return Item::message(m);
    }
    
    void Dynamite::setState(int extState) {
        if (state == IDLE && extState == 1) {
            state = BURNING;
            if (isDisplayable())
                init_model();
        }
    }
    
    void Dynamite::init_model() {
        if (state == IDLE) {
            set_model("it_dynamite");
        } else
            set_anim("it_dynamite_burning");
    }
    
    void Dynamite::processLight(Direction d) {
        burn();
    }
    
    void Dynamite::animcb() {
        explode();
    }
    
    bool Dynamite::isStatic() const {
        return state == BURNING;  // burning dynamite cannot be picked up
    }
    
    void Dynamite::on_drop(Actor *a) {
        burn();
    }
    
    void Dynamite::burn() {
        if (state == IDLE) {
            state = BURNING;
            init_model();
        }
    }
    
    void Dynamite::explode() {
        GridPos p = get_pos();
        SendExplosionEffect(p, EXPLOSION_DYNAMITE);
        sound_event ("dynamite");
        Floor *fl = GetFloor(p);
        std::string flclass = fl->getClass();
        if (flclass == "fl_space") {
            replace("it_explosion_hollow");
        } else if (flclass == "fl_ice") {
            replace("it_explosion_crack");
        } else {
            replace("it_explosion_hollow");
        }
    }
    
    DEF_ITEMTRAITSF(Dynamite, "it_dynamite", it_dynamite, itf_indestructible | itf_fireproof);

    BOOT_REGISTER_START
        BootRegister(new Dynamite(0), "it_dynamite");
        BootRegister(new Dynamite(1), "it_dynamite_burning");
    BOOT_REGISTER_END

} // namespace enigma
