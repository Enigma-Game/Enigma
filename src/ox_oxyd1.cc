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

#include "server.hh"
#include "oxyd_internal.hh"

using namespace enigma;
using namespace world;
using namespace oxyd;

/* -------------------- Oxyd 1 level pack -------------------- */

LP_Oxyd1::LP_Oxyd1 (DatFile *dat, bool twoplayers)
: LevelPack_Oxyd (OxydVersion_Oxyd1, dat,
                  twoplayers ? 100 : 0,
                  twoplayers ? 199 : 99,
                  twoplayers)
{
}

void LP_Oxyd1::load (const OxydLib::Level &level)
{
    LoaderConfig c (needs_twoplayers(),
                    get_gamemode(),
                    oxyd1_floor_map,
                    oxyd1_item_map,
                    oxyd1_stone_map);
    c.id_timer  = 0x33;
    c.id_laser1 = 0x44;

    OxydLoader (level, c).load();

    if (server::GetDifficulty() == DIFFICULTY_EASY)
        server::WaterSinkSpeed = 100.0;
    else
        server::WaterSinkSpeed = 200.0;
}

#endif // PLAIN_SPEC_ONLY
// only tables following!

const char *oxyd::oxyd1_floor_map[256] = {
    "fl-abyss",                 // Oxyd1 floor 0x00
    "fl-gray",                  // Oxyd1 floor 0x01
    "fl-metal",                 // Oxyd1 floor 0x02
    "fl-samba",                 // Oxyd1 floor 0x03 was: checkered floor
    UNUSED,                     // Oxyd1 floor 0x04
    UNUSED,                     // Oxyd1 floor 0x05
    UNUSED,                     // Oxyd1 floor 0x06
    "fl-normal",                // Oxyd1 floor 0x07
    "fl-himalaya",              // Oxyd1 floor 0x08
    "fl-himalaya",              // Oxyd1 floor 0x09
    "fl-himalaya",              // Oxyd1 floor 0x0a
    "fl-himalaya",              // Oxyd1 floor 0x0b
    "fl-inverse2",              // Oxyd1 floor 0x0c
    "fl-acblack",               // Oxyd1 floor 0x0d
    "fl-acwhite",               // Oxyd1 floor 0x0e
    "fl-swamp",                 // Oxyd1 floor 0x0f
    UNUSED,                     // Oxyd1 floor 0x10
    UNUSED,                     // Oxyd1 floor 0x11
    UNUSED,                     // Oxyd1 floor 0x12
    "fl-water",                 // Oxyd1 floor 0x13
    UNUSED,                     // Oxyd1 floor 0x14
    UNUSED,                     // Oxyd1 floor 0x15
    UNUSED,                     // Oxyd1 floor 0x16
    "fl-ice",                   // Oxyd1 floor 0x17
    UNUSED,                     // Oxyd1 floor 0x18
    UNUSED,                     // Oxyd1 floor 0x19
    UNUSED,                     // Oxyd1 floor 0x1a
    UNUSED,                     // Oxyd1 floor 0x1b
    "fl-space",                 // Oxyd1 floor 0x1c
    "fl-space",                 // Oxyd1 floor 0x1d
    UNUSED,                     // Oxyd1 floor 0x1e
    UNUSED,                     // Oxyd1 floor 0x1f
    "fl-space",                 // Oxyd1 floor 0x20
    "fl-space-force",           // Oxyd1 floor 0x21
    "fl-space",                 // Oxyd1 floor 0x22
    "fl-gradient1",             // Oxyd1 floor 0x23
    "fl-gradient2",             // Oxyd1 floor 0x24
    "fl-gradient3",             // Oxyd1 floor 0x25
    "fl-gradient4",             // Oxyd1 floor 0x26
    "fl-gradient5",             // Oxyd1 floor 0x27
    "fl-gradient6",             // Oxyd1 floor 0x28
    "fl-gradient7",             // Oxyd1 floor 0x29
    "fl-gradient8",             // Oxyd1 floor 0x2a
    "fl-gradient9",             // Oxyd1 floor 0x2b
    "fl-gradient10",            // Oxyd1 floor 0x2c
    "fl-gradient11",            // Oxyd1 floor 0x2d
    "fl-gradient12",            // Oxyd1 floor 0x2e
    "fl-gradient13",            // Oxyd1 floor 0x2f
    "fl-gradient14",            // Oxyd1 floor 0x30
    "fl-gradient15",            // Oxyd1 floor 0x31
    "fl-gradient16",            // Oxyd1 floor 0x32
    "fl-brick",                 // Oxyd1 floor 0x33
    "fl-brick",                 // Oxyd1 floor 0x34 (common was 'fl-rough')
    UNUSED,                     // Oxyd1 floor 0x35
    UNUSED,                     // Oxyd1 floor 0x36
    "fl-brick",                 // Oxyd1 floor 0x37
    "fl-brick",                 // Oxyd1 floor 0x38
    UNUSED,                     // Oxyd1 floor 0x39
    UNUSED,                     // Oxyd1 floor 0x3a
    UNUSED,                     // Oxyd1 floor 0x3b
    "fl-rock",                  // Oxyd1 floor 0x3c
    UNUSED,                     // Oxyd1 floor 0x3d
    UNUSED,                     // Oxyd1 floor 0x3e
    UNUSED,                     // Oxyd1 floor 0x3f
    "fl-stwood1",               // Oxyd1 floor 0x40
    "fl-wood",                  // Oxyd1 floor 0x41
    "fl-samba1",                // Oxyd1 floor 0x42
    UNUSED,                     // Oxyd1 floor 0x43
    UNUSED,                     // Oxyd1 floor 0x44
    UNUSED,                     // Oxyd1 floor 0x45
    "fl-gravel",                // Oxyd1 floor 0x46
    "fl-gravel",                // Oxyd1 floor 0x47
    UNUSED,                     // Oxyd1 floor 0x48
    UNUSED,                     // Oxyd1 floor 0x49
    UNUSED,                     // Oxyd1 floor 0x4a
    UNUSED,                     // Oxyd1 floor 0x4b
    UNUSED,                     // Oxyd1 floor 0x4c
    "fl-tigris",                // Oxyd1 floor 0x4d
    UNUSED,                     // Oxyd1 floor 0x4e
    UNUSED,                     // Oxyd1 floor 0x4f
    UNUSED,                     // Oxyd1 floor 0x50
    UNUSED,                     // Oxyd1 floor 0x51
    "fl_bridge",                // Oxyd1 floor 0x52
    "fl-springboard",           // Oxyd1 floor 0x53
    UNUSED,                     // Oxyd1 floor 0x54
    "fl-gray",                  // Oxyd1 floor 0x55
    "fl-light",                 // Oxyd1 floor 0x56
    "fl-lightgray",             // Oxyd1 floor 0x57
    "fl-darkgray",              // Oxyd1 floor 0x58
    "fl-dunes",                 // Oxyd1 floor 0x59
    "fl-normal",                // Oxyd1 floor 0x5a
    "fl-plank",                 // Oxyd1 floor 0x5b
    // codes >= 0x5c are unused
};

