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
#ifndef VORTEXITEM_HH
#define VORTEXITEM_HH

#include "items.hh"

#include "enigma.hh"

namespace enigma {
    class Vortex : public Item, public TimeHandler {
    private:
        enum iState {
            OPEN,          ///< TODO exchange OPEN, CLOSED to match external state 
            CLOSED,        ///<
            OPENING,       ///<
            CLOSING,       ///<
            WARPING,       ///< an open vortex occupied in process of sending a marble
            EMITTING,      ///< an open vortex occupied, waiting for the destination to open
            SWALLOWING     ///< an open vortex eats up a marble
        };

    public:
        CLONEOBJ(Vortex);
        DECL_ITEMTRAITS_ARRAY(2, is_open());
        
        Vortex(bool opened);
        virtual ~Vortex();

        // Object interface
        virtual std::string getClass() const;
        virtual Value message(const Message &m);

        // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);
        virtual void toggleState();

        // GridObject interface
        virtual void init_model();
        virtual void on_removal(GridPos p);
        
        // ModelCallback interface
        virtual void animcb();

        // Item interface
        virtual bool covers_floor(ecl::V2 pos, Actor *a) const;
        virtual bool actor_hit(Actor*);
        virtual std::list<GridPos> warpSpreadPos(bool isWater);

        // TimeHandler interface
        virtual void alarm();

    private:
        void prepare_for_warp(Actor *actor);
        void emit_actor(Vortex *destVortex);

        void perform_warp();    // warp swallowed actor(s)
        void warp_to(const ecl::V2 &target);

        bool is_open() const { return externalState() == 1 ; }

    };
    
} // namespace enigma

#endif
