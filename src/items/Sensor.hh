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
#ifndef SENSORITEM_HH
#define SENSORITEM_HH

#include "items.hh"

#include "enigma.hh"

namespace enigma {
    class Sensor : public Item {
        CLONEOBJ(Sensor);
        DECL_ITEMTRAITS_ARRAY(5, traitsIdx());
    private:
        enum ObjectPrivatFlagsBits {
            OBJBIT_EXIT      =   1<<24,    ///< sensor that reacts on actor leave
            OBJBIT_ISFILTER  =   1<<25,    ///< sensor that filters signals, too
            OBJBIT_INVISIBLE =   1<<26     ///< sensor that is invisible
        };
    public:
        static void setup();
            
        Sensor(bool inverse, bool isFilter = false, bool exit = false);
        
        // Object interface
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual void setAttr(const string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        
        // GridObject interface
        virtual void init_model();
        virtual void actor_enter(Actor *a);
        virtual void actor_leave(Actor *a);

        // ModelCallback interface
        virtual void animcb();

    private:
        int traitsIdx() const;
    };
    
} // namespace enigma

#endif
