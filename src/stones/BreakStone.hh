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
#ifndef BREAKSTONE_HH
#define BREAKSTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     */
    class BreakStone : public Stone {
        CLONEOBJ(BreakStone);
        DECL_TRAITS_ARRAY(7, traitsIdx());
    private:
        enum iState {
            IDLE,         ///< 
            BREAKING      ///< 
        };

        enum BreakStoneTyp {
            PLAIN = 0,
            ACBLACK,
            ACWHITE,
            OXYDC,
            BOULDER,
            BUG,
            LASER
        };

        enum ObjectPrivatFlagsBits {
            OBJBIT_SUBTYP   =   15<<24,    ///<  
        };
        
    public:
        BreakStone(int type);
        
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
        virtual void actor_hit(const StoneContact &sc);
        virtual const char *collision_sound();

    private:
        int typ() const;
        void doBreak();
        int traitsIdx() const;
    };

} // namespace enigma

#endif
