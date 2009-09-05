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

#include "actors/Rotors.hh"
#include "errors.hh"
//#include "main.hh"
#include "player.hh"
#include "world.hh"

namespace enigma {

/* -------------------- RotorBase  -------------------- */

    RotorBase::RotorBase(const ActorTraits &tr) : Actor (tr),  attacknearest (false), attackCurrentOnly (false),
            timeKeepAttackStrategy (0) {
        setAttr("range", 5.0);
        setAttr("strength", 10.0);
        setAttr("gohome", true);
        setAttr("attacknearest", false);
        setAttr("prefercurrent", 0.0);
    }

    void RotorBase::setAttr(const string& key, const Value &val) {
        if (key == "range")
            range = val;
        else if (key == "strength") 
            force = val;
        else if (key == "gohome") 
            gohome = val.to_bool();
        else if (key == "attacknearest")
            attacknearest = val.to_bool();
        else if (key == "prefercurrent") 
            prefercurrent = val;
        Actor::setAttr(key, val);
    }

    bool RotorBase::is_dead() const {
        return false;
    }
    
    void RotorBase::think(double dtime)  {
        double cforce = force/6;
    
        Actor *target = 0;
        V2     target_vec;
        timeKeepAttackStrategy  -= dtime;
        if (timeKeepAttackStrategy < 0) {
            timeKeepAttackStrategy = enigma::DoubleRand(0.8, 1.6);
            attackCurrentOnly = (enigma::DoubleRand(0.0, 1.0) < prefercurrent);
        }
    
        ActorsInRangeIterator air_it = ActorsInRangeIterator(this, range,
                1<<ac_marble_white | 1<<ac_marble_black | 1<<ac_pearl_white);
        Actor *a;
        while((a = air_it.next()) != NULL) {
            if (a->is_movable() && !a->is_invisible()) {
                V2 v = a->get_pos() - get_pos();
                if (attacknearest && !attackCurrentOnly ||
                        attackCurrentOnly && a == player::GetMainActor(
                        player::CurrentPlayer())) {
                    if (!target || (length(v) < length(target_vec))) {
                        target = a;
                        target_vec = v;
                    }
                } else if (!attackCurrentOnly) {            
                    target = a;
                    target_vec += normalize(v);
                }
            }
        }
    
        if (!target && gohome) { 
            // no actors focussed -> return to start position
            target_vec = get_respawnpos()-get_pos();
        }
    
        double target_dist = length(target_vec);
    
        if (target_dist > 0.2)
            target_vec.normalize();
        add_force(target_vec * cforce);
    
        Actor::think(dtime);
    }
    
/* -------------------- Rotor  -------------------- */
    Rotor::Rotor() : RotorBase(traits) {
    }
    
    std::string Rotor::getClass() const {
        return "ac_rotor";
    }

    bool Rotor::on_collision (Actor *a) {
        if (a->is_on_floor())
            SendMessage(a, "_shatter");
        return false;
    }

    ActorTraits Rotor::traits = {"ac_rotor", ac_rotor, 1<<ac_rotor, 22.0/64, 0.8};

/* -------------------- Top  -------------------- */
    Top::Top() : RotorBase(traits) {
    }

    std::string Top::getClass() const {
        return "ac_top";
    }

    bool Top::on_collision (Actor *a) {
        // tops shatter and collide independent on flying status of oponent 
        SendMessage(a, "_shatter");
        return true;
    }
    
    ActorTraits Top::traits = {"ac_top", ac_top, 1<<ac_top, 16.0/64, 0.8};
    
    BOOT_REGISTER_START
        BootRegister(new Rotor(), "ac_rotor");
        BootRegister(new Top(), "ac_top");
    BOOT_REGISTER_END

} // namespace enigma


