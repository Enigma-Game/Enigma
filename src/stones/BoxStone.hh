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
#ifndef BOXSTONE_HH
#define BOXSTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     */
    class BoxStone : public Stone {
         DECL_TRAITS_ARRAY(2, traitsIdx());

    private:
        enum iState {
            IDLE,       ///< standard movable stone
            GROWING,    ///< a stone growing from a seed
            FALLING,    ///< a stone moved into water or abyss and marked to fall on floor change notification
            FALLEN,     ///< a stone that is fallen on floor change notification and is vanishing
            BREAKING    ///< breaking by it_axe
        };

        enum ObjectPrivatFlagsBits {
            OBJBIT_BLOCKFIRE =   1<<24,   ///< stone blocks fire on moves and floor changes beneath
            OBJBIT_SUBTYP    =   7<<25,   ///< the BoxStoneTyp
        };

        enum BoxStoneTyp {
            WOOD = 0,
            WOOD1,
            WOOD2,
            HAY,
            ROCK
        };
    public:
        BoxStone(int subtyp, int initState = IDLE);

        // Object interface
        virtual BoxStone* clone();
        virtual void dispose();
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
        virtual bool allowsSpreading(Direction dir, bool isFlood = false) const;
        virtual void actor_hit(const StoneContact &sc);
        virtual void actor_inside(Actor *a);
        virtual void actor_contact(Actor *a);
        virtual void doBreak();
        virtual bool on_move(const GridPos &origin);
        virtual void on_floor_change();

    private:
        // Private methods.
        void maybe_fall_or_stopfire(bool onMove = false, bool onFloorChange = false);
        int traitsIdx() const;
    };

} // namespace enigma

#endif /*BOXSTONE_HH*/
