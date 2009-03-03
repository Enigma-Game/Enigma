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

#include "stones/SurpriseStone.hh"
#include "errors.hh"
#include "lua.hh"
//#include "main.hh"
#include "server.hh"
#include "Value.hh"
#include "world.hh"

namespace enigma {

/* -------------------- Surprise stone -------------------- */
    SurpriseStone::SurpriseStone() : Stone ("st_surprise") {
    }
    
    std::string SurpriseStone::getClass() const {
        return "st_surprise";
    }

    Value SurpriseStone::getAttr(const std::string &key) const {
        if (key == "selection") {
            static const char *stonename[] = {
                "st_grate_cross",
                "st_death",
                "st_surprise",
                "st_lightglass_hollow",
                "st_knight",
                "st_thief",
                "st_flat_breakable",
                "st_flat_breaking"
            };
            if (Value v = Stone::getAttr(key))
                return v; 
            else {
                TokenList tl;
                for (int i = 0; i < 8; i++)
                    tl.push_back(stonename[i]);
                if (server::GameCompatibility != GAMET_ENIGMA)
                    tl.push_back("st_dongle");
                else
                    tl.push_back("st_puzzle_yellow");
               return tl;
            }
        } else
            return Stone::getAttr(key);
    }
    
    void SurpriseStone::actor_hit(const StoneContact &sc) {
        int id = getId();
            performAction(true);    // may kill the stone!
        
        if (Object::getObject(id) != NULL) {  // not killed?
            TokenList tl = getAttr("selection");
            int idx = enigma::IntegerRand(1, tl.size()) - 1;
            TokenList::iterator itr = tl.begin();
            for (int i = 0; i < idx; i++, ++itr); 
            sound_event("stonetransform");
            std::string name = (*itr).to_string();
            if (name.find('=') == 0) {
                if (lua::CallFunc(lua::LevelState(), "enigma.settile", name.substr(1), this)) {
                    throw XLevelRuntime(std::string("surprise set tile failed:\n")+lua::LastError(lua::LevelState()));
                }
            } else {
                ReplaceStone(get_pos(), MakeStone(name.c_str()));
            }
        }
    }
    
    FreezeStatusBits SurpriseStone::get_freeze_bits() {
        return FREEZEBIT_IRREGULAR;
    }
    
    DEF_TRAITS(SurpriseStone, "st_surprise", st_surprise);

    BOOT_REGISTER_START
        BootRegister(new SurpriseStone(), "st_surprise");
    BOOT_REGISTER_END

} // namespace enigma

