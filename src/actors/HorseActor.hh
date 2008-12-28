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
 
   public:
        Horse();
        
        // Object interface.
        virtual std::string getClass() const;
        virtual const char *get_kind() const;
//        virtual void setAttr(const string& key, const Value &val);

        // Actor interface
        virtual bool is_dead() const;
        virtual void think (double dtime);

    private:
        // Variables
        int m_targetidx;
        ecl::V2 m_target;
        
        // Private methods
        void update_target();
    };

} // namespace enigma

#endif /*HORSEACTOR_HH*/
