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
#ifndef FAKESTONE_HH
#define FAKESTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     */
    class FakeStone : public Stone {
         CLONEOBJ(FakeStone);

    private:
        enum iState {
            IDLE,     ///< standard fake stone - idle or closed
            ACTIVE    ///< a blinking quake or opened oxyd
        };
        
        enum ObjectPrivatFlagsBits {
            OBJBIT_SUBTYP    =   7<<24   ///< the FakeStoneTyp
        };
        
        enum FakeStoneTyp {
            QUAKE = 0,
            OXYDA,
            OXYDB,
            OXYDC,
            OXYDD,
            OXYDE
        };
    public:
        FakeStone(int subtyp);
        
        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        
        // StateObject interface
        virtual void setState(int extState);
        
        // GridObject interface
        virtual void init_model();
        
        // ModelCallback interface
        virtual void animcb();
        
        // Stone interface
        virtual const char *collision_sound();
        virtual void actor_hit(const StoneContact &sc);
    
    private:
        // Private methods.
        std::string getFlavor() const;
    };

} // namespace enigma

#endif /*FLOORBUILDERSTONE_HH*/
