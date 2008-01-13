/*
 * Copyright (C) 2007 Ronald Lamprecht
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
#ifndef FLOPPYSWITCH_HH
#define FLOPPYSWITCH_HH

#include "stones.hh"
#include "stones_internal.hh"

namespace enigma {

    /** 
     * A simple 2 state on/off switch that can be toggled only by inserting
     * a floppy disk. It is a StateObject with 2 internal states as the
     * state changes are not (yet) animated. The messages
     * "toggle", "signal", "on", "off" and get/set of the state are fully supported.
     * The floppyswitch starts per default in state OFF.<p>
     * Floppyswitches can send inverse action values by setting the "inverse"
     * attribute.
     */
    class FloppySwitch : public Stone {
        CLONEOBJ(FloppySwitch);
        DECL_TRAITS;
    private:
        enum iState { OFF, ON };
    public:
        FloppySwitch();

        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();

        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
        virtual const char *collision_sound();
    };

} // namespace enigma

#endif
