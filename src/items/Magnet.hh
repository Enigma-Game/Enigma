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
#ifndef MAGNETITEM_HH
#define MAGNETITEM_HH

#include "items.hh"

#include "enigma.hh"
#include "world.hh"

namespace enigma {
    class Magnet : public Item, public ForceField {
    private:
        enum iState {
            OFF,
            ON
        };
       
    public:
        CLONEOBJ(Magnet);
        DECL_ITEMTRAITS_ARRAY(2, state);

        Magnet(bool isOn);
        
        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const std::string &key, const Value &val);
        virtual Value message(const Message &m);

        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void init_model();

        // ForceField interface
        virtual void add_force(Actor *a, V2 &f);
        
    private:
        double correctedStrength;     // 0.6 * strength
        double squareRange;
    };
    
} // namespace enigma

#endif
