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

#include "stones/ScissorsStone.hh"
//#include "main.hh"

namespace enigma {
    ScissorsStone::ScissorsStone() : Stone () {
    }

    std::string ScissorsStone::getClass() const {
        return "st_scissors";
    }
        
    void ScissorsStone::init_model() {
        std::string base = get_traits().name;
        if (state == BREAKING)
            set_anim(base + "_breaking");
        else
            set_model(base);
    }
    
    void ScissorsStone::animcb() {
        if (state == BREAKING) {
            GridPos p = get_pos();
            KillStone(p);
        } else
            init_model();
    }
    
    Value ScissorsStone::message(const Message &m) {
        if (m.message == "_pebble")
            doBreak();
        return Stone::message(m);
    }

    void ScissorsStone::actor_hit(const StoneContact &sc) {
        if (state != IDLE)
            return;
        sound_event("scissors");
        set_anim("st_scissors_snip");
        // Cut connections and (if successful) perform action-target pair.
        if (SendMessage(sc.actor, "disconnect").to_bool())
            performAction(true);
        // Break adjacent st_document
        for (int i = WEST; i <= NORTH; i++) {
            if (Stone *st = GetStone(move(get_pos(), (Direction)i))) {
                SendMessage(st, "_scissors");
            }
        }
        // Break fellow st_document
        ObjectList olist = getAttr("fellows");
        for (ObjectList::iterator it = olist.begin(); it != olist.end(); ++it) {
            Stone *fellow = dynamic_cast<Stone *>(*it);
            if (fellow != NULL)
                SendMessage(fellow, "_scissors");
        }
    }

    void ScissorsStone::doBreak() {
        if (state == IDLE) {
            state = BREAKING;
            sound_event("stonedestroy");
            init_model();
        }
    }

    DEF_TRAITS(ScissorsStone, "st_scissors", st_scissors);
        
    BOOT_REGISTER_START
        BootRegister(new ScissorsStone(), "st_scissors");
    BOOT_REGISTER_END

} // namespace enigma
