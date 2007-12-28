/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
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

#include "GridObject.hh"

#include "errors.hh"
#include "game.hh"
#include "lua.hh"
#include "sound.hh"
#include "world.hh"

#include "ecl_util.hh"
#include "ecl_dict.hh"

#include <algorithm>
#include <string>
#include <cstdlib>
#include <cstdarg>
#include <iostream>
#include <iomanip>


using namespace std;

namespace enigma {

// remove comment from define below to switch on verbose messaging
// note: VERBOSE_MESSAGES is defined in multiple source files!
// #define VERBOSE_MESSAGES


/* -------------------- GridObject implementation -------------------- */

    void GridObject::setOwner(int player) {
        ASSERT(pos.x < 0, XLevelRuntime,
            "GridObject: attempt to add object to owner inventory that is still on grid");
        pos.x = -1;
        pos.y = player;
    }
    
    Value GridObject::getOwner() {
        if (pos.x == -1 && pos.y != -1)
            return Value(pos.y);
        else
            return Value();
    }
    
    void GridObject::setOwnerPos(GridPos po) {
        ASSERT(pos.x < 0, XLevelRuntime,
            "GridObject: attempt to add object to owner inventory that is still on grid");
        if (po.x >= 0) {
            pos.x = -2 - po.x;
            pos.y = -2 - po.y;
        } else {
            pos.x = po.x;
            pos.y = po.y;
        }
    }
    
    GridPos GridObject::getOwnerPos() {
        if (pos.x <= -2)
            return GridPos(-2 - pos.x, -2 - pos.y);
        else 
            return pos;
    }
    
    display::Model *GridObject::set_anim (const std::string &mname) 
    {
        set_model (mname);
        display::Model *m = get_model();
        m->set_callback(this);
        return m;
    }
    
    bool GridObject::sound_event (const char *name, double vol)
    {
        return sound::EmitSoundEvent (name, get_pos().center(), getVolume(name, this, vol));
    }
    
    void GridObject::warning(const char *format, ...) const 
    {
        va_list        arg_ptr;
        const GridPos& position = get_pos();
    
        va_start(arg_ptr, format);
    
        fprintf(stderr, "%p \"%s\" at %i/%i: ", this, get_kind(), position.x, position.y);
        vfprintf(stderr, format, arg_ptr);
        fputc('\n', stderr);
    
        va_end(arg_ptr);
    }
    
} // namespace enigma
