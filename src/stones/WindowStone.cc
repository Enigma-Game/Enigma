/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007, 2008 Ronald Lamprecht
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

#include "stones/WindowStone.hh"
#include "actors.hh"
#include "items.hh"
//#include "main.hh"
#include "server.hh"
#include "world.hh"
#include <vector>

namespace enigma {
    
    WindowStone::WindowStone(std::string faces) : Stone() {
        setAttr("faces", faces);
    }
    
    void WindowStone::setAttr(const std::string &key, const Value &val) {
        if (key == "secure") {
            if (val.to_bool() != (objFlags & OBJBIT_SECURE)) {
                if (val.to_bool()) {
                    objFlags |= OBJBIT_SECURE;
                } else {
                    objFlags &= ~OBJBIT_SECURE;
                }
                if (isDisplayable())
                    init_model();
                return;
            }
        }
        Stone::setAttr(key, val);
    }
    
    Value WindowStone::getAttr(const std::string &key) const {
        if (key == "secure") {
            return (bool)(objFlags & OBJBIT_SECURE);
        }
        return Stone::getAttr(key);
    }
    
    Value WindowStone::message(const Message &m) {
        if (m.message == "inner_pull" ) {
            return Value(tryInnerPull(to_direction(m.value)));
        } else if (m.message == "_explosion") {
            GridPos expl = m.value;
            DirectionBits faces = getFaces();
            DirectionBits scratchDirs = (DirectionBits)((objFlags & OBJBIT_SCRATCHDIRS) >> 24);
            if ((faces & WESTBIT) && ((move(get_pos(), WEST) == expl) || (get_pos() == expl)) 
                    && (!(objFlags & OBJBIT_SECURE) || (scratchDirs & WESTBIT)))
                breakFaces(WESTBIT);
            if ((faces & SOUTHBIT) && ((move(get_pos(), SOUTH) == expl) || (get_pos() == expl))
                    && (!(objFlags & OBJBIT_SECURE) || (scratchDirs & SOUTHBIT)))
                breakFaces(SOUTHBIT);
            if ((faces & EASTBIT) && ((move(get_pos(), EAST) == expl) || (get_pos() == expl))
                    && (!(objFlags & OBJBIT_SECURE) || (scratchDirs & EASTBIT)))
                breakFaces(EASTBIT);
            if ((faces & NORTHBIT) && ((move(get_pos(), NORTH) == expl) || (get_pos() == expl))
                    && (!(objFlags & OBJBIT_SECURE) || (scratchDirs & NORTHBIT)))
                breakFaces(NORTHBIT);
            
            return Value();
        }
        return Stone::message(m);
    }
    
    int WindowStone::externalState() const {
        return 0;
    }
    
    void WindowStone::setState(int extState) {
        // ignore any state access
    }

    void WindowStone::init_model() {
        if (state == IDLE) {   // anims will not be cleared 
            if (getFaces() == NODIRBIT) {   // a tease of level author tried to set a faceless window!
                KillStone(get_pos());
                return;
            }
            set_model(ecl::strf("st_window_%s%d", objFlags & OBJBIT_SECURE ? "green" : "blue" , getConnections()));
        }
    }

    void WindowStone::animcb() {
        if (state == FINALBREAK)
            KillStone(get_pos());
        else {
            state = IDLE;
            init_model();
        }
    }
    
    void WindowStone::actor_hit(const StoneContact &sc) {
        Actor *a = sc.actor;
        
        if (state == IDLE) {
            double impulse = -(a->get_vel() * sc.normal) * get_mass(a);
            double threshold = 22;
            if (player::WieldedItemIs (sc.actor, "it_hammer"))
                threshold -= 7;
            if (objFlags & (sc.faces << 24))  // face is scratched
                threshold -= 10;
            
            if (impulse > 35) {
                SendMessage(a, "shatter");
            }

            else if (!(objFlags & OBJBIT_SECURE) && impulse > threshold) {
                breakFaces(sc.faces);
            }
            
            else if (player::WieldedItemIs (sc.actor, "it_wrench")) {
                if (sc.faces == WESTBIT && sc.normal[0] < 0){
                    tryInnerPull(EAST);
                } else if (sc.faces == EASTBIT && sc.normal[0] > 0) {
                    tryInnerPull(WEST);
                } else if (sc.faces == SOUTHBIT && sc.normal[1] > 0) {
                    tryInnerPull(NORTH);
                } else if (sc.faces == NORTHBIT && sc.normal[1] < 0) {
                    tryInnerPull(SOUTH);
                }
            } else if (player::WieldedItemIs (sc.actor, "it-ring")) {
                objFlags |= (sc.faces << 24);   // scratch face
                sound_event("crack");
            }
        }
    }
    
    bool WindowStone::is_sticky(const Actor *a) const  {
        return false;
    }
    
