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
#ifndef ROTORACTORS_HH
#define ROTORACTORS_HH

#include "actors.hh"

namespace enigma {

    /** 
     * The base class for rotors and spinning tops
     */
    class RotorBase : public Actor {
        
    public:
        RotorBase(const ActorTraits &tr);

        // Object interface.
        virtual void setAttr(const string& key, const Value &val);
//        virtual Value getAttr(const std::string &key) const;
        
        // Actor interface
        virtual bool is_dead() const;
        virtual void think (double dtime);

    private:
        // Variables
        double range;
        double force;
        bool   gohome;
        bool   attacknearest;
        double prefercurrent;
        bool   attackCurrentOnly;
        double timeKeepAttackStrategy;
    };
    
/* -------------------- Rotor  -------------------- */
    class Rotor : public RotorBase {
        CLONEACTOR(Rotor);
        DECL_ACTORTRAITS;
        
    public:
        Rotor();
        
        // Object interface.
        virtual std::string getClass() const;
        virtual const char *get_kind() const;

        // Actor interface
        virtual bool on_collision(Actor *a);
    };
    
/* -------------------- Top  -------------------- */
    class Top : public RotorBase {
        CLONEACTOR(Top);
        DECL_ACTORTRAITS;
        
    public:
        Top();
        
        // Object interface.
        virtual std::string getClass() const;
        virtual const char *get_kind() const;

        // Actor interface
        virtual bool on_collision(Actor *a);
    };
    

} // namespace enigma

#endif /*ROTORACTORS_HH*/
