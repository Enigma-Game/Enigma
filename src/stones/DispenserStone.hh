/*
 * Copyright (C) 2009 Ronald Lamprecht
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
#ifndef DISPENSERSTONE_HH
#define DISPENSERSTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /**
     *
     */
    class DispenserStone : public Stone {
        CLONEOBJ(DispenserStone);
        DECL_TRAITS_ARRAY(5, traitsIdx());

    private:
        enum iState {
            IDLE,       ///<
            BREAKING    ///<
        };

        enum ObjectPrivatFlagsBits {
            OBJBIT_SUBTYP    =   7<<24,   ///< the item typ
        };

        enum DispenserStoneTyp {
            BOMBBLACK,
            BOMBWHITE,
            DYNAMITE,
            EXTRALIFE,
            AXE
        };
    public:
        DispenserStone(int subtyp);

        // Object interface
        virtual std::string getClass() const;
        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);

        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();

        // ModelCallback interface
        virtual void animcb();

        // Stone interface
        virtual void actor_hit(const StoneContact &sc);

    private:
        // Private methods.
        void doBreak();
        int traitsIdx() const;
    };

} // namespace enigma

#endif /*DISPENSERSTONE_HH*/
