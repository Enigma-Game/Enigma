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
 */
#ifndef INKWELLSTONE_HH
#define INKWELLSTONE_HH

#include "stones.hh"
#include "stones_internal.hh"

namespace enigma {

    class InkwellStone : public Stone {
        CLONEOBJ(InkwellStone);
        DECL_TRAITS;
        
    private:
        enum iState {
            BLACK_INK,
            WHITE_INK,
            EMPTY_WELL,
            BLACK_TO_EMPTY,
            EMPTY_TO_WHITE,
            WHITE_TO_EMPTY,
            EMPTY_TO_BLACK,
        };

    public:
        InkwellStone(int initState);
        
         // Object interface
        virtual std::string getClass() const;        
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual int maxState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        
        // ModelCallback interface  - Animation callback
        virtual void animcb();

        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
    };

} // namespace enigma

#endif
