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

    ShogunDot::ShogunDot(int initState) {
        state = initState;
    }

    std::string ShogunDot::getClass() const {
        return "it_shogun";
    }
    
    Value ShogunDot::message(const Message &m) {
        if (m.message =="_init" ) {
            if (SendMessage(GetStone(get_pos()), "_shogun", (state/2 -1)*4).to_bool()) {
                objFlags |= OBJBIT_ACTIVE;
                if (server::EnigmaCompatibility < 1.10)
                    performAction(true);
            }
        } else if (m.message =="_shogun" && server::WorldInitialized) {
            if (!(objFlags & OBJBIT_ACTIVE) && ((state/2 -1)*4 == (int)m.value)) {  // shoguns s, sm, sml for dots s, m, l
                performAction(true);
                objFlags |= OBJBIT_ACTIVE;
            } else if ((objFlags & OBJBIT_ACTIVE) && ((state/2 -1)*4 != (int)m.value)) {  // shoguns s, sm, sml for dots s, m, l
                performAction(false);
                objFlags &= ~OBJBIT_ACTIVE;
            }
            return Value();
        }
        return Item::message(m);
    }
    
    void ShogunDot::setState(int extState) {
        ASSERT(extState == 16 || extState == 8 || extState == 4, XLevelRuntime,"");
        state = extState;
        if (isDisplayable()) {
            init_model();
        }
    }
    
    void ShogunDot::on_creation(GridPos p) {
        if (server::WorldInitialized &&
                SendMessage(GetStone(get_pos()), "_shogun", (state/2 -1)*4).to_bool())
            objFlags |= OBJBIT_ACTIVE;
        Item::on_creation(p);
    }
    
    int ShogunDot::traitsIdx() const {
        return state == 16 ? 2 : (state == 8 ? 1 : 0);
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
