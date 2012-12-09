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

#include "items/SurpriseItem.hh"
#include "errors.hh"
#include "lua.hh"
//#include "main.hh"
//#include "server.hh"
#include "Value.hh"
#include "world.hh"

namespace enigma {

/* -------------------- Surprise item -------------------- */
    SurpriseItem::SurpriseItem() : Item() {
    }

    std::string SurpriseItem::getClass() const {
        return "it_surprise";
    }

    Value SurpriseItem::getAttr(const std::string &key) const {
        if (key == "selection") {
            static const char *stonename[] = {
                "it_umbrella",
                "it_spring_keep",
                "it_dynamite",
                "it_coffee",
                "it_hammer"
            };
            if (Value v = Item::getAttr(key))
                return v;
            else {
                TokenList tl;
                for (int i = 0; i < 5; i++)
                    tl.push_back(stonename[i]);
               return tl;
            }
        } else
            return Item::getAttr(key);
    }

    void SurpriseItem::on_drop(Actor *a) {
        int theid = getId();
            performAction(true);    // may kill the stone!

        if (Object::getObject(theid) != NULL) {  // not killed?
            TokenList tl = getAttr("selection");
            int idx = enigma::IntegerRand(1, tl.size()) - 1;
            TokenList::iterator itr = tl.begin();
            for (int i = 0; i < idx; i++, ++itr);
            std::string name = (*itr).to_string();
            if (name.find('=') == 0) {
                if (lua::CallFunc(lua::LevelState(), "enigma.settile", name.substr(1), this)) {
                    throw XLevelRuntime(std::string("surprise set tile failed:\n")+lua::LastError(lua::LevelState()));
                }
            } else {
                replace(name.c_str());
            }
        }
    }

    DEF_ITEMTRAITS(SurpriseItem, "it_surprise", it_surprise);

    BOOT_REGISTER_START
        BootRegister(new SurpriseItem(), "it_surprise");
    BOOT_REGISTER_END

} // namespace enigma

