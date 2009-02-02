/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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

#include "stones/CompatibilityStones.hh"
#include "errors.hh"
#include "client.hh"
#include "main.hh"

namespace enigma {

/* -------------------- Oxyd compatibility stones -------------------- */

/* -------------------- DongleStone -------------------- */

    DongleStone::DongleStone() {
    }
    
    void DongleStone::actor_hit(const StoneContact &sc) {
        if (sc.actor->getAttr("owner") && 
            sc.actor->get_vel() * sc.normal < -4)
        {
            KillStone(get_pos());
            client::Msg_ShowText("We don't sell books..", false, 2.0);
        }
    }
    
    DEF_TRAITSM(DongleStone, "st_dongle", st_dongle, MOVABLE_BREAKABLE);


/* -------------------- DummyStone -------------------- */

    DummyStone::DummyStone() : Stone("st_dummy"){
    }
    
    StoneResponse DummyStone::collision_response(const StoneContact &/*sc*/) {
        static int lastCode = -1;
        int        code     = getAttr("code");
        if (code != lastCode) {
            Log << ecl::strf("Collision with stone 0x%02x\n", code);
            lastCode = code;
        }
        return STONE_REBOUND;
    }

    /**  st-easymode Easy-Mode Stone
    
    In easy game mode this stone converts the floor at its
    position to fl-normal.
    In normal game mode the stone kills any item at its position.
    Then in both modes it kills itself.
    
    E.g. it can be used to hide water-barriers or to insert helper
    items that make the level easier in easy game mode.
    */
    
    EasyModeStone::EasyModeStone() {
    }
    
    Value EasyModeStone::message(const Message &m) {
        if (m.message == "_init") {
            if (server::GetDifficulty() == DIFFICULTY_EASY) {
                SetFloor (get_pos(), MakeFloor ("fl_metal_7n"));
            } else {
                KillItem (get_pos());
            }
            KillStone (get_pos());
            return Value();
        }
        return Stone::message(m);
    }

    DEF_TRAITSM(EasyModeStone, "st-easymode", st_easymode, MOVABLE_BREAKABLE);

    /* I have no idea what these stones are _really_ supposed to do;
       they seemingly do not appear in the landscape and they create
       normal floor tiles on creation.  Other than that... who
       knows... */

    Peroxyd_0xb8::Peroxyd_0xb8() {
    }

    void Peroxyd_0xb8::on_creation (GridPos p) {
        SetFloor(p, MakeFloor("fl_metal_7n"));
        KillStone(p);
    }

    DEF_TRAITSM(Peroxyd_0xb8, "st-peroxyd-0xb8", st_peroxyd_0xb8, MOVABLE_BREAKABLE);


    Peroxyd_0xb9::Peroxyd_0xb9() {
    }

    void Peroxyd_0xb9::on_creation (GridPos p) {
        SetFloor(p, MakeFloor("fl_metal_7n"));
        KillStone(p);
    }

    DEF_TRAITSM(Peroxyd_0xb9, "st-peroxyd-0xb9", st_peroxyd_0xb9, MOVABLE_BREAKABLE);

  
    Oxyd_0x18::Oxyd_0x18() {
    }
        
    void Oxyd_0x18::on_creation (GridPos p) {
        KillStone(p);
    }

    DEF_TRAITSM(Oxyd_0x18, "st-oxyd-0x18", st_oxyd_0x18, MOVABLE_BREAKABLE);
    
    BOOT_REGISTER_START
        BootRegister(new DongleStone(), "st_dongle");
        BootRegister(new DummyStone(), "st_dummy");
        BootRegister(new EasyModeStone(), "st-easymode");
        BootRegister(new Peroxyd_0xb8(), "st-peroxyd-0xb8");
        BootRegister(new Peroxyd_0xb9(), "st-peroxyd-0xb9");
        BootRegister(new Oxyd_0x18(), "st-oxyd-0x18");
    BOOT_REGISTER_END

} // namespace enigma

