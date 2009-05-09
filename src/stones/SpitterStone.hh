/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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
#ifndef SPITTERSTONE_HH
#define SPITTERSTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * SpitterStone
     */
    class SpitterStone : public Stone {
        CLONEOBJ(SpitterStone);
        DECL_TRAITS;
    private:
        enum iState {
            IDLE,
            ACTIVE,
            LOADING,
            SPITTING,
            BREAKING
        };

    public:
        SpitterStone ();
        
        // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);
        virtual Value message(const Message &m);

        // GridObject interface
        virtual void init_model();
        
        // ModelCallback interface  - Animation callback
        virtual void animcb();

        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
    };

} // namespace enigma

#endif
