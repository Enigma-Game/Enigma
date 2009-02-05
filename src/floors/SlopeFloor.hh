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
#ifndef SLOPEFLOOR_HH
#define SLOPEFLOOR_HH

#include "floors.hh"

namespace enigma {

    /** 
     * 
     */
    class SlopeFloor : public Floor {
        CLONEOBJ(SlopeFloor);
        
    private:
        enum ObjectPrivatFlagsBits {
            OBJBIT_STRENGTH =   1<<24,   ///< floor specific strength factor
            OBJBIT_FORCE    =   1<<25    ///< floor specific force vector
        };
    public:
        SlopeFloor(int gradient, std::string shape);

        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        
        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
                
        // Floor interface
        virtual void add_force(Actor *, ecl::V2 &);
    };

} // namespace enigma

#endif
