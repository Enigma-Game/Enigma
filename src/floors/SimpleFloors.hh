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
#ifndef SIMPLEFLOORS_HH
#define SIMPLEFLOORS_HH

#include "floors.hh"

namespace enigma {

    /** 
     * Abyss
     */
    class Abyss : public Floor {
        CLONEOBJ(Abyss);
        
    public:
        Abyss();

        // Object interface
        virtual std::string getClass() const;

        // Floor interface
        virtual bool is_destructible() const;
        virtual void actor_contact(Actor *);
    };

    /** 
     * IceFloor
     */
    class IceFloor : public Floor {
        CLONEOBJ(IceFloor);
        
    public:
        IceFloor();

        // Object interface
        virtual std::string getClass() const;
    };

    /** 
     * Space
     */
    class Space : public Floor {
        CLONEOBJ(Space);

    private:
        enum ObjectPrivatFlagsBits {
            OBJBIT_FORCETYP  =   1<<24   ///< apply flat force on space
        };
        
    public:
        Space(bool withForce = false);
 
        // Object interface
        virtual std::string getClass() const;

        // Floor interface
        virtual void add_force(Actor *a, ecl::V2 &);  // Note: actor = 0 must be allowed!
    };

    /** 
     * Swamp
     */
    class Swamp : public Floor {
        CLONEOBJ(Swamp);
        
    public:
        Swamp();

        // Object interface
        virtual std::string getClass() const;

        // Floor interface
        virtual bool is_destructible() const;
        virtual void get_sink_speed (double &sinkspeed, double &raisespeed) const;        
    };

    /** 
     * Yinyang
     */
    class YinyangFloor : public Floor {
        CLONEOBJ(YinyangFloor);
        
    private:
        enum iState {
            YIN,       ///< player 0 - black image
            YANG       ///< player 1 - white image
        };

    public:
        YinyangFloor(int initState);

        // Object interface
        virtual std::string getClass() const;
        
        // GridObject interface
        virtual void init_model();
                
        // Floor interface
        virtual ecl::V2 process_mouseforce (Actor *a, ecl::V2 force);
    };
} // namespace enigma

#endif
