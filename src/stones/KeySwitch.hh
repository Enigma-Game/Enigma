/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007,2008,2009 Ronald Lamprecht
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
#ifndef KEYSWITCH_HH
#define KEYSWITCH_HH

#include "stones.hh"
#include "stones_internal.hh"
#include "Inventory.hh"

namespace enigma {

    /** 
     * This stone acts as a lock and can only be activated by
     * using a key item. You can use keycodes to let keys only
     * open specific key stones.<p>
     * It is a StateObject with 2 internal states as the
     * state changes are not animated. The messages
     * "toggle", "signal", "on", "off" and get/set of the state are fully supported.
     * The keyswitch starts per default in state OFF.<p>
     * They can
     * send inverse action values by setting the "inverse" attribute.
     */
    class KeySwitch : public Stone {
        CLONEOBJ(KeySwitch);
        DECL_TRAITS;
    private:
        enum iState { OFF, ON };
    public:
        KeySwitch();

        // Object interface
        virtual void setAttr(const std::string& key, const Value &val);

        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();

        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
        virtual const char *collision_sound();
    private:
        bool check_matching_key(enigma::Inventory *inv);
    };

} // namespace enigma

#endif
