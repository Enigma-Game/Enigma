/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2008,2009,2010 Ronald Lamprecht
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
#ifndef HORSEACTOR_HH
#define HORSEACTOR_HH

#include "actors.hh"
#include "ecl_math.hh"

namespace enigma {

    /** 
     * Horse
     */
    class Horse : public Actor {
        CLONEACTOR(Horse);
        DECL_ACTORTRAITS;
 
    private:
        enum ObjectPrivatFlagsBits {
            OBJBIT_AUTOMOVE     =  1<<24,   ///< Actor is moved automatically 
            OBJBIT_NEWDEST      =  1<<25,   ///< Actor's destination or strength has changed
            OBJBIT_STEADY       =  1<<26    ///< Force on actor directs always to next destination
        };
   public:
        Horse();
        
        // Object interface.
        virtual std::string getClass() const;
        virtual Value getAttr(const std::string &key) const;
        virtual void setAttr(const std::string& key, const Value &val);
 
        // Actor interface
        virtual bool is_dead() const;
        virtual void think(double dtime);
        virtual void afterStoneBounce(const StoneContact &sc);

    private:
        // Variables
        double strength;
        Value destination;
        int destidx;
        ecl::V2 target;
        
        // Private methods
        void updateTarget(bool touched =false);
    };

} // namespace enigma

#endif /*HORSEACTOR_HH*/
