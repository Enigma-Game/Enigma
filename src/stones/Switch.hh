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
#ifndef SWITCH_HH
#define SWITCH_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * A standard colored 2 state on/off switch that can be toggled only by actors
     * of a compatible color. It is a StateObject with 4 internal states as the
     * state changes are animated. The external states are mapped. The messages
     * "toggle", "signal", "on", "off" and get/set of the state are fully supported.
     * The switch starts per default in state OFF.<p>
     * Switches can be recolored by setting the "color" attribute. They can
     * send inverse action values by setting the "inverse" attribute.<p>
     * Note that the state changes per default not directly on messages and 
     * attribute settings. The actions will be performed after the turning 
     * animation finished. This is a small derivation from the inconsistent
     * 1.00 behaviour. Authors that need a switch that performs its action instantly
     * can set the attribute "instant" to true. But the usage of other switches
     * like "st_fourswitch", "st_floppy", "st_key" that provide an instant acton
     * per default should be preferred.
     */
    class Switch : public Stone {
        CLONEOBJ(Switch);
        DECL_TRAITS_ARRAY(6, traitsIdx());
    private:
        enum iState { OFF, ON, TURNON, TURNOFF };  // sequence is essential!
    public:
        Switch(Value color, bool instant = false);
        
        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const string& key, const Value &val);
        virtual Value message(const Message &m);
        
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
        
    private:
        int iColor() const;
        const char* colorName() const;
        bool isInstant() const;
        int traitsIdx() const;
    };

} // namespace enigma

#endif
