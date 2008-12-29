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
#ifndef STONEIMPULSESTONE_HH
#define STONEIMPULSESTONE_HH

#include "stones.hh"

#include "stones_internal.hh"

namespace enigma {

    /** 
     * 
     * It is essential for the propagation of impulses, that the expanding phase
     * of a stoneimpulse is slightly shorter than the shrinking phase. This is
     * currently achieved by the animation sequence. This mismatch is responsible
     * for blocking impulse waves from backfiring.
     * 
     * 
     */
    class StoneImpulse : public Stone {
        CLONEOBJ(StoneImpulse);
        DECL_TRAITS_ARRAY(4, traitsIdx());
    private:
        enum iState {
            IDLE,        ///<  
            EXPANDING,   ///<  
            SHRINKING    ///<
        };
        
        enum ObjectPrivatFlagsBits {
            OBJBIT_MOVABLE     =  1<<24,   ///< Object is movable 
            OBJBIT_HOLLOW      =  1<<25,   ///< hollow SI (exclusive usage with movalble feature!)
            OBJBIT_PROPAGATE   =  1<<25,   ///< only for movable SI mark impulse propagation on shrinking 
            OBJBIT_STEADY      =  1<<26,   ///< continuous laser light causes repeating impulses 
            OBJBIT_REPULSE     =  1<<27,   ///< pending impulse
            OBJBIT_NOBACKFIRE  =  1<<28,   ///< avoid sending impulse back into incoming direction
            OBJBIT_INCOMINGDIR =  3<<29,   ///< incoming direction
            OBJBIT_LASERIDLE   =  1<<31,   ///< not steady stoneimpulse did run singleton pulse and is idle but still enlighted
        };
    public:
        StoneImpulse(bool isSteady, bool isHollow, bool isMovable, bool isActive);
        
        // Object interface
        virtual std::string getClass() const;
        virtual void setAttr(const string& key, const Value &val);
        virtual Value getAttr(const std::string &key) const;
        virtual Value message(const Message &m);
        
        // StateObject interface
        virtual void setState(int extState);

        // GridObject interface
        virtual void init_model();
        virtual void on_creation(GridPos p);
        virtual void lightDirChanged(DirectionBits oldDirs, DirectionBits newDirs);
        
        // ModelCallback interface
        virtual void animcb();
        
        // Stone interface
        virtual bool is_floating() const;
        virtual StoneResponse collision_response(const StoneContact &sc);
        virtual void actor_inside(Actor *a);
        virtual void actor_hit(const StoneContact &sc);
        virtual void on_impulse(const Impulse& impulse);
//        virtual void on_move();
        virtual FreezeStatusBits get_freeze_bits();
        
    private:
        void setIState(int newState, Direction incoming = NODIR);
        void propagateImpulse(const Impulse& impulse);
        bool isHollow() const;
        int traitsIdx() const;
    };

} // namespace enigma

#endif

