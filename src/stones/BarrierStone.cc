/*
 * Copyright (C) 2007,2008,2009 Ronald Lamprecht
 * Copyright (C) 2008 Andreas Lochmann
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

#include "stones/BarrierStone.hh"
#include "errors.hh"
#include "player.hh"
#include "Inventory.hh"

namespace enigma {
    
    BarrierStone::BarrierStone() : Stone() {
    }
    
    std::string BarrierStone::getClass() const {
        return "st_barrier";
    }
        
    void BarrierStone::setAttr(const std::string &key, const Value &val) {
        Stone::setAttr(key, val);
        if (key == "flavor") {
            std::string flavor = val.to_string();
            Item *it = dynamic_cast<Item *>(GetObjectTemplate(val.to_string()));
            ASSERT(flavor == "all" || (it != NULL && it->isPortable()), XLevelRuntime, 
                    ecl::strf("Barrier stone with illeagal flavor '%s'", flavor.c_str()).c_str());
            if (isDisplayable())
                init_model();
        }
    }
    
    void BarrierStone::init_model() {
        set_model("st_barrier_" + getAttr("flavor").to_string());
    }
    
    bool BarrierStone::is_removable() const {
        return false;
    }
    
    bool BarrierStone::is_sticky (const Actor *) const {
        return false;
    }
    
    StoneResponse BarrierStone::collision_response(const StoneContact &sc) {
        assert(sc.actor);
        std::string flavor(getAttr("flavor"));
        enigma::Inventory *inv = player::GetInventory(sc.actor);
        if (flavor == "all") {
            if (inv && inv->size() > 0)
                return STONE_REBOUND;
        } else {
            if ((inv && inv->containsKind(flavor)) ||
                    (flavor == "it_umbrella" && sc.actor->has_shield()) ||
                    (flavor == "it_cherry" && sc.actor->is_invisible()) ||
                    (flavor == "it_drop" && sc.actor->isKind("ac_rotor")) )
                return STONE_REBOUND;
        }
        return STONE_PASS;
    }

    DEF_TRAITSM(BarrierStone, "st_barrier", st_barrier, MOVABLE_PERSISTENT);

    BOOT_REGISTER_START
        BootRegister(new BarrierStone(), "st_barrier");
    BOOT_REGISTER_END

} // namespace enigma
