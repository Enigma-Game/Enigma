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

#include "stones/ChameleonStone.hh"

#include "errors.hh"
//#include "main.hh"

namespace enigma {

    ChameleonStone::ChameleonStone() : Stone() {
    }
    
    std::string ChameleonStone::getClass() const {
        return "st_chameleon";
    }
    
    Value ChameleonStone::message(const Message &m) {
        if (m.message == "_model_reanimated") {
                init_model();
        }
        return Stone::message(m);
    }
    
    void ChameleonStone::init_model()  {
        if (Floor *fl = GetFloor(get_pos())) {
            set_model(fl->getModelName());
        } else
            ASSERT(false, XLevelRuntime, "Chameleon Stone: Floor must be set before stone");
    }
    
    bool ChameleonStone::is_floating() const {
        return true;
    }

    StoneResponse ChameleonStone::collision_response(const StoneContact &) {
        return STONE_PASS;
    }

    DEF_TRAITS(ChameleonStone, "st_chameleon", st_chameleon);

    BOOT_REGISTER_START
        BootRegister(new ChameleonStone(), "st_chameleon");
    BOOT_REGISTER_END

} // namespace enigma
