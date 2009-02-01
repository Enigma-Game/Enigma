/*
 * Copyright (C) May 2003 by Ralf Westram <amgine@rallysoft.de>
 * Copyright (C) 2004 Daniel Heck
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

#if defined(PLAIN_SPEC_ONLY)
// This is used from lib-src/oxydlib/EnigmaNames.cpp.
// 
// defining PLAIN_SPEC_ONLY only shows xxx_floor_map, xxx_stone_map and xxx_item_map
// Note:  the xxx_item_map changes it's type (ItemID -> const char *)

#else
// Standard enigma section :

#include "oxyd_internal.hh"
using namespace enigma;

#endif // PLAIN_SPEC_ONLY
// only tables following!

const char *oxyd::oxydextra_floor_map[256] = {
    "fl_abyss",          // OxydExtra floor 0x00
    "fl_gray",           // OxydExtra floor 0x01
    "fl_metal",          // OxydExtra floor 0x02
    UNUSED,              // OxydExtra floor 0x03
    UNUSED,              // OxydExtra floor 0x04
    UNUSED,              // OxydExtra floor 0x05
    UNUSED,              // OxydExtra floor 0x06
    "fl_ivory",          // OxydExtra floor 0x07
    "fl_redslab",        // OxydExtra floor 0x08
    "fl_blueslab",       // OxydExtra floor 0x09
    "fl_blueslab",       // OxydExtra floor 0x0a
    "fl_redslab",        // OxydExtra floor 0x0b
    "fl_inverse",        // OxydExtra floor 0x0c
    "fl_dark",           // OxydExtra floor 0x0d
    UNUSED,              // OxydExtra floor 0x0e
    UNUSED,              // OxydExtra floor 0x0f
    UNUSED,              // OxydExtra floor 0x10
    UNUSED,              // OxydExtra floor 0x11
    UNUSED,              // OxydExtra floor 0x12
    "fl_water",          // OxydExtra floor 0x13
    UNUSED,              // OxydExtra floor 0x14
    UNUSED,              // OxydExtra floor 0x15
    UNUSED,              // OxydExtra floor 0x16
    "fl_ice",            // OxydExtra floor 0x17
    UNUSED,              // OxydExtra floor 0x18
    UNUSED,              // OxydExtra floor 0x19
    UNUSED,              // OxydExtra floor 0x1a
    UNUSED,              // OxydExtra floor 0x1b
    "fl_space",          // OxydExtra floor 0x1c
    UNUSED,              // OxydExtra floor 0x1d
    UNUSED,              // OxydExtra floor 0x1e
    UNUSED,              // OxydExtra floor 0x1f
    UNUSED,              // OxydExtra floor 0x20
    UNUSED,              // OxydExtra floor 0x21
    UNUSED,              // OxydExtra floor 0x22
    UNUSED,              // OxydExtra floor 0x23
    UNUSED,              // OxydExtra floor 0x24
    "fl-gradient3",      // OxydExtra floor 0x25
    "fl-gradient4",      // OxydExtra floor 0x26
    UNUSED,              // OxydExtra floor 0x27
    UNUSED,              // OxydExtra floor 0x28
    UNUSED,              // OxydExtra floor 0x29
    UNUSED,              // OxydExtra floor 0x2a
    UNUSED,              // OxydExtra floor 0x2b
    UNUSED,              // OxydExtra floor 0x2c
    UNUSED,              // OxydExtra floor 0x2d
    UNUSED,              // OxydExtra floor 0x2e
    UNUSED,              // OxydExtra floor 0x2f
    UNUSED,              // OxydExtra floor 0x30
    "fl-gradient15",     // OxydExtra floor 0x31
    "fl-gradient16",     // OxydExtra floor 0x32
    UNUSED,              // OxydExtra floor 0x33
    "fl_rough",          // OxydExtra floor 0x34
    UNUSED,              // OxydExtra floor 0x35
    UNUSED,              // OxydExtra floor 0x36
    UNUSED,              // OxydExtra floor 0x37
    "fl_mortar",         // OxydExtra floor 0x38
    UNUSED,              // OxydExtra floor 0x39
    UNUSED,              // OxydExtra floor 0x3a
    UNUSED,              // OxydExtra floor 0x3b
    "fl_samba",          // OxydExtra floor 0x3c
    UNUSED,              // OxydExtra floor 0x3d
    UNUSED,              // OxydExtra floor 0x3e
    UNUSED,              // OxydExtra floor 0x3f
    UNUSED,              // OxydExtra floor 0x40
    "fl_samba",          // OxydExtra floor 0x41
    UNUSED,              // OxydExtra floor 0x42
    "fl_pinkbumps",      // OxydExtra floor 0x43
    UNUSED,              // OxydExtra floor 0x44
    UNUSED,              // OxydExtra floor 0x45
    UNUSED,              // OxydExtra floor 0x46
    "fl_pinkbumps",      // OxydExtra floor 0x47
    "fl_rock",           // OxydExtra floor 0x48
    UNUSED,              // OxydExtra floor 0x49
    UNUSED,              // OxydExtra floor 0x4a
    UNUSED,              // OxydExtra floor 0x4b
    "fl_bridge",         // OxydExtra floor 0x4c
    UNUSED,              // OxydExtra floor 0x4d
    UNUSED,              // OxydExtra floor 0x4e
    UNUSED,              // OxydExtra floor 0x4f
    UNUSED,              // OxydExtra floor 0x50
    UNUSED,              // OxydExtra floor 0x51
    UNUSED,              // OxydExtra floor 0x52
    UNUSED,              // OxydExtra floor 0x53
    "fl_gray",           // OxydExtra floor 0x54
    "fl_plank",          // OxydExtra floor 0x55
    UNUSED,              // OxydExtra floor 0x56
    UNUSED,              // OxydExtra floor 0x57
    UNUSED,              // OxydExtra floor 0x58
    "fl_trigger",        // OxydExtra floor 0x59
    UNUSED,              // OxydExtra floor 0x5a
    "fl_gravel",         // OxydExtra floor 0x5b
    // codes >= 0x5c are unused
};

const char *oxyd::oxydextra_stone_map[256] = {
    0,                   // no stone
    0, 0, 0, 0, 0, 0, 0, 0,     // oxyd stones
    0, 0, 0, 0, 0, 0, 0, 0,     // oxyd stones
    "st_fake_quake",     // OxydExtra stone 0x11
    "st_flat",           // OxydExtra stone 0x12
    "st_purplemarble",   // OxydExtra stone 0x13
    UNUSED,              // OxydExtra stone 0x14
    "st_tigris",         // OxydExtra stone 0x15
    "st_greengray",      // OxydExtra stone 0x16
    UNUSED,              // OxydExtra stone 0x17
    "st_grate_cross",    // OxydExtra stone 0x18
    UNUSED,              // OxydExtra stone 0x19
    UNUSED,              // OxydExtra stone 0x1a
    UNUSED,              // OxydExtra stone 0x1b
    "st_redbrown",       // OxydExtra stone 0x1c
    UNUSED,              // OxydExtra stone 0x1d
    UNUSED,              // OxydExtra stone 0x1e
    "st_tigris",         // OxydExtra stone 0x1f
    "st_redmarble",      // OxydExtra stone 0x20
    "st_greengray",      // OxydExtra stone 0x21
    "st_lightglass",     // OxydExtra stone 0x22
    UNUSED,              // OxydExtra stone 0x23
    "st_flat_breakable", // OxydExtra stone 0x24
    "st_flat_hollow",    // OxydExtra stone 0x25
    "st_flat_movable",   // OxydExtra stone 0x26
    "st_box_wood",       // OxydExtra stone 0x27
    "st_switch_instant", // OxydExtra stone 0x28
    UNUSED,              // OxydExtra stone 0x29
    UNUSED,              // OxydExtra stone 0x2a
    UNUSED,              // OxydExtra stone 0x2b
    UNUSED,              // OxydExtra stone 0x2c
    "st_floppy",         // OxydExtra stone 0x2d
    UNUSED,              // OxydExtra stone 0x2e
    "st_death",          // OxydExtra stone 0x2f
    "st_death_invisible", // OxydExtra stone 0x30
    "st_oneway_black_w", // OxydExtra stone 0x31
    UNUSED,              // OxydExtra stone 0x32
    "st_oneway_black_n", // OxydExtra stone 0x33
    "st_oneway_black_s", // OxydExtra stone 0x34
    UNUSED,              // OxydExtra stone 0x35
    UNUSED,              // OxydExtra stone 0x36
    UNUSED,              // OxydExtra stone 0x37
    UNUSED,              // OxydExtra stone 0x38
    UNUSED,              // OxydExtra stone 0x39
    UNUSED,              // OxydExtra stone 0x3a
    UNUSED,              // OxydExtra stone 0x3b
    "st_actorimpulse",   // OxydExtra stone 0x3c
    UNUSED,              // OxydExtra stone 0x3d
    "st-laser-1",        // OxydExtra stone 0x3e  The laser-names are fake names!
    "st-laser-2",        // OxydExtra stone 0x3f  Direction and state are generated by Enigma.
    "st-laser-3",        // OxydExtra stone 0x40
    UNUSED,              // OxydExtra stone 0x41
    UNUSED,              // OxydExtra stone 0x42
    "st_mirror_slab_s",  // OxydExtra stone 0x43
    "st_mirror_slab_w",  // OxydExtra stone 0x44
    UNUSED,              // OxydExtra stone 0x45
    "st_mirror_slab_e",  // OxydExtra stone 0x46
    UNUSED,              // OxydExtra stone 0x47
    UNUSED,              // OxydExtra stone 0x48
    UNUSED,              // OxydExtra stone 0x49
    UNUSED,              // OxydExtra stone 0x4a
    "st_mirror_slab_st", // OxydExtra stone 0x4b
    UNUSED,              // OxydExtra stone 0x4c
    "st_mirror_triangle_ec", // OxydExtra stone 0x4d
    "st_mirror_triangle_nc", // OxydExtra stone 0x4e
    "st_mirror_triangle_wc", // OxydExtra stone 0x4f
    "st_mirror_triangle_sc", // OxydExtra stone 0x50
    UNUSED,              // OxydExtra stone 0x51
    UNUSED,              // OxydExtra stone 0x52
    UNUSED,              // OxydExtra stone 0x53
    UNUSED,              // OxydExtra stone 0x54
    UNUSED,              // OxydExtra stone 0x55
    UNUSED,              // OxydExtra stone 0x56
    UNUSED,              // OxydExtra stone 0x57
    UNUSED,              // OxydExtra stone 0x58
    UNUSED,              // OxydExtra stone 0x59
    UNUSED,              // OxydExtra stone 0x5a
    UNUSED,              // OxydExtra stone 0x5b
    UNUSED,              // OxydExtra stone 0x5c
    "st_break_oxydc",    // OxydExtra stone 0x5d
    UNUSED,              // OxydExtra stone 0x5e
    UNUSED,              // OxydExtra stone 0x5f
    UNUSED,              // OxydExtra stone 0x60
    UNUSED,              // OxydExtra stone 0x61
    UNUSED,              // OxydExtra stone 0x62
    UNUSED,              // OxydExtra stone 0x63
    "st_coinslot_instant", // OxydExtra stone 0x64
    "st_thief",          // OxydExtra stone 0x65
    "st_shogun_s",       // OxydExtra stone 0x66
    UNUSED,              // OxydExtra stone 0x67
    UNUSED,              // OxydExtra stone 0x68
    UNUSED,              // OxydExtra stone 0x69
    UNUSED,              // OxydExtra stone 0x6a
    UNUSED,              // OxydExtra stone 0x6b
    UNUSED,              // OxydExtra stone 0x6c
    "st_stoneimpulse",   // OxydExtra stone 0x6d
    UNUSED,              // OxydExtra stone 0x6e
    UNUSED,              // OxydExtra stone 0x6f
    UNUSED,              // OxydExtra stone 0x70
    UNUSED,              // OxydExtra stone 0x71
    UNUSED,              // OxydExtra stone 0x72
    "st_door_d",         // OxydExtra stone 0x73
    "st_door_d_ew",      // OxydExtra stone 0x74
    UNUSED,              // OxydExtra stone 0x75
    "st_invisible",      // OxydExtra stone 0x76
    UNUSED,              // OxydExtra stone 0x77
    UNUSED,              // OxydExtra stone 0x78
    UNUSED,              // OxydExtra stone 0x79
    UNUSED,              // OxydExtra stone 0x7a
    UNUSED,              // OxydExtra stone 0x7b
    UNUSED,              // OxydExtra stone 0x7c
    UNUSED,              // OxydExtra stone 0x7d
    UNUSED,              // OxydExtra stone 0x7e
    UNUSED,              // OxydExtra stone 0x7f
    UNUSED,              // OxydExtra stone 0x80
    UNUSED,              // OxydExtra stone 0x81
    UNUSED,              // OxydExtra stone 0x82
    UNUSED,              // OxydExtra stone 0x83
    UNUSED,              // OxydExtra stone 0x84
    UNUSED,              // OxydExtra stone 0x85
    UNUSED,              // OxydExtra stone 0x86
    UNUSED,              // OxydExtra stone 0x87
    UNUSED,              // OxydExtra stone 0x88
    UNUSED,              // OxydExtra stone 0x89
    "st_bluesand",       // OxydExtra stone 0x8a
    "st_bluegray",       // OxydExtra stone 0x8b
    UNUSED,              // OxydExtra stone 0x8c
    UNUSED,              // OxydExtra stone 0x8d
    UNUSED,              // OxydExtra stone 0x8e
    UNUSED,              // OxydExtra stone 0x8f
    UNUSED,              // OxydExtra stone 0x90
    UNUSED,              // OxydExtra stone 0x91
    UNUSED,              // OxydExtra stone 0x92
    UNUSED,              // OxydExtra stone 0x93
    UNUSED,              // OxydExtra stone 0x94
    UNUSED,              // OxydExtra stone 0x95
    UNUSED,              // OxydExtra stone 0x96
    UNUSED,              // OxydExtra stone 0x97
    UNUSED,              // OxydExtra stone 0x98
    UNUSED,              // OxydExtra stone 0x99
    UNUSED,              // OxydExtra stone 0x9a
    UNUSED,              // OxydExtra stone 0x9b
    UNUSED,              // OxydExtra stone 0x9c
    UNUSED,              // OxydExtra stone 0x9d
    "st_turnstile_red",  // OxydExtra stone 0x9e
    "st_turnstilearm_n", // OxydExtra stone 0x9f
    "st_turnstilearm_s", // OxydExtra stone 0xa0
    "st_turnstilearm_w", // OxydExtra stone 0xa1
    "st_turnstilearm_e", // OxydExtra stone 0xa2
    UNUSED,              // OxydExtra stone 0xa3
    "st_laserflop_on",   // OxydExtra stone 0xa4
    UNUSED,              // OxydExtra stone 0xa5
    UNUSED,              // OxydExtra stone 0xa6
    UNUSED,              // OxydExtra stone 0xa7
    UNUSED,              // OxydExtra stone 0xa8
    UNUSED,              // OxydExtra stone 0xa9
    UNUSED,              // OxydExtra stone 0xaa
    UNUSED,              // OxydExtra stone 0xab
    UNUSED,              // OxydExtra stone 0xac
    UNUSED,              // OxydExtra stone 0xad
    UNUSED,              // OxydExtra stone 0xae
    UNUSED,              // OxydExtra stone 0xaf
    UNUSED,              // OxydExtra stone 0xb0
    UNUSED,              // OxydExtra stone 0xb1
    UNUSED,              // OxydExtra stone 0xb2
    UNUSED,              // OxydExtra stone 0xb3
    UNUSED,              // OxydExtra stone 0xb4
    UNUSED,              // OxydExtra stone 0xb5
    UNUSED,              // OxydExtra stone 0xb6
    UNUSED,              // OxydExtra stone 0xb7
    UNUSED,              // OxydExtra stone 0xb8
    UNUSED,              // OxydExtra stone 0xb9
    UNUSED,              // OxydExtra stone 0xba
    "st_stoneimpulse_hollow", // OxydExtra stone 0xbb
    // codes >= 0xbc are unused
};

const char* oxyd::oxydextra_item_map[256] = {
    IT_INVALID,                    // OxydExtra item 0x00
    "it_extralife",               // OxydExtra item 0x01
    IT_EXTERNAL,                // OxydExtra item 0x02
    IT_EXTERNAL,                // OxydExtra item 0x03
    "it_hammer",                  // OxydExtra item 0x04
    "it_coffee",                  // OxydExtra item 0x05
    "it_cherry",                  // OxydExtra item 0x06
    "it_umbrella",                // OxydExtra item 0x07
    IT_MISSING,                 // OxydExtra item 0x08
    UNUSED,                  // OxydExtra item 0x09
    "it-dynamite",                // OxydExtra item 0x0a
    UNUSED,                  // OxydExtra item 0x0b
    UNUSED,                  // OxydExtra item 0x0c
    "it_crack_i",                 // OxydExtra item 0x0d
    "it_crack_s",                 // OxydExtra item 0x0e
    "it_crack_m",                 // OxydExtra item 0x0f
    "it_crack_l",                 // OxydExtra item 0x10
    "it_coin_s",                  // OxydExtra item 0x11
    "it_coin_m",                  // OxydExtra item 0x12
    "it_coin_l",                  // OxydExtra item 0x13
    "it_key_a",                   // OxydExtra item 0x14
    "it_key_b",                   // OxydExtra item 0x15
    "it_key_c",                   // OxydExtra item 0x16
    "it_floppy",                  // OxydExtra item 0x17
    UNUSED,                  // OxydExtra item 0x18
    UNUSED,                  // OxydExtra item 0x19
    UNUSED,                  // OxydExtra item 0x1a
    UNUSED,                  // OxydExtra item 0x1b
    UNUSED,                  // OxydExtra item 0x1c
    UNUSED,                  // OxydExtra item 0x1d
    UNUSED,                  // OxydExtra item 0x1e
    UNUSED,                  // OxydExtra item 0x1f
    UNUSED,                  // OxydExtra item 0x20
    "it_spade",                   // OxydExtra item 0x21
    "it_surprise",                // OxydExtra item 0x22
    "it_pin",                     // OxydExtra item 0x23
    UNUSED,                  // OxydExtra item 0x24
    UNUSED,                  // OxydExtra item 0x25
    UNUSED,                  // OxydExtra item 0x26
    "it_bag",                     // OxydExtra item 0x27
    UNUSED,                  // OxydExtra item 0x28
    UNUSED,                  // OxydExtra item 0x29
    "it_sensor",                  // OxydExtra item 0x2a
    "it_shogun_s",                // OxydExtra item 0x2b
    UNUSED,                  // OxydExtra item 0x2c
    "it_vortex_open",             // OxydExtra item 0x2d
    UNUSED,                  // OxydExtra item 0x2e
    "it_wormhole_on",        // OxydExtra item 0x2f
    "it_meditation_hill",    // OxydExtra item 0x30
    "it_meditation_bump",    // OxydExtra item 0x31
    "it_meditation_hollow",  // OxydExtra item 0x32
    "it_meditation_dent",    // OxydExtra item 0x33
    UNUSED,                  // OxydExtra item 0x34
    UNUSED,                  // OxydExtra item 0x35
    UNUSED,                  // OxydExtra item 0x36
    "it-bridge-oxyd",             // OxydExtra item 0x37
    UNUSED,                  // OxydExtra item 0x38
    UNUSED,                  // OxydExtra item 0x39
    UNUSED,                  // OxydExtra item 0x3a
    UNUSED,                  // OxydExtra item 0x3b
    IT_MISSING,                 // OxydExtra item 0x3c
    UNUSED,                  // OxydExtra item 0x3d
    UNUSED,                  // OxydExtra item 0x3e
    UNUSED,                  // OxydExtra item 0x3f
    "it_trigger",                 // OxydExtra item 0x40
    "it_brush",                   // OxydExtra item 0x41
    "it_banana",                  // OxydExtra item 0x42
    // codes >= 0x43 are unused
};
