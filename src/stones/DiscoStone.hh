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
#ifndef DISCOSTONE_HH
#define DISCOSTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     */
    class DiscoStone : public Stone {
        CLONEOBJ(DiscoStone);
        DECL_TRAITS;
        
    private:
        enum iState {
            LIGHT,   ///< 
            MEDIUM,  ///< 
            DARK     ///< 
        };

    public:
        DiscoStone(int initState);
        
        // Object interface
        virtual std::string getClass() const;
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual int maxState() const;

        // GridObject interface
        virtual void init_model();
        
        // Stone interface
        virtual bool is_floating() const;
        virtual StoneResponse collision_response(const StoneContact &sc);
        
    private:
        std::string model_basename();
        void set_iState(int newState);
    };

} // namespace enigma

#endif
