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
#ifndef MIRRORSTONE_HH
#define MIRRORSTONE_HH

#include "stones.hh"
#include "stones_internal.hh"

namespace enigma {

    class MirrorStone : public Stone {
         CLONEOBJ(MirrorStone);
         DECL_TRAITS_ARRAY(2, traitsIdx());
    private:
        enum iState {
        };
        enum ObjectPrivatFlagsBits {
            OBJBIT_TRIANGLE =   1<<24,    ///< 
            OBJBIT_PANE     =   1<<25,    ///< transparency of mirror panes 
            OBJBIT_SIDE     =   1<<26,    ///< transparency of mirror sides (just planar mirror)
            OBJBIT_MOVABLE  =   1<<27     ///<  
        };

    public:
        MirrorStone(bool isTriangle, bool isMovable, bool paneTransparent, bool sideTransparent, 
                Direction dir=EAST, bool counterclock=false);

        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const std::string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual int maxState() const;
        virtual void setState(int extState);
        virtual void toggleState();

        // GridObject interface
        virtual void init_model();
        virtual void on_creation(GridPos p);
        virtual void processLight(Direction d);   // direction of laserbeam
        virtual DirectionBits emissionDirections() const;
        
        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
               
    private:
        // Private methods.
        int traitsIdx() const;
    };

} // namespace enigma

#endif
