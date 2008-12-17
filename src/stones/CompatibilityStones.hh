/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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
#ifndef COMPATIBILITYSTONES_HH
#define COMPATIBILITYSTONES_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     */
    class Peroxyd_0xb8 : public Stone {
        CLONEOBJ(Peroxyd_0xb8);
        DECL_TRAITS;
    public:
        Peroxyd_0xb8();

        // GridObject interface
        virtual void on_creation(GridPos p);
    };

    class Peroxyd_0xb9 : public Stone {
        CLONEOBJ(Peroxyd_0xb9);
        DECL_TRAITS;
    public:
        Peroxyd_0xb9();

        // GridObject interface
        virtual void on_creation(GridPos p);
    };

    class Oxyd_0x18 : public Stone {
        CLONEOBJ(Oxyd_0x18);
        DECL_TRAITS;
    public:
        Oxyd_0x18();
        
        // GridObject interface
        virtual void on_creation(GridPos p);
    };

} // namespace enigma

#endif /*COMPATIBILITYSTONES_HH*/
