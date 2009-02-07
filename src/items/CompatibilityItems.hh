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
#ifndef COMPATIBILITYITEMS_HH
#define COMPATIBILITYITEMS_HH

#include "items.hh"

#include "enigma.hh"

namespace enigma {
    /**
     * EasyKeepStone
     */
    class EasyKeepStone : public Item {
        CLONEOBJ(EasyKeepStone);
        DECL_ITEMTRAITS;
    public:
        EasyKeepStone();
        
        // Object interface
        virtual Value message(const Message &m);
    };

    /**
     * EasyKillStone
     */
    class EasyKillStone : public Item {
        CLONEOBJ(EasyKillStone);
        DECL_ITEMTRAITS;
    public:
        EasyKillStone();
        
        // Object interface
        virtual Value message(const Message &m);
    };

    /**
     * OnePKillStone
     */
    class OnePKillStone : public Item {
        CLONEOBJ (OnePKillStone);
        DECL_ITEMTRAITS;
    public:
        OnePKillStone();
        
        // Object interface
        virtual Value message(const Message &m);
    };

    /**
     * TwoPKillStone
     */
    class TwoPKillStone : public Item {
        CLONEOBJ (TwoPKillStone);
        DECL_ITEMTRAITS;
    public:
        TwoPKillStone();
        
        // Object interface
        virtual Value message(const Message &m);
    };

    /**
     * DummyItem
     */
    class DummyItem : public Item {
        CLONEOBJ(DummyItem);
        DECL_ITEMTRAITS;
    public:
        DummyItem();
        
        // Item interface
       virtual void on_drop(Actor *a);
       virtual void on_pickup(Actor *a);
    };


    /**
     * Odometer
     */
    DEF_ITEM(Odometer, "it_odometer", it_odometer);
    
    /**
     * OxydBridge
     */
    class OxydBridge : public Item {
        CLONEOBJ(OxydBridge);
        DECL_ITEMTRAITS;
    public:
        OxydBridge();
        
        // Object interface
        virtual Value message(const Message &m);
    };

    /**
     * OxydBridgeActive
     */
    class OxydBridgeActive : public OxydBridge {
        CLONEOBJ(OxydBridgeActive);
        DECL_ITEMTRAITS;
    public:
        OxydBridgeActive();
        
        // GridObject interface
        virtual void on_creation(GridPos p);
    };

} // namespace enigma

#endif
