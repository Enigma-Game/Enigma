/*
 * Copyright (C) 2008 Ronald Lamprecht
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

#include "stones/FakeStone.hh"
#include "errors.hh"
//#include "main.hh"

namespace enigma {
    FakeStone::FakeStone(int subtyp) : Stone() {
        objFlags |= (subtyp << 24);
    }
    
    std::string FakeStone::getClass() const {
        return "st_fake";
    }
    
    void FakeStone::setAttr(const string& key, const Value &val) {
        if (key == "flavor") {
            std::string fl = val.to_string();
            FakeStoneTyp typ = QUAKE;
            if (fl == "quake") typ = QUAKE;
            else if (fl == "oxyda") typ = OXYDA;
            else if (fl == "oxydb") typ = OXYDB;
            else if (fl == "oxydc") typ = OXYDC;
            else if (fl == "oxydd") typ = OXYDD;
            else if (fl == "oxyde") typ = OXYDE;
            else
                ASSERT(false, XLevelRuntime, ecl::strf("Fake stone with illegal flavor value: %s", fl.c_str()).c_str());
            objFlags = objFlags & OBJBIT_SUBTYP | (typ << 24);
            if (isDisplayable())
                init_model();
            return;
        } else
            Stone::setAttr(key, val);
    }
    
    Value FakeStone::getAttr(const std::string &key) const {
        if (key == "flavor") {
            return getFlavor();
        } else
            return Stone::getAttr(key);
    }

    void FakeStone::setState(int extState) {
        if (isDisplayable()) {
            if (((objFlags & OBJBIT_SUBTYP) >> 25) == QUAKE && state != extState) {
                state = extState;
                init_model();
            }
            // oxyd fakes do not allow a state change once they are set
        } else
            Stone::setState(extState);
    }
    
    void FakeStone::init_model() {
        FakeStoneTyp typ = (FakeStoneTyp)((objFlags & OBJBIT_SUBTYP) >> 24);
        switch (typ) {
            case QUAKE:
                if (state == ACTIVE)
                    set_anim("st-farting");
                else
                    set_model("st-fart");
                break;
            case OXYDA:
            case OXYDB:
            case OXYDC:
            case OXYDD:
            case OXYDE:
                set_model(ecl::strf("st-like%s%s", getFlavor().c_str(), state == IDLE ? "" : "-open").c_str());
                break;
        }
    }
    
    void FakeStone::animcb() {
        state = IDLE;
        init_model();
    }

    const char * FakeStone::collision_sound() {
        if ((FakeStoneTyp)((objFlags & OBJBIT_SUBTYP) >> 24) == QUAKE)
            return "metal";
        else
            return "stone";
    }
    
    void FakeStone::actor_hit(const StoneContact &sc) {
        if (((objFlags & OBJBIT_SUBTYP) >> 24) == QUAKE && state == IDLE) {
            sound_event("fakeoxyd");
            state = ACTIVE;
            init_model();
        } else
            Stone::actor_hit(sc);
    }
    
    std::string FakeStone::getFlavor() const {
        FakeStoneTyp typ = (FakeStoneTyp)((objFlags & OBJBIT_SUBTYP) >> 24);
        switch (typ) {
            case QUAKE:
                return "quake";
            case OXYDA:
                return "oxyda";
            case OXYDB:
                return "oxydb";
            case OXYDC:
                return "oxydc";
            case OXYDD:
                return "oxydd";
            case OXYDE:
                return "oxyde";
        }
    }
    
    BOOT_REGISTER_START
        BootRegister(new FakeStone(0), "st_fake_quake");
        BootRegister(new FakeStone(1), "st_fake_oxyda");
        BootRegister(new FakeStone(2), "st_fake_oxydb");
        BootRegister(new FakeStone(3), "st_fake_oxydc");
        BootRegister(new FakeStone(4), "st_fake_oxydd");
        BootRegister(new FakeStone(5), "st_fake_oxyde");
    BOOT_REGISTER_END

} // namespace enigma

