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
#ifndef STRIPITEM_HH
#define STRIPITEM_HH

#include "items.hh"

#include "enigma.hh"

namespace enigma {
    class StripItem : public Item {
        CLONEOBJ(StripItem);
        DECL_ITEMTRAITS_ARRAY(16, traitsIdx());

    public:
        static void setup();

        StripItem(std::string connections);

        // Object interface
        virtual std::string getClass() const;

        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual std::string getModelName() const;
        virtual void init_model();

        // Items interface
        virtual bool covers_floor(ecl::V2 position, Actor *a) const;
        virtual double getFriction(ecl::V2 position, double defaultFriction, Actor *a);
        virtual ecl::V2 calcMouseforce(Actor *a, ecl::V2 mouseForce, ecl::V2 floorForce);

    private:
        int traitsIdx() const;
    };

} // namespace enigma

#endif
