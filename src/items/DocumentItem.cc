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

#include "items/DocumentItem.hh"
#include "client.hh"
//#include "errors.hh"
//#include "main.hh"
#include "lev/Index.hh"
#include "lev/Proxy.hh"
#include "world.hh"

namespace enigma {

    DocumentItem::DocumentItem() : Item() {
    }
    
    Value DocumentItem::message(const Message &m) {
        bool explode = false;

        if (m.message == "ignite") {
            // dynamite does not blow up Documents in Oxyd1
            explode = server::GameCompatibility != GAMET_OXYD1;
        } else if (m.message == "_explosion") {
            explode = true;
        } else {
            return Item::message(m);
        }

        if (explode)
            replace("it_explosion_nil");
        return Value();
    }
    
    ItemAction DocumentItem::activate(Actor *a, GridPos) {
        if (Value v = getAttr("text")) {
            std::string txt(v);
            lev::Proxy *level = lev::Index::getCurrentProxy();
            // after complete switch to Proxy as levelloader the following
            // conditional can be abolished
            if (level)
                // translate text
                txt = level->getLocalizedString(txt);
            client::Msg_ShowText(txt, true);
        }
        return ITEM_KILL;          // remove from inventory
    }

    DEF_ITEMTRAITSF(DocumentItem, "it_document", it_document, itf_inflammable);

    BOOT_REGISTER_START
        BootRegister(new DocumentItem(), "it_document");
    BOOT_REGISTER_END

} // namespace enigma
