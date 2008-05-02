/*
 * Copyright (C) 2006 Andreas Lochmann
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
#ifndef LIGHTPASSENGERSTONE_HH
#define LIGHTPASSENGERSTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     */
    class LightPassengerStone : public Stone, public TimeHandler {
        CLONEOBJ(LightPassengerStone);
        DECL_TRAITS;
    private:
        enum iState {
            OFF,     ///< inactive, does not react on laser 
            ON,      ///< active, does react on laser
            BLINK,   ///< active, double lasered, ready to break
            BREAK    ///< active, breaking and dissolving
        };
        
        enum ObjectPrivatFlagsBits {
            OBJBIT_SKATEDIR =   7<<24,   ///< current skate direction
            OBJBIT_BLOCKED  =   1<<27    ///< have been blocked by a stone on last move attempt 
        };
    public:
        LightPassengerStone(bool isActive);
        virtual ~LightPassengerStone();
        
        // Object interface
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs);
        
        // ModelCallback interface
        virtual void animcb();
        
        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
        virtual void on_impulse(const Impulse& impulse);

        // TimeHandler interface
        virtual void alarm();
        
    private:
        double calcInterval();
    };

} // namespace enigma

#endif
