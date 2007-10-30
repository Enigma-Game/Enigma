/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
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
#include "server.hh"
#include "world.hh"

namespace enigma {
    DEF_TRAITSM(WindowStone, "st-window", st_window, MOVABLE_BREAKABLE);

    void WindowStone::actor_hit(const StoneContact &sc) {
        Actor *a = sc.actor;
        // TODO do we want to allow breaks while breaking?
        if (state == IDLE) {
            double impulse = -(a->get_vel() * sc.normal) * get_mass(a);
            if (impulse > 35) {
                SendMessage(a, "shatter");
            }

            else if (impulse > 25) {
                breakingFaces = sc.faces;
                set_attrib("connections", (get_connections() & ~breakingFaces) +1);
                sound_event ("shatter");
                state = BREAK;
                set_anim("st-window-anim");  // TODO anim with remaining unbroken faces
            }
            
            else if (player::WieldedItemIs (sc.actor, "it-wrench")) {
                DirectionBits faces = get_connections();
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
    
    void WindowStone::animcb() {
        DirectionBits faces = get_connections();
        DirectionBits newFaces = DirectionBits(faces & ~breakingFaces);
        if (newFaces == NODIRBIT)
            KillStone(get_pos());
        else {
            ReplaceStone(get_pos(), new WindowStone(newFaces+1));
        }
    }
    
    bool WindowStone::is_sticky(const Actor *a) const  {
        return false;
    }
    
    StoneResponse WindowStone::collision_response(const StoneContact &sc) {
        const double face_width = 3.0/32.0; 
        DirectionBits faces = get_connections();

        if (((sc.contact_point[0] <= get_pos().x + face_width) && faces&WESTBIT) ||
                ((sc.contact_point[0] >= get_pos().x + 1 - face_width) && faces&EASTBIT) ||
                ((sc.contact_point[1] <= get_pos().y + face_width) && faces&NORTHBIT) ||
                ((sc.contact_point[1] >= get_pos().y + 1 - face_width) && faces&SOUTHBIT)) {
            return STONE_REBOUND;
        } else {
            return STONE_PASS;
        }
    }
    
    Value WindowStone::message(const string &msg, const Value &val) {
        if (msg == "inner_pull" ) {
            return Value(tryInnerPull(to_direction(val)));
        }
        return Value();
    }
    
    bool WindowStone::tryInnerPull(Direction dir) {
        DirectionBits faces = get_connections();
        if (!has_dir(faces, dir) && has_dir(faces, reverse(dir))){
            Stone *stone = GetStone(move(get_pos(), dir));
            if (!stone || ((stone->get_traits().id == st_window) &&  
                    !has_dir(dynamic_cast<ConnectiveStone *>(stone)->get_connections(), reverse(dir)))) {
                ReplaceStone(get_pos(), new WindowStone((faces&(~to_bits(reverse(dir)))|to_bits(dir))+1));
                return true;
            }
        }
        return has_dir(faces, dir);
    }

    BOOT_REGISTER_START
        BootRegister(new WindowStone(3), "st-window");    // compatibility window with south face only
        BootRegister(new WindowStone(2), "st-window-w");
        BootRegister(new WindowStone(3), "st-window-s");
        BootRegister(new WindowStone(4), "st-window-sw");
        BootRegister(new WindowStone(5), "st-window-e");
        BootRegister(new WindowStone(6), "st-window-ew");
        BootRegister(new WindowStone(7), "st-window-es");
        BootRegister(new WindowStone(8), "st-window-esw");
        BootRegister(new WindowStone(9), "st-window-n");
        BootRegister(new WindowStone(10), "st-window-nw");
        BootRegister(new WindowStone(11), "st-window-ns");
        BootRegister(new WindowStone(12), "st-window-nsw");
        BootRegister(new WindowStone(13), "st-window-ne");
        BootRegister(new WindowStone(14), "st-window-new");
        BootRegister(new WindowStone(15), "st-window-nes");
        BootRegister(new WindowStone(16), "st-window-nesw");
    BOOT_REGISTER_END

} // namespace enigma
