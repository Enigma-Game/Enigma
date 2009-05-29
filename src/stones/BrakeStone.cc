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

#include "stones/BrakeStone.hh"
#include "player.hh"
#include "world.hh"
//#include "main.hh"

namespace enigma {
    BrakeStone::BrakeStone() {
    }
    
    std::string BrakeStone::getClass() const {
        return "st_brake";
    }
    
    Value BrakeStone::message(const Message &m) {
        if (m.message == "_explosion") {
            explode();
            return Value();
        }
        return Stone::message(m);
    }

    void BrakeStone::init_model() {
        set_model("st_brake");
    }
    
    void BrakeStone::on_creation (GridPos p) {
        Stone::on_creation(p);

        Item    *it = GetItem(p);
        if (it && it->is_kind("it_blocker")) {
            KillItem(p);
        }
    }
    
    void BrakeStone::processLight(Direction d) {
        explode();
    }

    bool BrakeStone::is_sticky(const Actor *a) const {
        return false;
    }
    
    StoneResponse BrakeStone::collision_response(const StoneContact &sc) {
        return STONE_PASS;
    }
    
    void BrakeStone::actor_inside(Actor *a) {
        static const double BRAKE_RADIUS = 0.3;
        GridPos p = get_pos();
        double dist = ecl::length(a->get_pos() - p.center());

        if (dist < BRAKE_RADIUS) {
            player::PickupStoneAsItem(a, p);
        }
    }
    
    bool BrakeStone::on_move(const GridPos &origin) {
        // we are not floating, but we do not shatter actors when swapped or pulled
        return true;
    }
    
    void BrakeStone::explode() {
        GridPos p = get_pos();
        KillStone(p);
        SetItem(p, MakeItem("it_explosion_nil"));
    }
            
    DEF_TRAITSM(BrakeStone, "st_brake", st_brake, MOVABLE_BREAKABLE);
    
    BOOT_REGISTER_START
        BootRegister(new BrakeStone(), "st_brake");
    BOOT_REGISTER_END

} // namespace enigma
