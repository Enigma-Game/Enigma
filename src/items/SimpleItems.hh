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
        virtual void on_drop(Actor *a);
    };
    
    /**
     * Coffee
     */
    DEF_ITEMF(Coffee, "it_coffee", it_coffee, itf_inflammable);

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
     * Debris
     */
    class Debris : public Item {
        CLONEOBJ(Debris);
        DECL_ITEMTRAITS;

    public:
        Debris();
        
        // ModelCallback interface
        virtual void animcb();

        // Item interface
        virtual bool actor_hit(Actor *a);
    };

    /**
     * Explosion
     */
    class Explosion : public Item {
        CLONEOBJ(Explosion);
        DECL_ITEMTRAITS_ARRAY(4, state);

    public:
        Explosion(int strength);
        
        // Object interface
        virtual std::string getClass() const;
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual void setState(int extState);
        
        // GridObject interface
        virtual void init_model();
        
        // ModelCallback interface
        virtual void animcb();
        
        // Item interface
        virtual bool actor_hit(Actor *a);
    };
    
    /**
     * FlagItem
     */
    class FlagItem : public Item {
        CLONEOBJ(FlagItem);
        DECL_ITEMTRAITS_ARRAY(2, traitsIdx());
    public:
        FlagItem(int type);
        
        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const string& key, const Value &val);
        
        // Item interface
        virtual void on_drop(Actor *a);
        virtual void on_pickup(Actor *a);
        
    private:
        int traitsIdx() const;
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
     * Pencil
     */
    class Pencil : public Item {
        CLONEOBJ(Pencil);
        DECL_ITEMTRAITS;

    public:
        Pencil();
        
        // Item interface
        virtual ItemAction activate(Actor* a, GridPos p);
    };


    /**
     * Pin
     */
    class Pin : public Item {
        CLONEOBJ(Pin);
        DECL_ITEMTRAITS;
    public:
        Pin();
        
        // GridObject interface
        virtual void setOwner(int player);
    };

    /**
     * Ring
     */
    class Ring : public Item {
        CLONEOBJ(Ring);
        DECL_ITEMTRAITS;
        
    public:
        Ring();
        
        // Item interface
        virtual ItemAction activate(Actor* a, GridPos p);
    };

    /**
     * Spade
     */
    class Spade : public Item {
        CLONEOBJ(Spade);
        DECL_ITEMTRAITS;

    public:
        Spade();
        
        // Item interface
        virtual ItemAction activate(Actor* a, GridPos p);
    };
    
    /**
     * Spoon
     */
    class Spoon : public Item {
        CLONEOBJ(Spoon);
        DECL_ITEMTRAITS;

    public:
        Spoon();
        
        // Item interface
        virtual ItemAction activate(Actor* a, GridPos p);
    };

    /**
     * Spring
     */
    class Spring : public Item {
        CLONEOBJ(Spring);
        DECL_ITEMTRAITS_ARRAY(2, traitsIdx());
    private:
        enum iState {
            KEEP     =  0,    ///< 
            DROP     =  1     ///< 
        };
    public:
        Spring(int type);
        
        // Object interface
        virtual std::string getClass() const;

        // StateObject interface
        virtual void setState(int extState);

        // Item interface
        virtual ItemAction activate(Actor* a, GridPos p);
        
    private:
        int traitsIdx() const;
    };
    
    /**
     * Springboard
     */
    class Springboard : public Item {
        CLONEOBJ(Springboard);
        DECL_ITEMTRAITS;

    public:
        Springboard();
        
        // ModelCallback interface
        virtual void animcb();
        
        // ItemObject interface
        virtual bool actor_hit(Actor *a);
    };

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
     * Weight
     */
    class Weight : public Item {
        CLONEOBJ(Weight);
        DECL_ITEMTRAITS;
    public:
        Weight();
        
        // Object interface
        virtual void setAttr(const string& key, const Value &val);
        
        // GridObject interface
        virtual void setOwner(int player);

        // Item interface
        virtual ItemAction activate(Actor* a, GridPos p);
    };

    /**
     * Wrench
     */
    DEF_ITEM(Wrench, "it_wrench", it_wrench);

    /**
     * Yinyang
     */
    class Yinyang : public Item {
        CLONEOBJ(Yinyang);
        DECL_ITEMTRAITS;
        
    public:
        Yinyang();

        // Item interface
        virtual std::string get_inventory_model();
        virtual ItemAction activate(Actor* a, GridPos p);
    };
} // namespace enigma

#endif
