/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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

#include "stones/RubberbandStone.hh"
//#include "main.hh"
#include "player.hh"

namespace enigma {
    RubberbandStone::RubberbandStone() : Stone () {
    }

    std::string RubberbandStone::getClass() const {
        return "st_rubberband";
    }
        
    void RubberbandStone::init_model() {
        set_model("st-rubberband");
    }
    
    void RubberbandStone::actor_hit(const StoneContact &sc) {
        // The mode attribute "scissor" defines, if when touching an st-rubberband,
        // other rubberbands to the actor will be cut of or not, true means they will. true is default.
        bool isScissor = to_bool(getAttr("scissor"));

        bool alreadyConnected = false;
        ObjectList rubbers = sc.actor->getAttr("rubbers");
        for (ObjectList::iterator it =  rubbers.begin(); it != rubbers.end(); ++it) {
            if (((Object *)(*it)->getAttr("anchor2")) == this) {
                alreadyConnected = true;
                break;
            }
        }
        if (!alreadyConnected) {
            sound_event("rubberband");
            if (isScissor) {
                SendMessage(sc.actor, "disconnect");
            }
            Object *obj = MakeObject("ot_rubberband");
            obj->setAttr("anchor1", sc.actor);
            obj->setAttr("anchor2", this);
            obj->setAttr("strength", getAttr("strength"));
            obj->setAttr("length", getAttr("length"));
            obj->setAttr("threshold", getAttr("threshold"));
            obj->setAttr("max", getAttr("max"));
            AddOther(dynamic_cast<Other *>(obj));
        }
        
        maybe_push_stone(sc);
    }

    void RubberbandStone::on_impulse (const Impulse& impulse) {
        Actor *a = dynamic_cast<Actor *>(impulse.sender);
        if (a && player::WieldedItemIs (a, "it_magicwand")) {
            move_stone(impulse.dir);
            propagateImpulse(impulse);
    }

    DEF_TRAITSM(RubberbandStone, "st_rubberband", st_rubberband, MOVABLE_STANDARD);
        
    BOOT_REGISTER_START
        BootRegister(new RubberbandStone(), "st_rubberband");
    BOOT_REGISTER_END

} // namespace enigma
