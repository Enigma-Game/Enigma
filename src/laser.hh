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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id: laser.hh,v 1.6 2004/03/15 20:15:35 dheck Exp $
 */
#ifndef LASER_HH
#define LASER_HH

/* This file contains the declarations for (almost) everything that
   has to do with lasers. */

#include "objects.hh"

namespace world
{
    // This interface must be implemented by all items and stones that
    // are capable of emitting light.
    class LaserEmitter {
    public:
	virtual ~LaserEmitter() {}
	virtual DirectionBits emission_directions() const = 0;
    };

/* -------------------- PhotoCell -------------------- */

    /*! Photocells are objects (not necessarily stones) that are
      sensitive to laser light. */

    class PhotoCell {
    public:
        virtual ~PhotoCell() {
            photo_deactivate();
        }

        static void notify_start();
        static void notify_finish();

        virtual void on_recalc_start() = 0;
        virtual void on_recalc_finish() = 0;
    protected:
        void photo_activate();
        void photo_deactivate();
    private:
        static std::vector<void*> instances;
    };

/* -------------------- PhotoStone -------------------- */

    /*! Most stones are indifferent to laser beams: They either block
      the light completely or they let it pass, but they do not change
      their internal state when they are hit by light.  Certain kinds
      of stones need to be notified whenever the `light' goes on or off
      -- these can be derived from this class.
    
      The most prominent example are Oxyd stones -- they open when
      they are hit by a laser beam.  See the remarks at the beginning
      of this file to understand why overriding `on_laserhit' is not
      sufficient for a proper implementation of Oxyd stones.
    */

    class PhotoStone : public Stone, public PhotoCell {
    protected:
        PhotoStone(const char *kind) : Stone(kind) {illuminated = false;}
    private:
        bool illuminated;

        void on_recalc_start() {}
        void on_recalc_finish() { check_state(); }

        void check_state();

        virtual void notify_laseron() = 0;
        virtual void notify_laseroff() = 0;
    };
}

/* -------------------- Functions -------------------- */
namespace lasers
{
    void Init();

    /*! This function must be called at the end of each tick; it
      recalculates the laser beams if necessary. */
    void RecalcLightNow();

    /*! Force all light beams to be recalculated at the end of the
      current tick.  So far, this is only used by laser stones and in
      world::InitWorld().  */
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
}
#endif
