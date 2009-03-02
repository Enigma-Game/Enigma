/*
 * Copyright (C) 2009 Ronald Lamprecht
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
#ifndef THIEFSTONE_HH
#define THIEFSTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     */
    class ThiefStone : public Stone {
        CLONEOBJ(ThiefStone);
        DECL_TRAITS;
        
    private:
        enum iState {
            IDLE,             ///< 
            EMERGING,         ///< 
            RETREATING,       ///< 
            CAPTURE,          ///< thief is just being captured
            DRUNKEN,          ///<
            DRUNKENCAPTURE    ///< drunken thief is just being captured
        };
        
    public:
        ThiefStone();
        ~ThiefStone();
        
        // Object interface
        virtual std::string getClass() const;        
//        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        
        // ModelCallback interface
        virtual void animcb();
        
        // Stone interface
        virtual const char *collision_sound();
        virtual void actor_touch(const StoneContact &sc);
        virtual void actor_hit(const StoneContact &sc);

    private:
        int victimId;
        Item  *bag;
        
        void doSteal();
    };

} // namespace enigma

#endif
