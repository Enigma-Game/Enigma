/*
 * Copyright (C) 2007,2008,2009 Ronald Lamprecht
 * Copyright (C) 2008 Andreas Lochmann
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
#ifndef BARRIERSTONE_HH_INCLUDED
#define BARRIERSTONE_HH_INCLUDED

#include "stones_internal.hh"

namespace enigma {
    
    /**
     * st_barrier Barrier Stone
     * 
     * A marble may pass BarrierStone only if a certain item is
     * not in its inventory. The type of the item is saved in
     * the "flavor" attribute.
     */
    class BarrierStone : public Stone {
        CLONEOBJ(BarrierStone);
        DECL_TRAITS;
    public:
        BarrierStone();
        
        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const std::string &key, const Value &val);
       
        // GridObject interface
        virtual void init_model();

        // Stone interface
        virtual bool is_removable() const;
        virtual bool is_sticky(const Actor *) const;
        virtual StoneResponse collision_response(const StoneContact &sc);
    };

} // namespace enigma

#endif
