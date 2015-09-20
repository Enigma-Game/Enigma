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
 */
#include "stones/DeathStone.hh"
#include "items/GlassesItem.hh"

namespace enigma {
    DeathStone::DeathStone(bool isInvisible, bool isMovable) : Stone () {
        if (isInvisible)
            objFlags |= OBJBIT_INVISIBLE;
        if (isMovable)
            objFlags |= OBJBIT_MOVABLE;
            
        state = IDLE;
    }

    DeathStone::~DeathStone() {
        GameTimer.remove_alarm(this);
    }
    
    std::string DeathStone::getClass() const {
        return "st_death";
    }
        
    void DeathStone::setAttr(const std::string& key, const Value &val) {
        if (key == "invisible") {
            if (val.to_bool() != ((objFlags & OBJBIT_INVISIBLE) != 0)) {
                // change of visibility
                objFlags ^= OBJBIT_INVISIBLE;
                if (isDisplayable())
                    init_model();
            }
            if ((objFlags & OBJBIT_INVISIBLE) && (objFlags & OBJBIT_INVISIBLE))
                objFlags &= ~OBJBIT_MOVABLE;    // there is no invisible movable death
        } else if (key == "movable") {
            GameTimer.remove_alarm(this);
            if (val.to_bool()) {
                objFlags |= OBJBIT_MOVABLE;
                if (objFlags & OBJBIT_INVISIBLE)  // there is no invisible movable death
                    objFlags &= ~OBJBIT_INVISIBLE;
                if (isDisplayable())
                    setAlarm();
            } else
                objFlags &= ~OBJBIT_MOVABLE;
            if (isDisplayable())
                init_model();
        } else
            Stone::setAttr(key, val);
    }
    
    Value DeathStone::getAttr(const std::string &key) const {
        if (key == "invisible") {
            return (objFlags & OBJBIT_INVISIBLE) != 0;
        } else if (key == "movable") {
            return (bool)(objFlags & OBJBIT_MOVABLE);
        } else
            return Stone::getAttr(key);
    }
    
    Value DeathStone::message(const Message &m) {
        if (m.message == "_model_reanimated") {
            state = IDLE;   // reset any running anim, be ready to bump again on new grid
            init_model();   // visibility might have changed
        } else if (m.message == "_glasses") {
            if (state == IDLE)   // avoid restart of animations
                init_model();
            return Value();
        } else if (m.message == "_trigger" && !m.value.to_bool() && (objFlags & OBJBIT_MOVABLE)) {
            Direction incoming = NODIR;
            if (m.sender != NULL)
                incoming = direction_fromto(dynamic_cast<GridObject *>(m.sender)->get_pos(), get_pos());
            if (incoming != NODIR)
                move_stone(incoming);
            return Value();
        }
        return Stone::message(m);
    }
            
    void DeathStone::setState(int extState) {
        if (state == IDLE && extState == 1 && isDisplayable()) {
            state = PULSING;
            set_anim("st-death-anim");
        }
    }

    void DeathStone::init_model() {
        if (state == IDLE) {
            if ((objFlags & OBJBIT_INVISIBLE) && ((server::GlassesVisibility & Glasses::DEATH) == 0))
                set_model("invisible");
            else if ((objFlags & OBJBIT_MOVABLE) && ((server::GlassesVisibility & Glasses::DEATH) != 0))
                set_model("st_death_light");
            else
                set_model("st_death");
        } else {
            if ((objFlags & OBJBIT_MOVABLE) && ((server::GlassesVisibility & Glasses::DEATH) != 0))
                set_anim("st_death_light-anim");
            else
                set_anim("st_death-anim");
        }
        // PULSING anim is always visible and should continue independent on init_model calls
    }
    
    void DeathStone::on_creation (GridPos p) {
        if (objFlags & OBJBIT_MOVABLE)
            setAlarm();

        Stone::on_creation(p);    // init the model
    }
    
    void DeathStone::on_removal(GridPos p) {
        if (objFlags & OBJBIT_MOVABLE)
            GameTimer.remove_alarm(this);
        Stone::on_removal(p);
    }
    
    void DeathStone::animcb() {
        if (state == PULSING) {
            state = IDLE;
            init_model();
        }
    }
    
    void DeathStone::actor_hit(const StoneContact &sc) {
        actor_touch(sc);
        if (sc.actor->has_shield())   // just shielded actors are allowed to push this stone
            Stone::actor_hit(sc);
    }

    void DeathStone::actor_touch(const StoneContact &sc) {
        // even a slight touch should shatter the actor: 
        SendMessage(sc.actor, "_shatter");
        if (state == IDLE) {
            state = PULSING;
            init_model();
        }
    }
    
    void DeathStone::alarm() {
        if (state == IDLE) {
            state = PULSING;
            init_model();
        }
        setAlarm();
    }

    void DeathStone::setAlarm() {
        double dt = (double)getAttr("interval");
        if (dt > 0) {
            dt *= DoubleRand(0.8, 1.2);
            GameTimer.set_alarm(this, dt, false);
        }
    }
    
    int DeathStone::traitsIdx() const {
        return (objFlags & OBJBIT_INVISIBLE) ? 1 : ((objFlags & OBJBIT_MOVABLE) ? 2 : 0);
    }
    
    StoneTraits DeathStone::traits[3] = {
        {"st_death", st_death, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_death_invisible", st_death_invisible, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT},
        {"st_death_movable", st_death_movable, stf_none, material_stone, 1.0, MOVABLE_STANDARD},
    };
    
    BOOT_REGISTER_START
        BootRegister(new DeathStone(false), "st_death");
        BootRegister(new DeathStone(true), "st_death_invisible");
        BootRegister(new DeathStone(false, true), "st_death_movable");
    BOOT_REGISTER_END

} // namespace enigma
