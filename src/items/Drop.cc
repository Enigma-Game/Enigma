/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2009,2010 Ronald Lamprecht
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

#include "items/Drop.hh"
//#include "errors.hh"
#include "main.hh"
#include "player.hh"
#include "world.hh"

namespace enigma {

    Actor *replace_actor(Actor *olda, Actor *newa) {
        ActorInfo *info = newa->get_actorinfo();
        info->vel = olda->get_vel();
        info->charge = olda->get_actorinfo()->charge;
        info->last_gridpos = olda->get_actorinfo()->gridpos;

        Value v = olda->getAttr("owner");
        if (v) {
            player::ReplaceActor((int)v, olda, newa);
        }

        AddActor(olda->get_pos()[0], olda->get_pos()[1], newa);
        if (!YieldActor (olda)) {
            enigma::Log << "Strange: could not remove old actor\n";
        }
        SendMessage(newa, "_update_mass", v);
        SendMessage(newa, "_update_pin", v);
        olda->hide();
        newa->show();
        return olda;
    }

    class DropCallback : public enigma::TimeHandler {
        Actor *rotor;
        Actor *old;
    
    public:
        DropCallback (Actor *rotor_, Actor *old_)
        : rotor (rotor_), old (old_)
        {}

        // TimerHandler interface
        virtual void alarm()
        {
            replace_actor (rotor, old);

            delete rotor;
            delete this;
        }
    };

/* -------------------- Drop -------------------- */

    Drop::Drop() : Item() {
    }

    std::string Drop::getClass() const {
        return "it_drop";
    }
        
    ItemAction Drop::activate(Actor *a, GridPos) {
        const double ROTOR_LIFETIME = 5.0;

        int     iplayer = a->getAttr("owner");
        ActorID id      = get_id (a);

        if (id == ac_marble_black || id == ac_marble_white) {
            // Kill ALL rubberbands connected with the actor:
            SendMessage(a, "disconnect");
            Actor *rotor = MakeActor("ac_rotor");
            rotor->setAttr("adhesion", 1.0);
            rotor->setAttr("controllers", iplayer+1);
            rotor->setAttr("owner", iplayer);
            rotor->setAttr("gohome", false);
            rotor->setAttr("essential", a->getAttr("essential"));
            std::string essId;
            if (Value v = a->getAttr("essential_id")) {
                essId = v.to_string();
            } else {
                essId = a->get_traits().name;
            }
            rotor->setAttr("essential_id", Value(essId));

            replace_actor(a, rotor);

            GameTimer.set_alarm (new DropCallback (rotor, a),
                                        ROTOR_LIFETIME,
                                        false);
        }
        return ITEM_KILL;          // remove from inventory
    }
    
    DEF_ITEMTRAITS(Drop, "it_drop", it_drop);

    BOOT_REGISTER_START
        BootRegister(new Drop(), "it_drop");
    BOOT_REGISTER_END

} // namespace enigma
