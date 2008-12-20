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
#ifndef SIMPLEITEMS_HH
#define SIMPLEITEMS_HH

#include "items.hh"

#include "enigma.hh"

namespace enigma {
    /**
     * Banana
     */
    class Banana : public Item {
        CLONEOBJ(Banana);
        DECL_ITEMTRAITS;

    public:
        Banana();

        // GridObject interface
        virtual void processLight(Direction d);   // direction of laserbeam
        
        // Item interface
        virtual void on_stonehit(Stone *st);
    };

    /**
     * Brush Can "paint" some stones and remove ash
     */
    class Brush : public Item {
        CLONEOBJ(Brush);
        DECL_ITEMTRAITS;

    public:
        Brush();
        
        // Item interface
        virtual ItemAction activate(Actor* a, GridPos p);
    };
    

    /**
     * Cherry
     */

    class Cherry : public Item {
        CLONEOBJ(Cherry);
        DECL_ITEMTRAITS;
        
    public:
        Cherry();

        // Item interface
        virtual ItemAction activate(Actor* a, GridPos p);
        virtual void on_stonehit(Stone *st);
    };
    
    /**
     * DeathItem
     */
    class DeathItem : public Item {
        CLONEOBJ(DeathItem);
        DECL_ITEMTRAITS;
        
    public:
        DeathItem();
        
        // ModelCallback interface
        virtual void animcb();

        // Item interface
        virtual bool actor_hit(Actor *a);
        
    };

    /**
     * Floppy
     */
    DEF_ITEM(Floppy, "it_floppy", it_floppy);

    /**
     * MagicWand
     */
    DEF_ITEM(MagicWand, "it_magicwand", it_magicwand);

    /**
     * Key
     */
    DEF_ITEM(Key, "it_key", it_key);

    /**
     * Squashed
     */
    class Squashed : public Item {
        CLONEOBJ(Squashed);
        DECL_ITEMTRAITS;

    public:
        Squashed();
        
        // Object interface
        virtual Value message(const Message &m);
    };

    /**
     * Wrench
     */
    DEF_ITEM(Wrench, "it_wrench", it_wrench);

} // namespace enigma

#endif
