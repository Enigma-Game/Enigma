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
#ifndef ROTATORSTONE_HH
#define ROTATORSTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     */
    class RotatorStone : public Stone, public TimeHandler {
        CLONEOBJ(RotatorStone);
        DECL_TRAITS_ARRAY(2, traitsIdx());
    private:
        enum iState {
            CW,   ///< clockwise rotation 
            CCW   ///< counter clockwise rotation
        };
        
        enum ObjectPrivatFlagsBits {
            OBJBIT_MOVABLE   =   1<<24,  ///< Object is movable 
            OBJBIT_LIGHT     =  15<<25   ///< Light status kept for move, swap, pull operations 
        };
    public:
        RotatorStone(bool isMovable, bool counterclockwise);
        ~RotatorStone();
        
        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const std::string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        
        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        virtual void on_creation(GridPos p);
        virtual void on_removal(GridPos p);
        virtual void lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs);
        
        // ModelCallback interface
        virtual void animcb();
        
        // Stone interface
        virtual void actor_hit(const StoneContact &sc);
        virtual void on_impulse(const Impulse& impulse);
        virtual bool on_move(const GridPos &origin);
        virtual FreezeStatusBits get_freeze_bits();

        // TimeHandler interface
        virtual void alarm();
        
    private:
        int traitsIdx() const;
        void send_impulses();
    };

} // namespace enigma

#endif

