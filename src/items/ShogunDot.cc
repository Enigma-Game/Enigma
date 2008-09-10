/*
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

#include "items/ShogunDot.hh"
#include "stones/ShogunStone.hh"
#include "errors.hh"
//#include "main.hh"
#include "world.hh"

namespace enigma {

    ShogunDot::ShogunDot(int holes) {
        objFlags |= holes << 24;
    }

    std::string ShogunDot::getClass() const {
        return "it_shogun";
    }
    
    void ShogunDot::setAttr(const string& key, const Value &val) {
        if (key == "flavor") {
            std::string flavor = val.to_string();
            int holes = 0;
            if (flavor == "s")
                holes = ShogunStone::S;
            else if (flavor == "m")
                holes = ShogunStone::M;
            else if (flavor == "l")
                holes = ShogunStone::L;
            else
                ASSERT(false, XLevelRuntime, ecl::strf("ShogunDot: illegal 'flavor' of '%s'", flavor.c_str()).c_str());
            objFlags &= ~OBJBIT_HOLES;
            objFlags |= holes << 24;
            if (isDisplayable()) {
                init_model();
                state = SendMessage(GetStone(get_pos()), "_shogun", requiredShogunHoles()).to_bool() ? ON : OFF;
                // no action is performed due to snapshot principle 
            }
            return;
        }
        Item::setAttr(key, val);
    }
    
    Value ShogunDot::getAttr(const string &key) const {
        if (key == "flavor") {
            int holes = getHoles();
            return holes == ShogunStone::L ? "l" : (state == ShogunStone::M ? "m" : "s");
        } else
            return Item::getAttr(key);
    }
    
    Value ShogunDot::message(const Message &m) {
        if (m.message =="_init" ) {
            state = (SendMessage(GetStone(get_pos()), "_shogun", requiredShogunHoles()).to_bool()) ? ON : OFF;
            if (server::EnigmaCompatibility < 1.10 && state == ON) {
                performAction(true);
            }
        }
        return Item::message(m);
    }
    
    void ShogunDot::setState(int extState) {
        // deny any write access 
    }
    
    void ShogunDot::on_creation(GridPos p) {
        if (server::WorldInitialized &&
                SendMessage(GetStone(get_pos()), "_shogun", requiredShogunHoles()).to_bool())
            state = ON;
        Item::on_creation(p);
    }
    
    void ShogunDot::stone_change(Stone *st) {
        int newState = SendMessage(GetStone(get_pos()), "_shogun", requiredShogunHoles()).to_bool() ? ON : OFF;
        if (state != newState ) {  
            state = newState;
            performAction(newState == ON);
        }
    }
    
    int ShogunDot::getHoles() const {
        return (objFlags & OBJBIT_HOLES) >> 24;
    }
    
    int ShogunDot::requiredShogunHoles() const {  // currently shoguns s, sm, sml for dots s, m, l
        static int smallestHole = ShogunStone::S;
        return (getHoles()/smallestHole * 2 - 1) * smallestHole;
    }
    
    int ShogunDot::traitsIdx() const {
        int holes = getHoles();
        return holes == ShogunStone::L ? 2 : (holes == ShogunStone::M ? 1 : 0);
    }
    

    ItemTraits ShogunDot::traits[3] = {
        { "it_shogun_s", it_shogun_s, itf_static, 0.0 },
        { "it_shogun_m", it_shogun_m, itf_static, 0.0 },
        { "it_shogun_l", it_shogun_l, itf_static, 0.0 }
    };

    BOOT_REGISTER_START
        BootRegister(new ShogunDot(4), "it_shogun");
        BootRegister(new ShogunDot(4), "it_shogun_s");
        BootRegister(new ShogunDot(8), "it_shogun_m");
        BootRegister(new ShogunDot(16), "it_shogun_l");
    BOOT_REGISTER_END

} // namespace enigma
