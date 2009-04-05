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
#ifndef TIMERGADGET_HH
#define TIMERGADGET_HH

#include "others/Other.hh"

/* -------------------- Other -------------------- */


namespace enigma {
    
    class TimerGadget : public Other, public TimeHandler {
        CLONEOBJ(TimerGadget)

    private:
        enum iState {
            OFF,          ///< timer is inactive, next action value is true (default)
            ON,           ///< timer is yet inactive, but will start running on being
            ON_TRUE,      ///< timer is active, next action value is true
            ON_FALSE      ///< timer is active, next action value is false
        };
        enum ObjectPrivatFlagsBits {
            OBJBIT_ADDED =   1<<24,    ///< 
        };
    public:
        TimerGadget();
        ~TimerGadget();

        // Object interface
        virtual std::string getClass() const;
        
        // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);

        // Other interface
        virtual void postAddition();
        virtual void preRemoval();

        // TimeHandler interface
        virtual void alarm();

    private:
        void updateAlarm();        
    };

} // namespace enigma

#endif
