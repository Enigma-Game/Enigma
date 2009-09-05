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
#ifndef PIPEITEM_HH
#define PIPEITEM_HH

#include "items.hh"


namespace enigma {
    class PipeItem : public Item {
        CLONEOBJ(PipeItem);
        DECL_ITEMTRAITS_ARRAY(10, traitsIdx());

    public:
        static GridPos findPipeEndpoint(GridPos start, Direction dir);
    
        PipeItem(std::string connections);

        // Object interface
        virtual std::string getClass() const;
        
        // Item interface
        
    private:
        int traitsIdx() const;
    };
    
} // namespace enigma

#endif
