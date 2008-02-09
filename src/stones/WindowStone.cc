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
//#include "main.hh"
#include "server.hh"
#include "world.hh"

namespace enigma {
    WindowStone::WindowStone(std::string faces) : Stone(), breakingFaces(NODIRBIT) {
        setAttr("faces", faces);
    }
    
    Value WindowStone::message(const Message &m) {
        if (m.message == "inner_pull" ) {
            return Value(tryInnerPull(to_direction(m.value)));
        }
        return Stone::message(m);
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
        // TODO do we want to allow breaks while breaking?
        if (state == IDLE) {
            double impulse = -(a->get_vel() * sc.normal) * get_mass(a);
            if (impulse > 35) {
                SendMessage(a, "shatter");
            }

            else if (impulse > 20) {
                DirectionBits remainigFaces = (DirectionBits)(getFaces() & ~sc.faces);  // remove breaking face
                Object::setAttr("$connections", ALL_DIRECTIONS ^ remainigFaces);     // avoid init of model
                sound_event ("shatter");
                state = (remainigFaces == NODIRBIT) ? FINALBREAK : BREAK;
                set_anim("st_window_anim");  // TODO anim with remaining unbroken faces
            }
            
            else if (player::WieldedItemIs (sc.actor, "it-wrench")) {
                if (sc.faces == WESTBIT && sc.normal[0] < 0){
                    tryInnerPull(EAST);
                } else if (sc.faces == EASTBIT && sc.normal[0] > 0) {
                    tryInnerPull(WEST);
                } else if (sc.faces == SOUTHBIT && sc.normal[1] > 0) {
                    tryInnerPull(NORTH);
                } else if (sc.faces == NORTHBIT && sc.normal[1] < 0) {
                    tryInnerPull(SOUTH);
                }
            }
        }
    }
    
    bool WindowStone::is_sticky(const Actor *a) const  {
        return false;
    }
    
    StoneResponse WindowStone::collision_response(const StoneContact &sc) {
        const double face_width = 3.0/32.0; 
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
                return true;
            }
        }
        return has_dir(faces, dir);
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
