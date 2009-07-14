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
#ifndef RUBBERBAND_HH_INCLUDED
#define RUBBERBAND_HH_INCLUDED

#include "others/Other.hh"
#include "actors.hh"
#include "display.hh"
#include "stones.hh"

/* -------------------- Other -------------------- */


namespace enigma {
    
    class Rubberband : public Other {
        CLONEOBJ(Rubberband)

        enum ObjectPrivatFlagsBits {
            OBJBIT_STONE        =  1<<24,   ///< anchor2 is a stone
            OBJBIT_MAXVIOLATION =  1<<25,   ///< max may be not fulfilled due to extraordinary events
            OBJBIT_MINVIOLATION =  1<<26    ///< min may be not fulfilled due to extraordinary events
        };

    public:
        Rubberband();
        
        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const std::string &key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);
        
        // Other interface
        virtual void postAddition();
        virtual void preRemoval();
        virtual void tick(double dt);
        
        void applyForces(double dt);
        
    private:
        Actor * anchor1;
        union {
            Actor * ac;
            Stone * st;
        } anchor2;
        double strength;
        double outerThreshold;
        double innerThreshold;
        double minLength;
        double maxLength;
        display::RubberHandle model;
        
        Object * anchor2Object() const;
        ecl::V2 posAnchor2() const;
        void switchAnchor(Object *oldAnchor, Object *newAnchor, Object *otherAnchor);
    };

} // namespace enigma

#endif
