/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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
#ifndef MEDITATIONITEM_HH
#define MEDITATIONITEM_HH

#include "items.hh"

#include "actors.hh"
#include "stones.hh"

namespace enigma {
    class Meditation : public Item {
        CLONEOBJ(Meditation);
        DECL_ITEMTRAITS_ARRAY(6, traitsIdx());

        enum iState {
            CALDERA = -3,     ///< a large deepening with a small hill in the middle
            HOLLOW  = -2,     ///< a large deepening
            DENT    = -1,     ///< a small deepening
            BUMP    =  1,     ///< a small elevation
            HILL    =  2,     ///< a large elevation
            VOLCANO =  3      ///< a large elevation with a small dent in the middle
        };

        enum ObjectPrivatFlagsBits {
            OBJBIT_INDISPENSIBLE =   1<<24,   ///< 
        };
    public:                
        Meditation(int initState);
        
        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);

        // StateObject interface
        virtual int minState() const;
        virtual int maxState() const;
        virtual void toggleState();
        virtual void setState(int extState);
        
        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void actor_leave(Actor *a);
        
        // Items interface
        virtual void on_stonehit(Stone *st);
        virtual bool actor_hit(Actor *a);
        virtual void add_force(Actor *a, ecl::V2 &f);
        
    private:
        // Variables.
        Actor *whiteball;   // The small white ball that is currently being tracked
        double enter_time;  // ... when did it enter the hollow?

        bool isMeditating(Actor *a);
        void shovel();
        void checkActors();
        void deregisterWhiteball();
        int traitsIdx() const;
    };
    
} // namespace enigma

#endif
