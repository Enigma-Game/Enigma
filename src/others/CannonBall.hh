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
#ifndef CANNONBALL_HH
#define CANNONBALL_HH

#include "others/Other.hh"
#include "display.hh"

/* -------------------- Other -------------------- */


namespace enigma {
    
    class CannonBall : public Other, public display::ModelCallback {
        CLONEOBJ(CannonBall)

    public:
        CannonBall();
        
        // Object interface
        virtual std::string getClass() const;
        
        // ModelCallback interface  - Animation callback
        virtual void animcb();

        // Other interface
        virtual void postAddition();
        virtual void preRemoval();
        virtual void tick(double dt);
        
    private:
        display::SpriteHandle sprite;
        ecl::V2 pos;
    };

} // namespace enigma

#endif
