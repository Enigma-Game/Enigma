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

#include "floors/SimpleFloors.hh"

//#include "errors.hh"
#include "main.hh"
#include "player.hh"
#include "world.hh"

namespace enigma {
    
/* -------------------- Abyss -------------------- */

    Abyss::Abyss() : Floor ("fl_abyss", 0.0, 0.0, flf_indestructible) {
    }
    
    std::string Abyss::getClass() const {
        return "fl_abyss";
    }
    
    bool Abyss::is_destructible() const {
        return false;
    }

    void Abyss::actor_contact (Actor* a) {
        SendMessage(a, "_fall");
    }
    
/* -------------------- DummyFloor -------------------- */

    DummyFloor::DummyFloor() : Floor ("fl_dummy", 4.0, 2.5) {
    }
    
    std::string DummyFloor::getClass() const {
        return "fl_dummy";
    }

    void DummyFloor::actor_contact(Actor *) {
        static int lastCode = -1;
        int code = getAttr("code");
        Log << ecl::strf("Entering floor 0x%x\n", code);
    }

/* -------------------- Ice -------------------- */


    IceFloor::IceFloor() : Floor ("fl_ice") {
    }
    
    std::string IceFloor::getClass() const {
        return "fl_ice";
    }
    
/* -------------------- Space -------------------- */

    
    Space::Space(bool withForce) : Floor("fl_space") {
        if (withForce)
            objFlags |= OBJBIT_FORCETYP;
    }
    
    std::string Space::getClass() const {
        return "fl_space";
    }
    
    void Space::add_force (Actor *a, V2 &f) {
        if (objFlags & OBJBIT_FORCETYP)
            f[1] -= server::FlatForce;
        Floor::add_force(a, f);
    }
    
/* -------------------- Swamp -------------------- */

    Swamp::Swamp() : Floor("fl_swamp", 0.0, 0.0, flf_indestructible) {
    }
    
    std::string Swamp::getClass() const {
        return "fl_swamp";
    }
    
    bool Swamp::is_destructible() const {
        return false;
    }

    void Swamp::get_sink_speed (double &sink_speed, double &raise_speed) const { 
        sink_speed = server::SwampSinkSpeed;
        raise_speed = 6.0;
    }

/* -------------------- Yinyang -------------------- */

    YinyangFloor::YinyangFloor(int initState) : Floor("fl_yinyang", 5.2, 2.0) {
        state = initState;
    }
    
    std::string YinyangFloor::getClass() const {
        return "fl_yinyang";
    }
        
    void YinyangFloor::init_model()  {
        set_model(std::string("fl_yinyang_") + ((state == YIN) ? "yin" : "yang"));
    }
    
    ecl::V2 YinyangFloor::process_mouseforce (Actor *a, ecl::V2 force) {
        if (player::CurrentPlayer() == state)
            return getAdhesion() * force;
        else
            return ecl::V2();
    }
    
    BOOT_REGISTER_START
        BootRegister(new Abyss(),     "fl_abyss");
        BootRegister(new DummyFloor(),"fl_dummy");
        BootRegister(new IceFloor(),  "fl_ice");
        BootRegister(new Space(),     "fl_space");
        BootRegister(new Space(true), "fl_space_force");
        BootRegister(new Swamp(),     "fl_swamp");
        BootRegister(new YinyangFloor(0), "fl_yinyang");
        BootRegister(new YinyangFloor(0), "fl_yinyang_yin");
        BootRegister(new YinyangFloor(1), "fl_yinyang_yang");
    BOOT_REGISTER_END

} // namespace enigma
