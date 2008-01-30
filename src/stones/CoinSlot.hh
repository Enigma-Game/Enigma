/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007,2008 Ronald Lamprecht
 * Copyright (C) 2008 Raoul Bourquin
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
#ifndef COINSLOT_HH
#define COINSLOT_HH

#include "stones.hh"
#include "stones_internal.hh"

namespace enigma {

    /** 
     * A simple 2 state timeswitch that can be toggled only by inserting coins
     * Each coin adds a certain time the switch stays on. The messages
     * "toggle", "signal", "on", "off" and get/set of the state are fully supported.
     * The switch starts per default in state OFF.<p>
     */
    // TODO: Fix bug when throw in a coin just before the remaining time ends.
    class CoinSlot : public Stone, public TimeHandler {
        CLONEOBJ(CoinSlot);
        DECL_TRAITS;
    private:
        enum iState { OFF, ON, TURNON };
        double remaining_time;

    public:
        CoinSlot();
        ~CoinSlot();

        // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();

        // ModelCallback interface
        virtual void animcb();

        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
        virtual const char *collision_sound();

        // TimeHandler interface
        virtual void tick(double dtime);

    private:
        void setIState(iState newIState);
    };

} // namespace enigma

#endif
