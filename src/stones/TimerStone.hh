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
#ifndef TIMERSTONE_HH
#define TIMERSTONE_HH

#include "stones.hh"
#include "timer.hh"

namespace enigma {

    /** 
     * 
     * 
     * Note that a timer has to continue to run even if it is removed from the grid
     * to support swapping. Thus the alarm has to removed on delete or destructor.
     */
    class TimerStone : public Stone, public TimeHandler {
        CLONEOBJ(TimerStone);
    private:
        enum iState {
            OFF,          ///< timer is inactive, next action value is true (default)
            ON,           ///< timer is yet inactive, but will start running on being
            ON_TRUE,      ///< timer is active, next action value is true
            ON_FALSE      ///< timer is active, next action value is false
        };

    public:
        TimerStone();
        ~TimerStone();

        // Object interface
        virtual std::string getClass() const;
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        virtual void on_creation(GridPos p);
        
        // TimeHandler interface
        virtual void alarm();

    private:
        void updateAlarm();
    };

} // namespace enigma

#endif
