/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
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
#ifndef OXYDSTONE_HH_INCLUDED
#define OXYDSTONE_HH_INCLUDED

#include "stones.hh"
#include "laser.hh"

/* -------------------- Oxyd stone -------------------- */

/** \page st-oxyd Oxyd Stone

Oxyd stones are characterized by two attributes: Their flavor and
their color.  The flavor only affects the visual representation of
the stone; it can be either 'a' (opening like a flower) or 'b'
(displaying a fade-in animation).  The color attribute determines
the shape on the oxyd stone.

\b Note: You should usually not to create Oxyd stones manually
with \c set_stone(). Use the predefined \c oxyd() function instead.

\subsection oxyda Attributes

- \b flavor      "a", "b", "c", or "d"
- \b color       number between 0 and 7

\subsection oxydm Messages

- \b closeall    close all oxyd stones
- \b shuffle     interchange the colors of the oxyd stones in the current landscape
- \b trigger     open the stone

<table><tr>
<td>\image html st-oxyda.png "flavor A"
<td>\image html st-oxydb.png "flavor B"
<td>\image html st-oxydc.png "flavor C"
<td>\image html st-oxydd.png "flavor D"
</table>
*/

namespace enigma {

    class OxydStone : public PhotoStone {
        typedef std::vector<OxydStone *> InstanceList;
        static InstanceList instances;
        OxydStone * clone();
        void dispose() {
            instances.erase(find(instances.begin(), instances.end(), this));
            delete this;
        }

    public:
        OxydStone();

        static void shuffle_colors();
        virtual bool is_removable() const;
    private:
        enum State { CLOSED, OPEN, OPENING, CLOSING, BLINKING };
        State state;

        // Stone interface
        void actor_hit(const StoneContact &sc);
        void on_creation (GridPos p);
        void on_removal (GridPos p);
        const char *collision_sound() { return "stone"; }
        virtual Value message(const string &m, const Value &);


        // PhotoStone interface
        void notify_laseron() { maybe_open_stone(); }
        void notify_laseroff() {}

        // Animation callback
        void animcb();

        // Private methods
        void maybe_open_stone();
        void change_state(State newstate);


        static bool blinking(OxydStone *a) {
            return (a->state==BLINKING);
        }
        static bool blinking_or_opening(OxydStone *a) {
            return (a->state==BLINKING || a->state == OPENING);
        }
        static bool not_open(OxydStone *a) {
            return !(a->state==OPEN || a->state==OPENING);
        }

    };
} // namespace enigma

#endif
