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
#ifndef KILLERACTOR_HH
#define KILLERACTOR_HH

#include "actors.hh"

namespace enigma {

    /** 
     * Killer
     */
    class Killer : public Actor {
        CLONEACTOR(Killer);
        DECL_ACTORTRAITS;
 
   public:
        Killer();
        
        // Object interface.
        virtual std::string getClass() const;
        virtual const char *get_kind() const;
//        virtual void setAttr(const string& key, const Value &val);

        // Actor interface
        virtual bool is_dead() const;
        virtual bool has_shield() const;
        virtual bool on_collision(Actor *a);
    };

} // namespace enigma

#endif /*KILLERACTOR_HH*/
