/*
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
#ifndef SHOGUNSTONE_HH
#define SHOGUNSTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     */
    class ShogunStone : public Stone {
        DECL_TRAITS;
        
    public:
        enum Hole {
            N         =  1,   ///<  Nano - not yet existing
            T         =  2,   ///<  Tiny - not yet existing
            S         =  4,   ///<  Small
            M         =  8,   ///<  Medium
            L         = 16,   ///<  Large
            G         = 32,   ///<  Giant
            U         = 64    ///<  Universal
        };

        ShogunStone(int initState);
        ShogunStone* clone();
        void dispose();
        
        // Object interface
        virtual std::string getClass() const;
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        virtual void setOwnerPos(GridPos po);
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        
        // Stone interface
        virtual void on_impulse(const Impulse& impulse);
        virtual FreezeStatusBits get_freeze_bits();
        
    private:
        ShogunStone *superShogun;
        ShogunStone *subShogun;
        
        int ownHole();
        bool yieldShogun();
    };

} // namespace enigma

#endif
