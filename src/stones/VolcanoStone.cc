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

#include "stones/VolcanoStone.hh"
#include "player.hh"
#include "world.hh"
//#include "main.hh"

namespace enigma {
    VolcanoStone::VolcanoStone(int initState) {
        state = initState;
    }
    
    std::string VolcanoStone::getClass() const {
        return "st_volcano";
    }

    void VolcanoStone::setAttr(const string& key, const Value &val) {
        if (key == "secure") {
            if (val.to_bool())
                objFlags |= OBJBIT_SECURE;
            else
                objFlags &= ~OBJBIT_SECURE;
            return;
        }
        Stone::setAttr(key, val);
    }
    
    Value VolcanoStone::getAttr(const string &key) const {
        if (key == "secure") {
            return (objFlags & OBJBIT_SECURE) != 0;
        } else
            return Stone::getAttr(key);
    }
    
    Value VolcanoStone::message(const Message &m) {
        if (m.message == "_trigger" || m.message == "toggle") {
            if (state == INACTIVE) {
                state = ACTIVE;
                init_model();
            }
            return Value();
        }
        return Stone::message(m);
    }

    int VolcanoStone::externalState() const {
        return (state == INACTIVE || state == FINISHED || state == BREAKING) ? 0 : 1;
    }
    
    void VolcanoStone::setState(int extState) {
        if (state <= ACTIVE) {
            state = extState;
            if (isDisplayable())
                init_model();
        }
    }
    
    void VolcanoStone::init_model() {
        switch (state) {
            case FINISHED:
            case INACTIVE: set_model("st_flat"); break;
            case ACTIVE:   set_anim("st_quaking"); break;
            case BREAKING: set_anim("st_quakebreak_anim"); break;
            case NEW:      set_anim("it-seed-growing"); break;
            case GROWING:  set_anim("st-volcano-growing"); break;
        }
    }

    void VolcanoStone::animcb() {
        switch (state) {
            case NEW:
                state = GROWING;
                TouchStone(get_pos());   // inform triggers etc. of state change
                init_model();
                break;
            case GROWING:
                state = ACTIVE;
                init_model();
                break;
            case BREAKING:
                KillStone(get_pos());
                break;
            case ACTIVE:
                // Spread
                GridPos p = get_pos();
                if (DoubleRand(0, 1) > 0.7) spread(move(p, NORTH));
                if (DoubleRand(0, 1) > 0.7) spread(move(p, EAST));
                if (DoubleRand(0, 1) > 0.7) spread(move(p, SOUTH));
                if (DoubleRand(0, 1) > 0.7) spread(move(p, WEST));

                // Be finished at random time
                if ((((objFlags & OBJBIT_SECURE) == 0) || neighborsVulcanized()) && (DoubleRand(0, 1) > 0.95))
                    state = FINISHED;
                init_model();
                break;
        }
    }

    bool VolcanoStone::is_floating() const {
        return state == NEW;
    }
    
    bool VolcanoStone::is_transparent(Direction d) const {
        return false;
    }
    bool VolcanoStone::is_sticky(const Actor *a) const {
        return state != NEW;
    }
    
    StoneResponse VolcanoStone::collision_response(const StoneContact &sc) {
        return state != NEW ? STONE_REBOUND : STONE_PASS;
    }
    
    void VolcanoStone::actor_hit(const StoneContact &sc) {
        Actor *a = sc.actor;

        if( state == ACTIVE && player::WieldedItemIs (a, "it_hammer")) {
            state = BREAKING;
            init_model();
        } else if (state == GROWING) {
            SendMessage(a, "_shatter");
        }
    }
    
    void VolcanoStone::actor_touch(const StoneContact &sc) {
        if (state == GROWING)
            SendMessage(sc.actor, "_shatter");        
    }

    void VolcanoStone::actor_inside(Actor *a) {
        if (state != NEW)
            SendMessage(a, "_shatter");
    }
    
    void VolcanoStone::actor_contact(Actor *a) {
        if (state == GROWING)
            SendMessage(a, "_shatter");
    }
    
    FreezeStatusBits VolcanoStone::get_freeze_bits() {
        return FREEZEBIT_NO_STONE;
    }
    
    void VolcanoStone::spread(GridPos p) {
        if(GetStone(p) == NULL) {
            Stone * st = MakeStone("st_volcano_new");
            st->setAttr("secure", getAttr("secure"));
            SetStone(p, st);
        }
    }
    
    bool VolcanoStone::neighborsVulcanized() {
        for (Direction d = WEST; d != NODIR; d = next(d)) {
            if (GetStone(move(get_pos(), d)) == NULL)
                if (!positionVulcanizable(move(get_pos(), d)))
                    return false;
        }
        return true;
    }
    
    bool VolcanoStone::positionVulcanizable(GridPos p) {
        for (Direction d = WEST; d != NODIR; d = next(d)) {
            VolcanoStone *v = dynamic_cast<VolcanoStone *>(GetStone(move(p, d)));
            if (v != this && v != NULL && v->externalState() == 1)
                return true;
        }
    }
    
    DEF_TRAITSM(VolcanoStone, "st_volcano", st_volcano, MOVABLE_BREAKABLE);
    
    BOOT_REGISTER_START
        BootRegister(new VolcanoStone(0), "st_volcano");
        BootRegister(new VolcanoStone(0), "st_volcano_idle");
        BootRegister(new VolcanoStone(1), "st_volcano_active");
        BootRegister(new VolcanoStone(2), "st_volcano_new");
        BootRegister(new VolcanoStone(3), "st_volcano_growing");
    BOOT_REGISTER_END

} // namespace enigma
