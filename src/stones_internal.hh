/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 *
 * This program is free software; you can redistribute it and/ or
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
#ifndef STONES_INTERNAL_HH_INCLUDED
#define STONES_INTERNAL_HH_INCLUDED

#include "world.hh"
#include "main.hh"

#define DECL_TRAITS                                              \
        static StoneTraits traits;                               \
        const StoneTraits &get_traits() const { return traits; }

#define DECL_TRAITS_ARRAY(n, subtype_expr)                                      \
        static StoneTraits traits[n];                                            \
        const StoneTraits &get_traits() const { return traits[subtype_expr]; }

#define DEF_TRAITS(classname, name, id)         \
    StoneTraits classname::traits = { name, id, stf_none, material_stone, 1.0, MOVABLE_PERSISTENT }

#define DEF_TRAITSM(classname, name, id, movable)         \
    StoneTraits classname::traits = { name, id, stf_none, material_stone, 1.0, movable }



namespace enigma {

    void Init_simple();
    void Init_complex();


/* -------------------- Auxiliary Functions -------------------- */

    Direction get_push_direction (const StoneContact &sc);

    /* Move a stone (by sending an impulse) Called when actor hits a
       stone. */
    bool maybe_push_stone (const StoneContact &sc);

} // namespace enigma

#endif
