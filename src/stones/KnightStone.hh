/*
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
#ifndef KNIGHTSTONE_HH
#define KNIGHTSTONE_HH

#include "stones.hh"
#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     * 
     */
    class KnightStone : public Stone {
         CLONEOBJ(KnightStone);
         DECL_TRAITS;
    private:
        enum iState {
        };

    public:
        KnightStone();

        // Object interface
        virtual std::string getClass() const;
        
        // StateObject interface
        virtual int maxState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        
        // Stone interface
        virtual StoneResponse collision_response(const StoneContact &sc);
        virtual void actor_hit(const StoneContact &sc);
        virtual bool is_floating() const;
    };

} // namespace enigma

#endif