const char *oxyd::oxyd1_stone_map[256] = {
    0,                          // Oxyd1 stone 0x00
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    "st-fakeoxyd",              // Oxyd1 stone 0x11
    "st_door_c",                // Oxyd1 stone 0x12
    "st_brick_es",              // Oxyd1 stone 0x13
    "st_brick_sw",              // Oxyd1 stone 0x14
    "st_brick_ne"  ,            // Oxyd1 stone 0x15
    "st_brick_nw",              // Oxyd1 stone 0x16
    "st-plain_hole",            // Oxyd1 stone 0x17
    "st-oxyd-0x18",             // Oxyd1 stone 0x18
    "st_door_c_open",           // Oxyd1 stone 0x19
    "st-grate1",                // Oxyd1 stone 0x1a
    "st-grate2",                // Oxyd1 stone 0x1b
    "st-bug",                   // Oxyd1 stone 0x1c
    "st-surprise",              // Oxyd1 stone 0x1d
    "st-chameleon",             // Oxyd1 stone 0x1e
    "st_brick",                 // Oxyd1 stone 0x1f
    "st-rock1",                 // Oxyd1 stone 0x20
    "st-rock1",                 // Oxyd1 stone 0x21
    UNUSED,                     // Oxyd1 stone 0x22
    "st-woven",                 // Oxyd1 stone 0x23
    "st-marble",                // Oxyd1 stone 0x24
    "st-glass1",                // Oxyd1 stone 0x25
    "st-glass3",                // Oxyd1 stone 0x26
    "st-glass2",                // Oxyd1 stone 0x27
    "st-plain_break",           // Oxyd1 stone 0x28 [see Level#12]
    "st-plain_hole",            // Oxyd1 stone 0x29
    "st-plain_move",            // Oxyd1 stone 0x2a
    "st-wood",                  // Oxyd1 stone 0x2b
    "st_switch_instant",        // Oxyd1 stone 0x2c
    "st_switch_black_instant",  // Oxyd1 stone 0x2d
    "st_switch_white_instant",  // Oxyd1 stone 0x2e
    "st_fourswitch",            // Oxyd1 stone 0x2f
    "st_key",                   // Oxyd1 stone 0x30
    "st_floppy",                // Oxyd1 stone 0x31
    "st_knight",                // Oxyd1 stone 0x32
    0,                          // Oxyd1 stone 0x33 oscillator (properties set in oxyd.cc)
    "st_death",                 // Oxyd1 stone 0x34
    "st_death_invisible",       // Oxyd1 stone 0x35
    "st_oneway_black_w",        // Oxyd1 stone 0x36
    "st_oneway_black_e",        // Oxyd1 stone 0x37
    "st_oneway_black_n",        // Oxyd1 stone 0x38
    "st_oneway_black_s",        // Oxyd1 stone 0x39
    "st_oneway_white_w",        // Oxyd1 stone 0x3a
    "st_oneway_white_e",        // Oxyd1 stone 0x3b
    "st_oneway_white_n",        // Oxyd1 stone 0x3c
    "st_oneway_white_s",        // Oxyd1 stone 0x3d
    "st_window",                // Oxyd1 stone 0x3e
    "",                         // Oxyd1 stone 0x3f magic stone
    "",                         // Oxyd1 stone 0x40 magic stone
    "",                         // Oxyd1 stone 0x41 magic stone
    "st_actorimpulse",          // Oxyd1 stone 0x42
    "st_actorimpulse_invisible", // Oxyd1 stone 0x43
    "st-laser-1",               // Oxyd1 stone 0x44  The laser-names are fake names!
    "st-laser-2",               // Oxyd1 stone 0x45  Direction and state are generated by Enigma.
    "st-laser-3",               // Oxyd1 stone 0x46
    "st_mirror_slab_n",         // Oxyd1 stone 0x47
    "st_mirror_slab_e",         // Oxyd1 stone 0x48
    "st_mirror_slab_s",         // Oxyd1 stone 0x49
    "st_mirror_slab_w",         // Oxyd1 stone 0x4a
    "st_mirror_slab_nm",        // Oxyd1 stone 0x4b
    "st_mirror_slab_em",        // Oxyd1 stone 0x4c
    "st_mirror_slab_sm",        // Oxyd1 stone 0x4d
    "st_mirror_slab_wm",        // Oxyd1 stone 0x4e
    "st_mirror_slab_nt",        // Oxyd1 stone 0x4f
    "st_mirror_slab_et",        // Oxyd1 stone 0x50
    "st_mirror_slab_st",        // Oxyd1 stone 0x51
    "st_mirror_slab_wt",        // Oxyd1 stone 0x52
    "st_mirror_triangle_ec",    // Oxyd1 stone 0x53
    "st_mirror_triangle_nc",    // Oxyd1 stone 0x54
    "st_mirror_triangle_wc",    // Oxyd1 stone 0x55
    "st_mirror_triangle_sc",    // Oxyd1 stone 0x56
    "st_puzzle_yellow_es",      // Oxyd1 stone 0x57
    "st_puzzle_yellow_sw",      // Oxyd1 stone 0x58
    "st_puzzle_yellow_nw",      // Oxyd1 stone 0x59
    "st_puzzle_yellow_ne",      // Oxyd1 stone 0x5a
    "st_puzzle_yellow_ew",      // Oxyd1 stone 0x5b
    "st_puzzle_yellow_ns",      // Oxyd1 stone 0x5c
    "st_puzzle_yellow_n",       // Oxyd1 stone 0x5d
    "st_puzzle_yellow_s",       // Oxyd1 stone 0x5e
    "st_puzzle_yellow_w",       // Oxyd1 stone 0x5f
    "st_puzzle_yellow_e",       // Oxyd1 stone 0x60
    "st_puzzle_yellow_nesw",    // Oxyd1 stone 0x61
    "st-metal_hole",            // Oxyd1 stone 0x62
    "st-laserbreak",            // Oxyd1 stone 0x63
    "st-fakeoxyda",             // Oxyd1 stone 0x64
    UNUSED,                     // Oxyd1 stone 0x65
    "st-disco-medium",          // Oxyd1 stone 0x66
    "st-disco-light",           // Oxyd1 stone 0x67
    "st-bombs",                 // Oxyd1 stone 0x68
    "st-flash",                 // Oxyd1 stone 0x69
    "st_coinslot_instant",      // Oxyd1 stone 0x6a
    "st-thief",                 // Oxyd1 stone 0x6b
    "st_shogun_s",              // Oxyd1 stone 0x6c
    "st-stoneimpulse",          // Oxyd1 stone 0x6d
    "st_laserflop",             // Oxyd1 stone 0x6e
    "st_mail_n",                // Oxyd1 stone 0x6f
    "st_mail_w",                // Oxyd1 stone 0x70
    "st_mail_e",                // Oxyd1 stone 0x71
    "st_mail_s",                // Oxyd1 stone 0x72
    "st_door_d",                // Oxyd1 stone 0x73
    "st_door_d_ew",             // Oxyd1 stone 0x74
    "st-metal",                 // Oxyd1 stone 0x75
    "st-invisible",             // Oxyd1 stone 0x76
    "st_door_d_open",           // Oxyd1 stone 0x77
    "st_door_d_ew_open",        // Oxyd1 stone 0x78
    UNUSED,                     // Oxyd1 stone 0x79
    "st_laserflop_on",          // Oxyd1 stone 0x7a (only used in level #23)
    UNUSED,                     // Oxyd1 stone 0x7b
    UNUSED,                     // Oxyd1 stone 0x7c
    UNUSED,                     // Oxyd1 stone 0x7d
    UNUSED,                     // Oxyd1 stone 0x7e
    UNUSED,                     // Oxyd1 stone 0x7f
    UNUSED,                     // Oxyd1 stone 0x80
    "",                         // Oxyd1 stone 0x81
    UNUSED,                     // Oxyd1 stone 0x82
    UNUSED,                     // Oxyd1 stone 0x83
    UNUSED,                     // Oxyd1 stone 0x84
    UNUSED,                     // Oxyd1 stone 0x85
    "st-metal_hole",            // Oxyd1 stone 0x86
    "st-rock2",                 // Oxyd1 stone 0x87
    UNUSED,                     // Oxyd1 stone 0x88
    UNUSED,                     // Oxyd1 stone 0x89
    "st-stone1",                // Oxyd1 stone 0x8a
    "st_bluesand",              // Oxyd1 stone 0x8b
    "st-white1",                // Oxyd1 stone 0x8c
    "st-black1",                // Oxyd1 stone 0x8d
    "st-yinyang2",              // Oxyd1 stone 0x8e
    UNUSED,                     // Oxyd1 stone 0x8f
    "st-whiteballs",            // Oxyd1 stone 0x90
    "st-blackballs",            // Oxyd1 stone 0x91
    "st_fourswitch",            // Oxyd1 stone 0x92
    "st-yinyang1",              // Oxyd1 stone 0x93
    "st-break_acwhite",         // Oxyd1 stone 0x94
    "st-break_acblack",         // Oxyd1 stone 0x95
    "st-chargeplus",            // Oxyd1 stone 0x96
    "st-chargeminus",           // Oxyd1 stone 0x97
    "st-chargezero",            // Oxyd1 stone 0x98
    "st-glass1",                // Oxyd1 stone 0x99
    // codes >= 0x9a are unused
};

