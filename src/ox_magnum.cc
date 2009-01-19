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

const char *oxyd::oxydmag_floor_map[256] = {
    "fl_abyss",                 // OxydMagnum floor 0x00
    "fl_gray",                  // OxydMagnum floor 0x01 (common was 'fl_gray')
    "fl_metal",                 // OxydMagnum floor 0x02 (This should be "fl-stwood" for #112)
    "fl_metal_3",               // OxydMagnum floor 0x03
    "fl_metal_4",               // OxydMagnum floor 0x04
    0,                          // OxydMagnum floor 0x05 (common was 'fl-metal')(level 121 ?)
    "fl_metal_5",               // OxydMagnum floor 0x06
    "fl_metal_7n",              // OxydMagnum floor 0x07 (common was 'fl-metal')
    "fl_tigris",                // OxydMagnum floor 0x08 (common was 'fl-rough')
    "fl_tigris",                // OxydMagnum floor 0x09 (common was 'fl-normal')
    "fl_tigris",                // OxydMagnum floor 0x0a (common was 'fl-normal')
    "fl_tigris",                // OxydMagnum floor 0x0b (common was 'fl-stone')
    "fl_inverse",               // OxydMagnum floor 0x0c (common was 'fl_inverse')
    "fl_yinyang_yin",           // OxydMagnum floor 0x0d
    "fl_yinyang_yang",          // OxydMagnum floor 0x0e
    "fl_swamp",                 // OxydMagnum floor 0x0f
    UNUSED,                     // OxydMagnum floor 0x10
    UNUSED,                     // OxydMagnum floor 0x11
    UNUSED,                     // OxydMagnum floor 0x12
    "fl_water",                 // OxydMagnum floor 0x13
    UNUSED,                     // OxydMagnum floor 0x14
    UNUSED,                     // OxydMagnum floor 0x15
    UNUSED,                     // OxydMagnum floor 0x16
    "fl_ice",                   // OxydMagnum floor 0x17
    UNUSED,                     // OxydMagnum floor 0x18
    UNUSED,                     // OxydMagnum floor 0x19
    UNUSED,                     // OxydMagnum floor 0x1a
    UNUSED,                     // OxydMagnum floor 0x1b
    "fl_space",                 // OxydMagnum floor 0x1c
    UNUSED,                     // OxydMagnum floor 0x1d
    UNUSED,                     // OxydMagnum floor 0x1e
    UNUSED,                     // OxydMagnum floor 0x1f
    UNUSED,                     // OxydMagnum floor 0x20
    UNUSED,                     // OxydMagnum floor 0x21
    UNUSED,                     // OxydMagnum floor 0x22
    "fl-gradient1",             // OxydMagnum floor 0x23
    "fl-gradient2",             // OxydMagnum floor 0x24
    "fl-gradient3",             // OxydMagnum floor 0x25
    "fl-gradient4",             // OxydMagnum floor 0x26
    "fl-gradient5",             // OxydMagnum floor 0x27
    "fl-gradient6",             // OxydMagnum floor 0x28
    "fl-gradient7",             // OxydMagnum floor 0x29
    "fl-gradient8",             // OxydMagnum floor 0x2a
    "fl-gradient9",             // OxydMagnum floor 0x2b
    "fl-gradient10",            // OxydMagnum floor 0x2c
    "fl-gradient11",            // OxydMagnum floor 0x2d
    "fl-gradient12",            // OxydMagnum floor 0x2e
    "fl-gradient13",            // OxydMagnum floor 0x2f
    "fl-gradient14",            // OxydMagnum floor 0x30
    "fl-gradient15",            // OxydMagnum floor 0x31
    "fl-gradient16",            // OxydMagnum floor 0x32
    "fl_brick",                 // OxydMagnum floor 0x33 (common was 'fl-rough')
    "fl_brick",                 // OxydMagnum floor 0x34 (common was 'fl-rough')
    UNUSED,                     // OxydMagnum floor 0x35
    UNUSED,                     // OxydMagnum floor 0x36
    "fl_brick",                 // OxydMagnum floor 0x37
    "fl_brick",                 // OxydMagnum floor 0x38
    UNUSED,                     // OxydMagnum floor 0x39
    UNUSED,                     // OxydMagnum floor 0x3a
    UNUSED,                     // OxydMagnum floor 0x3b
    "fl_rock",                  // OxydMagnum floor 0x3c
    UNUSED,                     // OxydMagnum floor 0x3d
    UNUSED,                     // OxydMagnum floor 0x3e
    0,                          // OxydMagnum floor 0x3f (Level 121 ?)(in normal Oxyd Magnum Levels unused!)
    "fl-wood_framed_h",         // OxydMagnum floor 0x40
    "fl_wood",                  // OxydMagnum floor 0x41
    "fl_wood_h",                // OxydMagnum floor 0x42
    UNUSED,                     // OxydMagnum floor 0x43
    UNUSED,                     // OxydMagnum floor 0x44
    UNUSED,                     // OxydMagnum floor 0x45
    "fl_gravel",                // OxydMagnum floor 0x46
    "fl_gravel",                // OxydMagnum floor 0x47
    UNUSED,                     // OxydMagnum floor 0x48
    UNUSED,                     // OxydMagnum floor 0x49
    UNUSED,                     // OxydMagnum floor 0x4a
    UNUSED,                     // OxydMagnum floor 0x4b
    "fl_stone",                 // OxydMagnum floor 0x4c
    "fl_stone",                 // OxydMagnum floor 0x4d
    UNUSED,                     // OxydMagnum floor 0x4e
    UNUSED,                     // OxydMagnum floor 0x4f
    UNUSED,                     // OxydMagnum floor 0x50
    UNUSED,                     // OxydMagnum floor 0x51
    "fl_bridge",                // OxydMagnum floor 0x52 (common was 'fl-bridge')
    "fl_white",                 // OxydMagnum floor 0x53
    "fl_ivory",                 // OxydMagnum floor 0x54
    "fl_platinum",              // OxydMagnum floor 0x55
    "fl_darkgray",              // OxydMagnum floor 0x56
    UNUSED,                     // OxydMagnum floor 0x57
    "fl_darkgray",              // OxydMagnum floor 0x58
    "fl_lawn",                  // OxydMagnum floor 0x59
    "fl_metal_7n",              // OxydMagnum floor 0x5a (Only Oxyd Magnum Gold #119,120)
    "fl_plank",                 // OxydMagnum floor 0x5b
    // codes >= 0x5c are unused
};

