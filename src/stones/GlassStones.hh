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
#ifndef GLASSSTONE_HH
#define GLASSSTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     */
    class GlassStone : public Stone {
        CLONEOBJ(GlassStone);
        DECL_TRAITS_ARRAY(8, traitsIdx());
    private:
        enum iState {
            RAW,          ///< 
            RAW_MOVABLE,  ///<
            LIGHT,
            LIGHT_MOVABLE,
            DARK,
            DARK_MOVABLE,
            RED,
            BEADS
        };
        enum ObjectPrivatFlagsBits {
            OBJBIT_MOVABLE  =   1<<24,    ///<  
            OBJBIT_HOLLOW   =   1<<25,    ///< 
            OBJBIT_GLASSES  =   1<<26,    ///< invisible due to glasses
            OBJBIT_CLOTH    =   1<<27     ///< collision sound cloth instead of standard stone
        };
        
    public:
        GlassStone(int type, bool hollow, bool movable, bool cloth = false);
        
        // Object interface
        virtual std::string getClass() const;        
        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);
        
        // StateObject interface
//        virtual int maxState() const;
//        virtual int externalState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        
        // Stone interface
        virtual bool is_floating() const;
        virtual bool is_sticky(const Actor *a) const;
        virtual bool is_transparent(Direction d) const;
        virtual StoneResponse collision_response(const StoneContact &sc);
        virtual const char *collision_sound();

    private:
        int traitsIdx() const;
    };

} // namespace enigma

#endif
