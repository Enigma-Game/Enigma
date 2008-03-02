/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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
#ifndef STONES_HH_INCLUDED
#define STONES_HH_INCLUDED

#include "GridObject.hh"

namespace enigma {
    
//    using namespace enigma;
    struct Impulse;
    struct StoneContact;

    enum StoneID {
        st_INVALID = -1,
        st_FIRST = 0,
        st_none = 0,

        st_borderstone,
        st_black1,
        st_black2,
        st_black3,
        st_black4,
        st_block,
        st_boulder,
        st_brake,
        st_break_acblack,
        st_break_acwhite,
        st_break_bolder,
        st_break_invisible,
        st_breaking,
        st_bug,
        st_chameleon,
        st_chess,
        st_coffee,
        st_coinslot,
        st_death,
        st_death_invisible,
        st_easymode,
        st_explosion,
        st_fakeoxyda,
        st_fart,
        st_firebreak,
        st_firebreak_move,
        st_floppy,
        st_fourswitch,
        st_greenbrown_growing,
        st_key,
        st_knight,
        st_laserbreak,
        st_magic,
        st_movebreak,
        st_oneway,
        st_oneway_black,
        st_oneway_white,
        st_oxyd_0x18,
        st_peroxyd_0xb8,
        st_peroxyd_0xb9,
        st_plain,
        st_plain_break,
        st_plain_breaking,
        st_plain_cracked,
        st_plain_falling,
        st_plain_hole,
        st_plain_move,
        st_pull,
        st_puzzle,
        st_rotator,
        st_rubberband,
        st_scissors,
        st_shogun,
        st_stoneimpulse,
        st_stoneimpulse_hollow,
        st_stoneimpulse_movable,
        st_surprise,
        st_swap,
        st_switch,
        st_thief,
        st_turnstile,
        st_turnstile_e,
        st_turnstile_green,
        st_turnstile_n,
        st_turnstile_s,
        st_turnstile_w,
        st_volcano,
        st_white1,
        st_white2,
        st_white3,
        st_white4,
        st_window,
        st_wood_growing,
        st_lightpassenger,
        st_camouflage,
        st_polarswitch,
        st_volcano_growing,

        st_LAST,
        st_COUNT = st_LAST
    };

    enum StoneFlags {
        stf_none = 0x0,
        stf_transparent = 0x1,  //< Laser beams can pass
    };

    enum Material {
        material_FIRST = 0,
        material_stone = 0,
        material_glass,
        material_metal,
        material_wood,
        material_cloth,
        material_LAST
    };

    enum StoneMovableStatus {
        MOVABLE_PERSISTENT, // Stone is unmovable
        MOVABLE_BREAKABLE,  // Stone is breakable
        MOVABLE_STANDARD,   // Stone is movable in st-wood-way
        MOVABLE_IRREGULAR   // Stone moves in non-standard way
    };        

    struct StoneTraits {
        const char         *name;
        StoneID             id;
        int                 flags;
        Material            material;
        double              restitution;
        StoneMovableStatus  movable;
        // Note that many properties of stones are implemented as functions.
    };

    /*! Things that may happen when an actor hits a stone. */
    enum StoneResponse {
        STONE_PASS,             // Actor may pass stone
        STONE_REBOUND           // Actor bounces off the stone
    };

    /*! Combined status bits, used in the freeze check routines. */
    enum FreezeStatusBits {
        FREEZEBIT_NO_STONE   =  1, // Stone is NULL or easily breakable
        FREEZEBIT_HOLLOW     =  2, // Stone is hollow (no cherry)
        FREEZEBIT_PERSISTENT =  4, // Stone is unmovable
        FREEZEBIT_STANDARD   =  8, // Stone is movable in st-wood-way
        FREEZEBIT_IRREGULAR  = 16  // Stone moves in non-standard way
    };
        
    class Stone : public GridObject {
    public:
        Stone();
        Stone(const char *kind);
        ~Stone();

        /* ---------- Virtual functions ---------- */
        virtual Stone *clone() = 0;
        const char *get_kind() const;

        /* ---------- Stone interface (properties) ---------- */

        virtual const StoneTraits &get_traits() const;

        virtual const char *collision_sound();

        /**
         * 
         */
        virtual StoneResponse collision_response(const StoneContact &sc);

        /*! Is this stone movable? Affects impulse-stones, fire, ordinary pushes... */
        bool   is_movable() const {
            return get_traits().movable >= MOVABLE_STANDARD;
        }

        /*! Can a swap-stone or pull-stone swap this stone? */
        virtual bool   is_removable() const { return true; }

        /*! Is this stone floating above the floor (e.g. actors may pass)? */
        virtual bool   is_floating() const { return false; }
        
        /*! Can laser beams pass through stone? Return is_floating() by default. */
        virtual bool   is_transparent (Direction d) const { 
            return is_floating();
        }
        
        /*! Do actors get stuck in this stone? */
        virtual bool is_sticky(const Actor *a) const { 
            return !is_floating(); 
        }

        /* ---------- Stone interface (events) ---------- */

        virtual void   actor_hit (const StoneContact &sc);
        virtual void   actor_touch (const StoneContact &sc);
        virtual void   actor_inside (Actor * /*a*/) {}
        virtual void   actor_contact (Actor * /*a*/) {}

        virtual bool   freeze_check();
        
        virtual void   on_move();
        virtual void   on_floor_change() {}
        virtual void   on_impulse(const Impulse& impulse);

    protected:
        virtual Object::ObjectType getObjectType() const {return Object::STONE;}
        
        bool move_stone(GridPos newPos, const char *soundevent);
        bool move_stone(Direction dir);
        ecl::V2 distortedVelocity (ecl::V2 vel, double defaultfactor);

    protected:
        // GridObject interface
        virtual void set_model (const std::string &mname) {
            display::SetModel(GridLoc(GRID_STONES, get_pos()), mname);
        }

        virtual display::Model *get_model () {
            return display::GetModel(GridLoc(GRID_STONES, get_pos()));
        }

        virtual void kill_model (GridPos p) {
            display::KillModel (GridLoc (GRID_STONES, p));
        }

    private:
        // Help structure and routine for freeze_check()
        bool freeze_check_running;
        virtual FreezeStatusBits get_freeze_bits();  // own freeze bits
        virtual FreezeStatusBits get_freeze_bits(GridPos p);  // foreign freeze bits
    };

    inline StoneID get_id(Stone *st) {
        return st->get_traits().id;
    }
    
    void InitStones();
    
} // namespace enigma


#endif