const char *oxyd::oxydmag_stone_map[256] = {
    0,                          // OxydMagnum stone 0x00
    0,0,0,0,0,0,0,0,            // 0x01 -- 0x08  Oxyd stones
    0,0,0,0,0,0,0,0,            // 0x09 -- 0x10  Oxyd stones
    "st_fake_oxydd",            // OxydMagnum stone 0x11
    "st_flat",                  // OxydMagnum stone 0x12
    "st_brick_es",              // OxydMagnum stone 0x13
    "st_brick_sw",              // OxydMagnum stone 0x14
    "st_brick_ne",              // OxydMagnum stone 0x15
    "st_brick_nw",              // OxydMagnum stone 0x16
    UNUSED,                     // OxydMagnum stone 0x17
    UNUSED,                     // OxydMagnum stone 0x18
    "st_lightglass_hollow",     // OxydMagnum stone 0x19
    "st-grate1",                // OxydMagnum stone 0x1a
    "st-grate2",                // OxydMagnum stone 0x1b
    "st-bug",                   // OxydMagnum stone 0x1c
    UNUSED,                     // OxydMagnum stone 0x1d
    UNUSED,                     // OxydMagnum stone 0x1e
    "st_brick",                 // OxydMagnum stone 0x1f
    "st_granite",               // OxydMagnum stone 0x20
    "st_granite",               // OxydMagnum stone 0x21 (Level 121 ?)(Oxyd Magnum Gold)
    "st_granite",               // OxydMagnum stone 0x22 (common was 'st-glass')(Level 73 only)(looks like st-rock1)(looks like 0x20, but not like 0x87)
    "st_woven",                 // OxydMagnum stone 0x23
    "st_redbrown",              // OxydMagnum stone 0x24
    "st_lightglass",            // OxydMagnum stone 0x25
    "st_redfiber",              // OxydMagnum stone 0x26
    "st_lightglass",            // OxydMagnum stone 0x27
    "st_flat_breakable",        // OxydMagnum stone 0x28
    "st_flat_hollow",           // OxydMagnum stone 0x29
    "st_flat_movable",          // OxydMagnum stone 0x2a
    "st_box_wood",              // OxydMagnum stone 0x2b
    "st_switch_instant",        // OxydMagnum stone 0x2c
    "st_floppy",                // OxydMagnum stone 0x2d (in normal Oxyd Magnum Levels unused!)
    UNUSED,                     // OxydMagnum stone 0x2e
    "st_fourswitch",            // OxydMagnum stone 0x2f (Level 89 and 92)
    "st_key",                   // OxydMagnum stone 0x30
    "st_floppy",                // OxydMagnum stone 0x31
    "st_knight",                // OxydMagnum stone 0x32
    "st_fourswitch",            // OxydMagnum stone 0x33 (Level 51 and 100)
    "st_death",                 // OxydMagnum stone 0x34
    "st_death_invisible",       // OxydMagnum stone 0x35
    "st_oneway_black_w",        // OxydMagnum stone 0x36
    "st_oneway_black_e",        // OxydMagnum stone 0x37
    "st_oneway_black_n",        // OxydMagnum stone 0x38
    "st_oneway_black_s",        // OxydMagnum stone 0x39
    "st_oneway_white_w",        // OxydMagnum stone 0x3a
    "st_oneway_white_e",        // OxydMagnum stone 0x3b
    "st_oneway_white_n",        // OxydMagnum stone 0x3c
    "st_oneway_white_s",        // OxydMagnum stone 0x3d
    UNUSED,                     // OxydMagnum stone 0x3e
    UNUSED,                     // OxydMagnum stone 0x3f
    UNUSED,                     // OxydMagnum stone 0x40
    UNUSED,                     // OxydMagnum stone 0x41
    "st_actorimpulse",          // OxydMagnum stone 0x42
    "st_actorimpulse_invisible", // OxydMagnum stone 0x43
    "st-laser-1",               // OxydMagnum stone 0x44  The laser-names are fake names!
    "st-laser-2",               // OxydMagnum stone 0x45  Direction and state are generated by Enigma.
    "st-laser-3",               // OxydMagnum stone 0x46
    "st_mirror_slab_n",         // OxydMagnum stone 0x47
    "st_mirror_slab_e",         // OxydMagnum stone 0x48
    "st_mirror_slab_s",         // OxydMagnum stone 0x49
    "st_mirror_slab_w",         // OxydMagnum stone 0x4a
    "st_mirror_slab_nm",        // OxydMagnum stone 0x4b
    "st_mirror_slab_em",        // OxydMagnum stone 0x4c
    "st_mirror_slab_sm",        // OxydMagnum stone 0x4d
    "st_mirror_slab_wm",        // OxydMagnum stone 0x4e
    "st_mirror_slab_nt",        // OxydMagnum stone 0x4f
    "st_mirror_slab_et",        // OxydMagnum stone 0x50
    "st_mirror_slab_st",        // OxydMagnum stone 0x51
    "st_mirror_slab_wt",        // OxydMagnum stone 0x52
    "st_mirror_triangle_ec",    // OxydMagnum stone 0x53
    "st_mirror_triangle_nc",    // OxydMagnum stone 0x54
    "st_mirror_triangle_wc",    // OxydMagnum stone 0x55
    "st_mirror_triangle_sc",    // OxydMagnum stone 0x56
    "st_puzzle_yellow_es",      // Oxyd1 stone 0x57
    "st_puzzle_yellow_ew",      // Oxyd1 stone 0x58
    "st_puzzle_yellow_nw",      // Oxyd1 stone 0x59
    "st_puzzle_yellow_ne",      // Oxyd1 stone 0x5a
    "st_puzzle_yellow_ew",      // Oxyd1 stone 0x5b
    "st_puzzle_yellow_ns",      // Oxyd1 stone 0x5c
    "st_puzzle_yellow_n",       // Oxyd1 stone 0x5d
    "st_puzzle_yellow_s",       // Oxyd1 stone 0x5e
    "st_puzzle_yellow_w",       // Oxyd1 stone 0x5f
    "st_puzzle_yellow_e",       // Oxyd1 stone 0x60
    "st_puzzle_yellow_nesw",    // Oxyd1 stone 0x61
    "st_metal_hollow",          // OxydMagnum stone 0x62  (st-explosive: like st-metal, but explodes when touched ...)
    "st_flat_breakable",        // OxydMagnum stone 0x63
    "st-fakeoxyda",             // OxydMagnum stone 0x64
    UNUSED,                     // OxydMagnum stone 0x65
    UNUSED,                     // OxydMagnum stone 0x66
    UNUSED,                     // OxydMagnum stone 0x67
    "st-bombs",                 // OxydMagnum stone 0x68 (common was 'st-shogun-l')
    "st-flash",                 // OxydMagnum stone 0x69
    "st_coinslot_instant",      // OxydMagnum stone 0x6a
    "st-thief",                 // OxydMagnum stone 0x6b
    "st_shogun_s",              // OxydMagnum stone 0x6c
    "st_stoneimpulse",          // OxydMagnum stone 0x6d
    "st_laserflop",             // OxydMagnum stone 0x6e
    "st_mail_n",                // OxydMagnum stone 0x6f
    "st_mail_w",                // OxydMagnum stone 0x70
    "st_mail_e",                // OxydMagnum stone 0x71
    "st_mail_s",                // OxydMagnum stone 0x72
    "st_door_d",                // OxydMagnum stone 0x73
    "st_door_d_ew",             // OxydMagnum stone 0x74
    "st_metal",                 // OxydMagnum stone 0x75
    "st_invisible",             // OxydMagnum stone 0x76
    UNUSED,                     // OxydMagnum stone 0x77
    "st_door_d_ew_open",        // OxydMagnum stone 0x78 (st-door-h-open was wrong, look at level #32)
    UNUSED,                     // OxydMagnum stone 0x79
    "st_laserflop_on",          // OxydMagnum stone 0x7a (Level 66, this is a solid stone)
    UNUSED,                     // OxydMagnum stone 0x7b
    UNUSED,                     // OxydMagnum stone 0x7c
    UNUSED,                     // OxydMagnum stone 0x7d
    UNUSED,                     // OxydMagnum stone 0x7e
    UNUSED,                     // OxydMagnum stone 0x7f
    UNUSED,                     // OxydMagnum stone 0x80
    UNUSED,                     // OxydMagnum stone 0x81
    UNUSED,                     // OxydMagnum stone 0x82
    UNUSED,                     // OxydMagnum stone 0x83
    UNUSED,                     // OxydMagnum stone 0x84
    UNUSED,                     // OxydMagnum stone 0x85
    "st_plaster_hollow",        // OxydMagnum stone 0x86
    "st_purplemarble",          // OxydMagnum stone 0x87 (was st-rock1, but the pic has straight edges and surfaces)
    0,                          // OxydMagnum stone 0x88 (Level 121 ?)(in normal Oxyd Magnum Levels unused!)
    0,                          // OxydMagnum stone 0x89 (Level 121 ?)(in normal Oxyd Magnum Levels unused!)
    "st_brownpyramide",         // OxydMagnum stone 0x8a
    "st_purplegray",            // OxydMagnum stone 0x8b
    "st_passage_white_square",  // OxydMagnum stone 0x8c
    "st_passage_black_square",  // OxydMagnum stone 0x8d
    "st-yinyang2",              // OxydMagnum stone 0x8e
    UNUSED,                     // OxydMagnum stone 0x8f
    "st_redrock",               // OxydMagnum stone 0x90 (Level 89 only)
    UNUSED,                     // OxydMagnum stone 0x91
    "st_fourswitch",            // OxydMagnum stone 0x92 (Level 51 only)
    "st-yinyang1",              // OxydMagnum stone 0x93
    "st-break_acwhite",         // OxydMagnum stone 0x94
    "st-break_acblack",         // OxydMagnum stone 0x95
    "st_charge_plus",           // OxydMagnum stone 0x96
    "st_charge_minus",          // OxydMagnum stone 0x97
    "st_charge_zero",           // OxydMagnum stone 0x98
    0,                          // OxydMagnum stone 0x99 (common was 'st-blocker')(Level 121 ?)(in normal Oxyd Magnum Levels unused!)
    "st_turnstile_red",         // OxydMagnum stone 0x9a
    "st_turnstilearm_n",        // OxydMagnum stone 0x9b
    "st_turnstilearm_s",        // OxydMagnum stone 0x9c
    "st_turnstilearm_w",        // OxydMagnum stone 0x9d
    "st_turnstilearm_e",        // OxydMagnum stone 0x9e
    UNUSED,                     // OxydMagnum stone 0x9f
    UNUSED,                     // OxydMagnum stone 0xa0
    UNUSED,                     // OxydMagnum stone 0xa1
    UNUSED,                     // OxydMagnum stone 0xa2
    UNUSED,                     // OxydMagnum stone 0xa3
    "st_stoneimpulse_hollow",   // OxydMagnum stone 0xa4
    UNUSED,                     // OxydMagnum stone 0xa5
    UNUSED,                     // OxydMagnum stone 0xa6
    "st_lightpassenger",        // OxydMagnum stone 0xa7
    UNUSED,                     // OxydMagnum stone 0xa8
    UNUSED,                     // OxydMagnum stone 0xa9
    // codes >= 0xaa are unused
};

