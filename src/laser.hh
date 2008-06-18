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
#ifndef LASER_HH
#define LASER_HH

/* This file contains the declarations for (almost) everything that
   has to do with lasers. */

#include "GridObject.hh"
#include "stones.hh"
#include "items.hh"
#include <list>

namespace enigma {

/* -------------------- LaserBeam -------------------- */

    class LaserBeam : public Item {
    public:
        static void emit_from(GridPos p, Direction d);
        static void kill_all();
        static void all_emitted();
        static void prepareLevel();

        // GridObject interface
        DirectionBits emissionDirections() const { return (DirectionBits)(objFlags & 15); }
        static ItemTraits traits;

        const ItemTraits &get_traits() const {
            return traits;
        }
    private:
        LaserBeam(Direction dir) {
            objFlags |= to_bits(dir);
        }

        // Item interface.
        void processLight(Direction dir);
        void on_creation (GridPos p);
        virtual void on_removal(GridPos p);
        void init_model();
        bool actor_hit(Actor *actor);

        Item *clone() {
            // new LaserBeams may only created inside `emit_from'.
//            assert(0);
            return 0;
        }
        void dispose();

        // Variables
        
        static std::list<LaserBeam *> beamList;
    };

/* -------------------- Functions -------------------- */
    void InitLasers();

    /*! This function must be called at the end of each tick; it
      recalculates the laser beams if necessary. */
    void PerformRecalcLight(bool isInit);

    /*! Force all light beams to be recalculated at the end of the
      current tick.  So far, this is only used by laser stones and in
      WorldInitLevel().  */
    void RecalcLight();

    /*! If position `p' is inside a laser beam, force all laser beams
      to be recalculated.  This is mainly used when items and stones
      are created or removed, but it can be also used for objects
      (like doors) that sometimes shut off a light beam (when the door
      is closed) and sometimes don't (when the door is open). */
    void MaybeRecalcLight (enigma::GridPos p);

    /*! Return true iff a stone or an item at position `p' it hit by
      light coming from direction `dir'. */
    bool LightFrom (enigma::GridPos p, enigma::Direction dir);

} // namespace enigma

#endif
