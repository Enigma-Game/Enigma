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
 */
#ifndef LASERSTONE_HH
#define LASERSTONE_HH

#include "laser.hh"
#include "timer.hh"
#include <list>

namespace enigma {

    /** 
     * 
     * 
     * When a laser is switched on or off this may directly cause an action. 
     * A part of such an action can be another laser switching of the same laser.
     * To break infinit loops and to avoid laser flickering at a frequency that
     * the user cannot recognize laser switching is limited. After a switching
     * a laser stays in a NEW* state where further switch requests are delayed.
     */
    class LaserStone : public Stone, public TimeHandler {
    private:
        enum iState {
            OFF, 
            ON, 
            NEW_OFF,
            NEW_ON,
            NEW_OFF_PENDING_ON,
            NEW_ON_PENDING_OFF
        };
        typedef std::list<LaserStone*> InstanceList;
        static InstanceList instances;

    public:
        static void reemit_all();

        LaserStone(Direction dir=EAST);

        // Object interface
        virtual std::string getClass() const;
        virtual LaserStone *clone();
        virtual void dispose();
        virtual void setAttr(const std::string& key, const Value &val);
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);
        virtual void toggleState();

        // GridObject interface
        virtual void init_model();
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual DirectionBits emissionDirections() const;
        
        // TimeHandler interface
        virtual void alarm();

        
    private:
        // Private methods.
        void emit_light();
        Direction getOrientation() const;
    };

} // namespace enigma

#endif
