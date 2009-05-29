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
#ifndef SEEDITEM_HH
#define SEEDITEM_HH

#include "items.hh"

#include "enigma.hh"

namespace enigma {
    /**
     */
    class SeedItem : public Item {
        CLONEOBJ(SeedItem);
        DECL_ITEMTRAITS_ARRAY(4, traitsIdx());

    private:
        enum iState {
            IDLE,     ///< inactive
            ACTIVE    ///< activated
        };
        
        enum ObjectPrivatFlagsBits {
            OBJBIT_FLAVOR =   3<<24,   ///< wood, greenbrown, volcano, hay seed
        };
    public:
        SeedItem(int flavor);

        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        virtual void processLight(Direction d);
        
        // ModelCallback interface
        virtual void animcb();
        
        // ItemObject interface
        virtual std::string get_inventory_model();
        virtual bool isStatic() const;
        virtual void on_drop(Actor *a);
        virtual void on_stonehit(Stone *st);
        virtual bool actor_hit(Actor *a);

    private:
        void startGrowing();
        int traitsIdx() const;
    };
    
} // namespace enigma

#endif
