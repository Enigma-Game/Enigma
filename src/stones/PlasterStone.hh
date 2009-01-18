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
#ifndef PLASTERSTONE_HH
#define PLASTERSTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     */
    class PlasterStone : public Stone {
        CLONEOBJ(PlasterStone);
        DECL_TRAITS_ARRAY(4, traitsIdx());
    private:
        enum iState {
            IDLE,         ///< 
            BREAKING      ///< 
        };
        enum ObjectPrivatFlagsBits {
            OBJBIT_MOVABLE   =   1<<24,    ///<  
            OBJBIT_HOLLOW    =   1<<25,    ///< 
            OBJBIT_GLASSES   =   1<<26,    ///< invisible due to glasses
            OBJBIT_BREAKABLE =   1<<27     ///< breakable
        };
        
    public:
        PlasterStone(bool hollow, bool movable, bool breakable = false);
        
        // Object interface
        virtual std::string getClass() const;        
        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        virtual void processLight(Direction d);   // direction of laserbeam
        
        // ModelCallback interface
        virtual void animcb();
        
        // Stone interface
        virtual bool is_floating() const;
        virtual StoneResponse collision_response(const StoneContact &sc);
        virtual const char *collision_sound();
        virtual void actor_hit(const StoneContact &sc);

    private:
        void doBreak();
        int traitsIdx() const;
    };

} // namespace enigma

#endif