const char* oxyd::oxydmag_item_map[256] = {
    IT_INVALID,          // OxydMagnum item 0x00
    "it_extralife",     // OxydMagnum item 0x01
    IT_EXTERNAL,      // OxydMagnum item 0x02 document 1
    IT_EXTERNAL,      // OxydMagnum item 0x03 document 2
    "it_hammer",        // OxydMagnum item 0x04
    "it_coffee",        // OxydMagnum item 0x05
    "it_cherry",        // OxydMagnum item 0x06
    "it_umbrella",      // OxydMagnum item 0x07
    "it_glasses",       // OxydMagnum item 0x08
    "it_glasses_broken", // OxydMagnum item 0x09
    "it-dynamite",      // OxydMagnum item 0x0a
    "it-blackbomb",     // OxydMagnum item 0x0b
    "it-whitebomb",     // OxydMagnum item 0x0c
    "it_crack_i",        // OxydMagnum item 0x0d
    "it_crack_s",        // OxydMagnum item 0x0e
    "it_crack_m",        // OxydMagnum item 0x0f
    "it_crack_l",        // OxydMagnum item 0x10
    "it_coin_s",         // OxydMagnum item 0x11
    "it_coin_m",         // OxydMagnum item 0x12
    "it_coin_l",         // OxydMagnum item 0x13
    "it_key_a",         // OxydMagnum item 0x14
    "it_key_b",         // OxydMagnum item 0x15
    "it_key_c",         // OxydMagnum item 0x16
    "it_floppy",        // OxydMagnum item 0x17
    "it_sword",         // OxydMagnum item 0x18
    "it-flagwhite",     // OxydMagnum item 0x19
    "it-flagblack",     // OxydMagnum item 0x1a
    "it_ring",          // OxydMagnum item 0x1b
    "it_pipe_nw",       // OxydMagnum item 0x1c
    "it_pipe_sw",       // OxydMagnum item 0x1d
    "it_pipe_ne",       // OxydMagnum item 0x1e
    "it_pipe_es",       // OxydMagnum item 0x1f
    "it_pipe_ns",        // OxydMagnum item 0x20
    "it_pipe_ew",        // OxydMagnum item 0x21
    "it_spade",         // OxydMagnum item 0x22
    UNUSED,        // OxydMagnum item 0x23
    "it-pin",           // OxydMagnum item 0x24
    "it_seed",          // OxydMagnum item 0x25
    "it-spring2",       // OxydMagnum item 0x26
    "it-spring1",       // OxydMagnum item 0x27
    "it-bag",           // OxydMagnum item 0x28
    "it_magnet_off",    // OxydMagnum item 0x29
    "it_inversesensor", // OxydMagnum item 0x2a
    "it_sensor",        // OxydMagnum item 0x2b
    "it_shogun_s",      // OxydMagnum item 0x2c
    "it_vortex_open",   // OxydMagnum item 0x2d
    "it_vortex_closed", // OxydMagnum item 0x2e
    "it_wormhole_on",   // OxydMagnum item 0x2f
    "it_meditation_hill",   // OxydMagnum item 0x30
    "it_meditation_bump",   // OxydMagnum item 0x31
    "it_meditation_hollow", // OxydMagnum item 0x32
    "it_meditation_dent",   // OxydMagnum item 0x33
    "it_strip_ns",      // OxydMagnum item 0x34
    "it_strip_ew",      // OxydMagnum item 0x35
    "it-springboard",   // OxydMagnum item 0x36
    IT_MISSING,       // OxydMagnum item 0x37
    "it-bridge-oxyd",   // OxydMagnum item 0x38
    UNUSED,        // OxydMagnum item 0x39
    UNUSED,        // OxydMagnum item 0x3a
    UNUSED,        // OxydMagnum item 0x3b
    UNUSED,        // OxydMagnum item 0x3c
    UNUSED,        // OxydMagnum item 0x3d
    UNUSED,        // OxydMagnum item 0x3e
    UNUSED,        // OxydMagnum item 0x3f
    UNUSED,        // OxydMagnum item 0x40
    UNUSED,        // OxydMagnum item 0x41
    UNUSED,        // OxydMagnum item 0x42
    UNUSED,        // OxydMagnum item 0x43
    UNUSED,        // OxydMagnum item 0x44
    UNUSED,        // OxydMagnum item 0x45
    UNUSED,        // OxydMagnum item 0x46
    UNUSED,        // OxydMagnum item 0x47
    UNUSED,        // OxydMagnum item 0x48
    UNUSED,        // OxydMagnum item 0x49
    UNUSED,        // OxydMagnum item 0x4a
    UNUSED,        // OxydMagnum item 0x4b
    "it-springboard",   // OxydMagnum item 0x4c
    UNUSED,        // OxydMagnum item 0x4d
    UNUSED,        // OxydMagnum item 0x4e
    UNUSED,        // OxydMagnum item 0x4f
    UNUSED,        // OxydMagnum item 0x50
    UNUSED,        // OxydMagnum item 0x51
    UNUSED,        // OxydMagnum item 0x52
    UNUSED,        // OxydMagnum item 0x53
    UNUSED,        // OxydMagnum item 0x54
    UNUSED,        // OxydMagnum item 0x55
    UNUSED,        // OxydMagnum item 0x56
    UNUSED,        // OxydMagnum item 0x57
    UNUSED,        // OxydMagnum item 0x58
    UNUSED,        // OxydMagnum item 0x59
    UNUSED,        // OxydMagnum item 0x5a
    UNUSED,        // OxydMagnum item 0x5b
    UNUSED,        // OxydMagnum item 0x5c
    UNUSED,        // OxydMagnum item 0x5d
    UNUSED,        // OxydMagnum item 0x5e
    "it_sensor_filter1", // OxydMagnum item 0x5f
    "it-drop",          // OxydMagnum item 0x60 (drunk)
    UNUSED,        // OxydMagnum item 0x61 (rev. breaking are)
    UNUSED,        // OxydMagnum item 0x62 (player exchange)
    "it_trigger",       // OxydMagnum item 0x63
    "it-puller-n",      // OxydMagnum item 0x64
    "it-puller-s",      // OxydMagnum item 0x65
    "it-puller-w",      // OxydMagnum item 0x66
    "it-puller-e",      // OxydMagnum item 0x67
    // codes >= 0x68 are unused
};