    StoneResponse WindowStone::collision_response(const StoneContact &sc) {
        const double face_width = 3.0/32.0; 
        
        // blue windows let invisible actors pass
        if (!(objFlags & OBJBIT_SECURE) && sc.actor->is_invisible()) 
            return STONE_PASS;
            
        DirectionBits faces = getFaces();

        if (((sc.contact_point[0] <= get_pos().x + face_width) && faces&WESTBIT) ||
                ((sc.contact_point[0] >= get_pos().x + 1 - face_width) && faces&EASTBIT) ||
                ((sc.contact_point[1] <= get_pos().y + face_width) && faces&NORTHBIT) ||
                ((sc.contact_point[1] >= get_pos().y + 1 - face_width) && faces&SOUTHBIT)) {
            return STONE_REBOUND;
        } else {
            return STONE_PASS;
        }
    }
    
    void WindowStone::breakFaces(DirectionBits faces) {
        DirectionBits remainigFaces = (DirectionBits)(getFaces() & ~faces);  // remove breaking face
        objFlags &= ~(faces << 24);  // remove scratch mark from old position
        Object::setAttr("$connections", ALL_DIRECTIONS ^ remainigFaces);     // avoid init of model
        sound_event("shatter");
        state = (remainigFaces == NODIRBIT) ? FINALBREAK : BREAK;
        set_anim(ecl::strf("st_window_%s%d_anim",  objFlags & OBJBIT_SECURE ? "green" : "blue", getConnections()));
        if (server::GameCompatibility == GAMET_OXYD1)
            KillItem(get_pos());
    }
    
    bool WindowStone::tryInnerPull(Direction dir) {
        DirectionBits faces = getFaces();
        if (!has_dir(faces, dir) && has_dir(faces, reverse(dir))){
            Stone *stone = GetStone(move(get_pos(), dir));
            if (!stone || ((stone->get_traits().id == st_window) &&  
                    !has_dir(stone->getFaces(), reverse(dir)))) {
                DirectionBits remainigFaces = (DirectionBits)((faces & ~to_bits(reverse(dir)))
                        |to_bits(dir));  // move face
                Object::setAttr("$connections", ALL_DIRECTIONS ^ remainigFaces);     // avoid init of model
                init_model();
                // transfer scratches
                if (((objFlags & OBJBIT_SCRATCHDIRS) >> 24) & to_bits(reverse(dir)))
                    objFlags |= ((to_bits(dir)) << 24);       // mark moved face as scratched
                objFlags &= ~(to_bits(reverse(dir)) << 24);  // remove scratch mark from old position
                
                // move items
                Item *it = GetItem(get_pos());
                if (it != NULL && !it->isStatic())
                    SetItem(move(get_pos(), dir), YieldItem(get_pos()));
                
                // move actors
                std::vector<Actor*> found_actors;
                const double range_one_field = 1.415; // approx. 1 field [ > sqrt(1+1) ]
                GetActorsInRange(get_pos().center(), range_one_field, found_actors);
                std::vector<Actor*>::iterator e = found_actors.end();
                for (std::vector<Actor*>::iterator i = found_actors.begin(); i != e; ++i) {
                    Actor   *actor     = *i;
                    GridPos  actor_pos(actor->get_pos());
                    if (actor_pos == get_pos()) { // if the actor is in the field
                        V2 dest = actor->get_pos();
                        // we do not have to worry about the level border as no face can be pushed to the border
                        dest[0] += (dir == EAST) ? 1 : (dir == WEST) ? -1 : 0;
                        dest[1] += (dir == SOUTH) ? 1 : (dir == NORTH) ? -1 : 0;
                        WarpActor(actor, dest[0], dest[1], true);
                    }
                }
                
                return true;
            }
        }
        return false;
    }
    
    DEF_TRAITSM(WindowStone, "st_window", st_window, MOVABLE_BREAKABLE);

    BOOT_REGISTER_START
        BootRegister(new WindowStone("s"), "st_window");    // compatibility window with south face only
        BootRegister(new WindowStone("w"), "st_window_w");
        BootRegister(new WindowStone("s"), "st_window_s");
        BootRegister(new WindowStone("sw"), "st_window_sw");
        BootRegister(new WindowStone("e"), "st_window_e");
        BootRegister(new WindowStone("ew"), "st_window_ew");
        BootRegister(new WindowStone("es"), "st_window_es");
        BootRegister(new WindowStone("esw"), "st_window_esw");
        BootRegister(new WindowStone("n"), "st_window_n");
        BootRegister(new WindowStone("nw"), "st_window_nw");
        BootRegister(new WindowStone("ns"), "st_window_ns");
        BootRegister(new WindowStone("nsw"), "st_window_nsw");
        BootRegister(new WindowStone("ne"), "st_window_ne");
        BootRegister(new WindowStone("new"), "st_window_new");
        BootRegister(new WindowStone("nes"), "st_window_nes");
        BootRegister(new WindowStone("nesw"), "st_window_nesw");
    BOOT_REGISTER_END

} // namespace enigma
