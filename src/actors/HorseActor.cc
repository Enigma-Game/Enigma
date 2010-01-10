/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2008,2009,2010 Ronald Lamprecht
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

#include "actors/HorseActor.hh"
#include "errors.hh"
//#include "main.hh"
#include "world.hh"

namespace enigma {

/* -------------------- Horse  -------------------- */

    Horse::Horse() : Actor (traits), destidx(0), target(), strength(10) {
        objFlags |= OBJBIT_STEADY;
    }

    std::string Horse::getClass() const {
        return "ac_horse";
    }

    Value Horse::getAttr(const std::string &key) const {
        if (key == "destination") {
            return destination;
        } else if (key == "destidx") { 
            return destidx;
        } else if (key == "steady") { 
            return (bool)(objFlags & OBJBIT_STEADY);
        } else if (key == "strength") { 
            return strength;
        } else
            return Actor::getAttr(key);
    }
    
    void Horse::setAttr(const string& key, const Value &val) {
        if (key == "destination") {
            destination = val;
            if (destination.getType() == Value::NIL)
                objFlags &= ~OBJBIT_AUTOMOVE;
            else {
                destidx = 0;
                objFlags |= OBJBIT_NEWDEST;
            }
        } else if (key == "destidx") { 
            destidx = val;
            if (destidx >= 0)
                objFlags |= OBJBIT_NEWDEST;
            else
                objFlags &= ~OBJBIT_AUTOMOVE;
        } else if (key == "steady") {
            if (val.to_bool())
                 objFlags |= OBJBIT_STEADY;
            else
                 objFlags &= ~OBJBIT_STEADY;
        } else if (key == "strength") { 
            strength = val;
            if (strength < 0)
                objFlags &= ~OBJBIT_AUTOMOVE;
        } else
            Actor::setAttr(key, val);
        
        if (strength >= 0 &&  destidx >= 0 && getAttr("destination").getType() != Value::NIL)
            objFlags |= OBJBIT_AUTOMOVE;
    }

    bool Horse::is_dead() const {
        return false;
    }
    
    void Horse::think(double dtime) {
        if (objFlags & OBJBIT_AUTOMOVE) {
            updateTarget();
            if (objFlags & OBJBIT_AUTOMOVE) {
                if (!(objFlags & OBJBIT_STEADY) && 
                        ecl::square(get_vel()) * 0.6 / strength > ecl::length(target - get_pos()) - 0.05)
                    add_force(- normalize(get_vel()) * strength);
                else                
                    add_force(normalize(target - get_pos()) * strength);
            }
        }
        Actor::think(dtime);
    }

    void Horse::stoneBounce(const StoneContact &sc) {
        if ((objFlags & OBJBIT_AUTOMOVE) && (sc.stonepos == GridPos(target))) {
            updateTarget(true);           
        }
        Actor::stoneBounce(sc);
    }
    
    void Horse::updateTarget(bool touched) {
        if (objFlags & OBJBIT_NEWDEST) {
            // target not defined so far
            ASSERT(getDestinationByIndex(destidx, target), XLevelRuntime, "Horse actor missing valid destination");
            objFlags &= ~OBJBIT_NEWDEST;
        } else if (touched || length(target - get_pos()) < ((objFlags & OBJBIT_STEADY) ? 0.2 : 0.1)) {
            int id = getId();  // in future user might kill actors on callback
            performAction(true);
            // target reached or? try next one
            if ((Object::getObject(id) != NULL)  && (objFlags & OBJBIT_AUTOMOVE) && 
                    !getDestinationByIndex(++destidx, target)) {
                if (getAttr("loop").to_bool()) {
                    destidx = 0;     // failed -> start anew
                    getDestinationByIndex(destidx, target);
                } else
                    objFlags &= ~OBJBIT_AUTOMOVE;
            }
        }
    }
    
    ActorTraits Horse::traits = {"ac_horse", ac_horse, 1<<ac_horse, 24.0/64, 1.2};
    
    BOOT_REGISTER_START
        BootRegister(new Horse(), "ac_horse");
    BOOT_REGISTER_END

} // namespace enigma

