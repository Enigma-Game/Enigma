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
#ifndef SHOGUNDOTITEM_HH
#define SHOGUNDOTITEM_HH

#include "items.hh"

#include "enigma.hh"

namespace enigma {
    /**
     */
    class ShogunDot : public Item {
        CLONEOBJ(ShogunDot);
        DECL_ITEMTRAITS_ARRAY(3, traitsIdx());

    private:
        enum iState {
            OFF,     ///< inactive, no matching shogun stack on top
            ON       ///< active, matching shogun stack on top
        };
        
        enum ObjectPrivatFlagsBits {
            OBJBIT_HOLES =   127<<24,   ///< holes as defined in stones/ShogunStone.hh
        };
    public:
        ShogunDot(int holes);

        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const std::string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);

        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void on_creation(GridPos p);

        // Item interface
        virtual void stone_change(Stone *st);
        
    private:
        int getHoles() const;
        int requiredShogunHoles() const;
        int traitsIdx() const;
    };
    
} // namespace enigma

#endif
