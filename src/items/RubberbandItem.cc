/*
 * Copyright (C) 2006,2007 Raoul Bourquin
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

#include "items/RubberbandItem.hh"

//#include "main.hh"
#include "world.hh"

namespace enigma {
    RubberbandItem::RubberbandItem() {
    }
    
    std::string RubberbandItem::getClass() const {
        return "it_rubberband";
    }
    
    ItemAction RubberbandItem::activate(Actor *a, GridPos p) {
        // TODO: Multiple Targets!
        // TODO: Target for black and target for white marble?
        // TODO: MultiplayerGame: Defaulttarget is second actor!

        // Get actor or stone with the name, given in "connect_to":
        ObjectList ol = getAttr("anchor2").getObjectList(a);
        
        // Target does NOT exist, Drop Item
        if (ol.size() == 0)
            return ITEM_DROP;

        Object *anchor2 = ol.front();
            
        // The mode attribute "scissor" defines, if when using an it-rubberband,
        // other rubberbands to the actor will be cut of or not, true means they will. false is default.
        bool isScissor = to_bool(getAttr("scissor"));

        if (isScissor)
            SendMessage(a, "disconnect");

        sound_event ("rubberband");
        
        if (anchor2 != a) { // It's not allowed to connect a rubberband to self.
            Object *obj = MakeObject("ot_rubberband");
            obj->setAttr("anchor1", a);
            obj->setAttr("anchor2", anchor2);
            obj->setAttr("strength", getAttr("strength"));
            obj->setAttr("length", getAttr("length"));
            obj->setAttr("threshold", getAttr("threshold"));
            obj->setAttr("max", getAttr("max"));
            obj->setAttr("min", getAttr("min"));
            AddOther(dynamic_cast<Other *>(obj));
            transferIdentity(obj);
            SendMessage(obj, "_performaction");
            return ITEM_KILL;
        } else
            return ITEM_DROP;
    }
    
    DEF_ITEMTRAITS(RubberbandItem, "it_rubberband", it_rubberband);
    
    BOOT_REGISTER_START
        BootRegister(new RubberbandItem(), "it_rubberband");
    BOOT_REGISTER_END

} // namespace enigma
