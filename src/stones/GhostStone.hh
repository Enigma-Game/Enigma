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
#ifndef GHOSTSTONE_HH
#define GHOSTSTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /**
     *
     */
    class GhostStone : public Stone {
        CLONEOBJ(GhostStone);
        DECL_TRAITS_ARRAY(3, traitsIdx());
    private:
        enum iState {
            PURPLEMARBLE,
            GREENBROWN,
            BREAK
        };
        
    public:
        GhostStone(int type);
        
        // Object interface
        virtual std::string getClass() const;        
        virtual Value getAttr(const std::string &key) const;
        
        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        
        // Stone interface
        virtual bool is_transparent(Direction d) const;
        virtual void actor_hit (const StoneContact &sc);
        virtual const char *collision_sound();

    private:
        int traitsIdx() const;
    };

} // namespace enigma

#endif
