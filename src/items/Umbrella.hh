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
#ifndef UMBRELLAITEM_HH
#define UMBRELLAITEM_HH

#include "items.hh"

#include "enigma.hh"

namespace enigma {
    /**
     */
    class Umbrella : public Item, public TimeHandler {
        CLONEOBJ(Umbrella);
        DECL_ITEMTRAITS;

    public:
        Umbrella(bool isNew);

        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs);

        // Item interface
        virtual ItemAction activate(Actor* a, GridPos p);
        
        // TimeHandler interface
        virtual void alarm();
    };
   
} // namespace enigma

#endif
