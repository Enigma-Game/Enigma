/*
 * Copyright (C) 2009 Ronald Lamprecht
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

#include "others/CannonBall.hh"
#include "errors.hh"
#include "floors.hh"
#include "items.hh"
#include "main.hh"
#include "stones.hh"
#include "world.hh"

namespace enigma {
    CannonBall::CannonBall() : Other(), pos (-1, -1) {
    }
    
    std::string CannonBall::getClass() const {
        return "ot_cannonball";
    }

    void CannonBall::animcb() {
        GridPos p(pos);
        if (Stone *st = GetStone(p)) {
            SendMessage (st, "_spitter");
        } else if (Item *it = GetItem(p)) {
            if (!has_flags(it, itf_indestructible))
                SetItem (p, MakeItem("it_explosion_debris"));
            else
                SendMessage (it, "_spitter");
        } else if (Floor *fl = GetFloor(p)) {
            if (fl->is_destructible())
                SetItem (p, MakeItem("it_explosion_debris"));
        }
        KillOther(this);
    }
    
    void CannonBall::postAddition() {
        pos = getAttr("$position");
        sprite = display::AddEffect(pos, "ac_cannonball");
        sprite.set_callback(this);
    }
    
    void CannonBall::preRemoval() {
        sprite.kill();
    }
    
    void CannonBall::tick(double dt) {
        pos += (ecl::V2)getAttr("$ball_velocity") * dt;
        sprite.move(pos);
    }

    BOOT_REGISTER_START
        BootRegister(new CannonBall(), "ot_cannonball");
    BOOT_REGISTER_END

} // namespace enigma