const char *oxyd::oxyd1_item_map[256] = {
    IT_INVALID,                   // 0x00
    "it_extralife",               // 0x01
    IT_EXTERNAL,                  // 0x02 document 1
    IT_EXTERNAL,                  // 0x03 document 2
    "it_hammer",                  // 0x04
    "it-coffee",                  // 0x05
    "it-cherry",                  // 0x06
    "it_umbrella",                // 0x07
    "it_glasses",                 // 0x08
    "it_glasses_broken",          // 0x09
    "it-dynamite",                // 0x0a
    "it-blackbomb",               // 0x0b
    "it-whitebomb",               // 0x0c
    "it-crack0",                  // 0x0d
    "it-crack1",                  // 0x0e
    "it-crack2",                  // 0x0f
    "it-crack3",                  // 0x10
    "it_coin_s",                  // 0x11
    "it_coin_m",                  // 0x12
    "it_coin_l",                  // 0x13
    "it_key_a",                   // 0x14
    "it_key_b",                   // 0x15
    "it_key_c",                   // 0x16
    "it_floppy",                  // 0x17
    "it_sword",                   // 0x18
    "it-flagwhite",               // 0x19
    "it-flagblack",               // 0x1a
    "it-ring",                    // 0x1b
    "it_pipe_nw",                 // 0x1c
    "it_pipe_sw",                 // 0x1d
    "it_pipe_ne",                 // 0x1e
    "it_pipe_es",                 // 0x1f
    "it_pipe_ns",                 // 0x20
    "it_pipe_ew",                 // 0x21
    "it-spade",                   // 0x22
    "it-surprise",                // 0x23
    "it-pin",                     // 0x24
    "it_seed",                    // 0x25
    "it-spring2",                 // 0x26
    "it-spring1",                 // 0x27
    "it-bag",                     // 0x28
    "it_magnet_off",              // 0x29
    "it_inversesensor",           // 0x2a
    "it_sensor",                  // 0x2b
    "it_shogun_s",                // 0x2c
    "it_vortex_open",             // 0x2d
    "it_vortex_closed",           // 0x2e
    "it_wormhole_on",             // 0x2f
    "it_meditation_hill",         // 0x30
    "it_meditation_bump",         // 0x31
    "it_meditation_hollow",       // 0x32
    "it_meditation_dent",         // 0x33
    "it_strip_ns",                // 0x34
    "it_strip_ew",                // 0x35
    "it-springboard",             // 0x36
    "it-bridge-oxyd",             // 0x37 bridge active
    "it-bridge-oxyd",             // 0x38 bridge inactive
    "it-bridge-oxyd_active",      // 0x39 walkable bridge (?"
    UNUSED,                  // 0x3a
    UNUSED,                  // 0x3b
    UNUSED,                  // 0x3c
    UNUSED,                  // 0x3d
    UNUSED,                  // 0x3e
    UNUSED,                  // 0x3f
    UNUSED,                  // 0x40
    UNUSED,                  // 0x41
    UNUSED,                  // 0x42
    UNUSED,                  // 0x43
    UNUSED,                  // 0x44
    UNUSED,                  // 0x45
    UNUSED,                  // 0x46
    UNUSED,                  // 0x47
    UNUSED,                  // 0x48
    UNUSED,                  // 0x49
    UNUSED,                  // 0x4a
    UNUSED,                  // 0x4b
    UNUSED,                  // 0x4c
    UNUSED,                  // 0x4d
    UNUSED,                  // 0x4e
    UNUSED,                  // 0x4f
    UNUSED,                  // 0x50
    UNUSED,                  // 0x51
    UNUSED,                  // 0x52
    UNUSED,                  // 0x53
    UNUSED,                  // 0x54
    UNUSED,                  // 0x55
    UNUSED,                  // 0x56
    UNUSED,                  // 0x57
    UNUSED,                  // 0x58
    UNUSED,                  // 0x59
    UNUSED,                  // 0x5a
    UNUSED,                  // 0x5b
    UNUSED,                  // 0x5c
    UNUSED,                  // 0x5d
    UNUSED,                  // 0x5e
    "it_sensor",                  // 0x5f
    "it-drop",                    // 0x60    drop (turns actor into rotor)
    // codes >= 0x61 are unused
};
