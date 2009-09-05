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
#ifndef BUGACTOR_HH
#define BUGACTOR_HH

#include "actors.hh"
#include "ecl_math.hh"

namespace enigma {

    /** 
     * Bug
     */
    class Bug : public Actor {
        CLONEACTOR(Bug);
        DECL_ACTORTRAITS;
 
   public:
        Bug();
        
        // Object interface.
        virtual std::string getClass() const;

        // Actor interface
        virtual bool is_dead() const;
    };

} // namespace enigma

#endif /*BUGACTOR_HH*/
