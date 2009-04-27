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
#ifndef BOULDERSTONE_HH
#define BOULDERSTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     */
    class BoulderStone : public Stone {
        CLONEOBJ(BoulderStone);
        DECL_TRAITS;
    private:
        enum iState {
            INIT,       ///< 
            ACTIVE,     ///< may send trigger into direction
            IDLE,       ///< already sent trigger w/o success
            FALLING     ///< falling into abyss
        };
        
        enum ObjectPrivatFlagsBits {
            OBJBIT_LIGHT     =  15<<24   ///< Light status kept for move, swap, pull operations 
        };
    public:
        BoulderStone(Direction dir = NORTH);
        
        // Object interface
        virtual void setAttr(const string& key, const Value &val);
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual int externalState() const;
        virtual void setState(int extState);

        // GridObject interface
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void init_model();
        virtual void lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs);
        
        // ModelCallback interface
        virtual void animcb();
        
        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
        virtual void on_floor_change();
        virtual bool on_move(const GridPos &origin);
        virtual void on_impulse(const Impulse& impulse);
//        virtual const char *collision_sound();

    private:
        Direction getDir() const;
        void setDir(Direction d);
        bool haveObstacle(Direction dir);
        void triggerObstacle(Direction dir, bool isRaising);
    };

} // namespace enigma

#endif
