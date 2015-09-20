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
#ifndef MONOFLOPSTONE_HH
#define MONOFLOPSTONE_HH

#include "stones.hh"
#include "timer.hh"

namespace enigma {

    /** 
     * 
     * 
     * Note that a monoflop has to continue to run even if it is removed from the grid
     * to support swapping. Thus the alarm has to removed on delete or destructor.
     */
    class MonoFlopStone : public Stone, public TimeHandler {
        CLONEOBJ(MonoFlopStone);
    private:
        enum iState {
            OFF,          ///< monoflop is inactive
            OFF_NEW,      ///< monoflop that is new and shoud start in off state (needed to handle swap/laser)
            ON_LASER,     ///< monoflop is permanent active due to light
            ON_TIMER,     ///< monoflop is touched or light just switched off and active until timer goes off
            ON_PENDING    ///< monoflop that should start on grid set
        };
        enum ObjectPrivatFlagsBits {
            OBJBIT_LASER   =  1<<24  ///< Object is laser light sensitive 
        };

    public:
        MonoFlopStone(bool lightsensitive, bool isOn);
        ~MonoFlopStone();

        // Object interface
        virtual std::string getClass() const;
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs);
        
        // Stone interface
        const char *collision_sound();
        void actor_hit(const StoneContact &sc);
        
        // TimeHandler interface
        virtual void alarm();

    private:
        void updateAlarm();
    };

} // namespace enigma

#endif
