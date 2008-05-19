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
#ifndef FOURSWITCH_HH
#define FOURSWITCH_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * A switch that points to one of the four directions turning on every
     * switching event. It is a StateObject with 4 states that directly
     * represent the pointing direction. "toggle", "signal", and get/set of
     * the state are fully supported. The FourSwitch starts per default in
     * NORTH. The turning direction is clockwise per default and can be set
     * by the bool attribute "counterclock".<p>
     * Due to history and as very useful feature this object has two modes
     * of actions, a state dependent and a "global" state independent mode:
     * <p>
     * In the state dependent mode the state dependent target (target_0,...)
     * of the old state will receive an action value of "false" prior to
     * the turn. The new state will receive an action value of "true" after
     * the turn. This suits any connect signal receiver.<p>
     * But in the global mode just one action is send to the global "target"
     * with the direction representing value of the new state. This value
     * suits messages to objects like OneWayStone and BoulderStone. But it
     * works with signal messages to any StateObject as well, switching them
     * on and off in turn. To change the meaning of on and off you can set
     * the "inverse" attribute to "true".<p>
     * The Fourswitch is turned just on signal values of "1". Use "toggle"
     * messages to switch indepent of values.<p>
     * The state can be set anytime to any direction. Actions are performed
     * accordingly.<p>
     * Please note that due to compatibility just bool values are send to
     * global targets in the old API.
     */
    class FourSwitch : public Stone {
        CLONEOBJ(FourSwitch);
        DECL_TRAITS;
    public:
        FourSwitch();
        
        // Object interface
        virtual Value message(const Message &m);

        virtual Value invertActionValue(const Value &val) const;
        
        // StateObject interface
        virtual int maxState() const;
        virtual void toggleState();
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
                
        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
        virtual const char *collision_sound();
        
    private:
        bool m_inactive_so_far;  // TODO store flag in Object mask
        
        void turn(int num = 1, bool fixedClockwise = false);
    };

} // namespace enigma

#endif
