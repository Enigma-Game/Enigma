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

#include "stones/SpitterStone.hh"
#include "errors.hh"
#include "Inventory.hh"
#include "player.hh"
#include "server.hh"
#include "world.hh"
//#include "main.hh"

namespace enigma {
    SpitterStone::SpitterStone() {
        state = ACTIVE;
    }
    
    Value SpitterStone::message(const Message &m) {
        if (m.message == "_cannonball") {
            if (!getAttr("secure").to_bool() || server::GameCompatibility == enigma::GAMET_PEROXYD) {
                state = BREAKING;
                init_model();
            }
            return Value();
        } else if (m.message == "spit" && state == ACTIVE && isDisplayable()) {
            ecl::V2 dest = m.value;
            if (!IsInsideLevel(dest)) {
                int idx = getDefaultedAttr("$hitdestindex", 0);
                if (!getDestinationByIndex(idx++, dest)) {
                    if (idx != 1) {
                        idx = 0;
                        if (!getDestinationByIndex(idx++, dest))
                            return Value();
                    } else
                        return Value();
                }
                setAttr("$hitdestindex", idx);
                if (!IsInsideLevel(dest))
                    return Value();
            }
            ecl::V2 vel = (GridPos(dest).center() - get_pos().center()) /0.55;
            setAttr("$ball_velocity", vel);
            state = LOADING;
            init_model();
            return Value();
        } else if (m.message == "_model_reanimated") {
            if (state == LOADING)
                state = SPITTING;   // abort - position, velocity are bogus anyway
            init_model();  // replace bogus model - state may have changed
        }
        return Stone::message(m);
    }
    
    int SpitterStone::externalState() const {
        return state == IDLE ? 0 : 1;
    }
    
    void SpitterStone::setState(int extState) {
        if (state != extState) {
            state = (extState == 0) ? IDLE : ACTIVE;
            if (isDisplayable())
                init_model();
        }
    }
    
    void SpitterStone::init_model() {
        if (state == IDLE)
            set_model("st_spitter_idle");
        else if (state == ACTIVE)
            set_anim("st_spitter_active");
        else if (state == LOADING)
            set_anim("st_spitter_loading");
        else if (state == SPITTING)
            set_anim("st_spitter_spitting");
        else
            set_anim("st_spitter_breaking");
    }
    
    void SpitterStone::animcb() {
        switch (state) {
            case ACTIVE :
                ASSERT(false, XLevelRuntime, "SpitterStone: animcb called with inconsistent state"); 
            case LOADING : {
                Other *cb = dynamic_cast<Other *>(MakeObject("ot_cannonball"));
                ecl::V2 center  = get_pos().center();
                cb->setAttr("$position", center);
                cb->setAttr("$ball_velocity", getAttr("$ball_velocity"));
                AddOther(cb);
                
                state = SPITTING;
                init_model();
                break;
            }
            case SPITTING : 
                state = ACTIVE;
                init_model();
                break;
            case BREAKING :
                SetItem(get_pos(), MakeItem("it_meditation"));
                KillStone(get_pos());
                break;
        }
    }

    void SpitterStone::actor_hit (const StoneContact &sc) {
        if (state != ACTIVE)
            return;
    
        if (enigma::Inventory *inv = player::GetInventory(sc.actor)) {
            int lifepos = inv->find("it_extralife");
            if (lifepos != -1) {
                delete inv->yield_item(lifepos);
                player::RedrawInventory (inv);
                ecl::V2 vel = distortedVelocity(sc.actor->get_vel(), 1.0);
                double maxvel = (double)getAttr("range")/0.56;
                if (maxvel * maxvel < square(vel))
                    vel = maxvel * normalize(vel);
                setAttr("$ball_velocity", vel);
                state = LOADING;
                init_model();
            }
        }
    }

    DEF_TRAITSM(SpitterStone, "st_spitter", st_spitter, MOVABLE_PERSISTENT);

    
    BOOT_REGISTER_START
        BootRegister(new SpitterStone(), "st_spitter");
    BOOT_REGISTER_END

} // namespace enigma
