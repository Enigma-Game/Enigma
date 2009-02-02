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
#ifndef JAMBSTONE_HH
#define JAMBSTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     */
    class JambStone : public Stone {
         CLONEOBJ(JambStone);
         DECL_TRAITS;

    public:
        JambStone(int color);
        
        // Object interface
        virtual std::string getClass() const;
        virtual Value message(const Message &m);
            
        // GridObject interface
        virtual void init_model();
        
    private:
        // Private methods.
        int traitsIdx() const;
    };

} // namespace enigma

#endif /*JAMBSTONE_HH*/
