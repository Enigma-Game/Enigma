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
#ifndef QUAKESTONE_HH
#define QUAKESTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     */
    class QuakeStone : public Stone {
        CLONEOBJ(QuakeStone);
        DECL_TRAITS;
    private:
        enum iState {
            IDLE,           ///< 
            ACTIVE,         ///< 
            BREAKING,       ///< 
            ACTIVEBREAKING  ///< active state with a pending breaking 
        };
        
    public:
        QuakeStone();
        
        // Object interface
        virtual std::string getClass() const;        
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual int maxState() const;
        virtual int externalState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        virtual void processLight(Direction d);
        
        // ModelCallback interface
        virtual void animcb();
        
        // Stone interface
        virtual void actor_hit(const StoneContact &sc);

    private:
        void quake();
    };

} // namespace enigma

#endif
