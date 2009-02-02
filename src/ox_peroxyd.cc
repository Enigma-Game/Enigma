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
using namespace oxyd;

Stone *PerOxydLoader::make_stone (int type, int x, int y)
{
    Stone *st = 0;
    switch (type) {
    case 0x39: case 0x3a: case 0x3b:
        // Create magic stones only if they are absolutely necessary
        if (level.getRequireMagicPiece())
            st = MakeStone ("st_dongle");
        break;
    default:
        st = OxydLoader::make_stone (type, x, y);
        break;
    }
    return st;
}



/* -------------------- Per.Oxyd level pack -------------------- */

LP_PerOxyd::LP_PerOxyd (DatFile *dat, bool twoplayers)
: LevelPack_Oxyd (OxydVersion_PerOxyd, dat,
                  twoplayers ? 100 : 0,
                  twoplayers ? 199 : 99,
                  twoplayers)
{
}

void LP_PerOxyd::load (const OxydLib::Level &level)
{
    LoaderConfig c (needs_twoplayers(),
                    get_gamemode(),
                    peroxyd_floor_map,
                    peroxyd_item_map,
                    peroxyd_stone_map);
    c.id_timer  = 0x2e;
    c.id_laser1 = 0x3e;
    PerOxydLoader (level, c).load();
}

bool LP_PerOxyd::hasEasymode(size_t index) {
    static const bool easymode_map[200] = { // 1-100 single, 101 - 200 netplay
        false, false, true,  false, false, false, false, true,  true,  true,  //   1 -  10
        true,  false, false, true,  false, false, true,  false, false, false, //  11 -  20
        false, false, false, true,  false, true,  true,  false, true,  true,  //  21 -  30
        true,  false, true,  true,  false, false, true,  true,  false, true,  //  31 -  40
        true,  true,  true,  false, true,  false, true,  true,  false, true,  //  41 -  50
        true,  true,  false, true,  true,  true,  false, true,  false, true,  //  51 -  60
        true,  false, false, false, true,  false, true,  true,  false, true,  //  61 -  70
        false, true,  true,  true,  true,  true,  true,  true,  true,  true,  //  71 -  80
        true,  true,  true,  true,  false, true,  false, true,  true,  true,  //  81 -  90
        false, true,  false, true,  true,  true,  true,  true,  true,  true,  //  91 - 100
        false, false, true,  true,  true,  false, false, true,  false, true,  // 101 - 110
        false, true,  false, true,  true,  true,  false, false, false, false, // 111 - 120
        true,  false, false, true,  false, false, false, false, true,  false, // 121 - 130
        false, false, true,  true,  false, true,  false, true,  true,  true,  // 131 - 140
        false, false, true,  false, false, true,  true,  true,  true,  false, // 141 - 150
        true,  true,  true,  true,  false, true,  true,  true,  false, false, // 151 - 160
        true,  true,  true,  true,  true,  true,  false, true,  true,  true,  // 161 - 170
        false, true,  true,  true,  true,  true,  true,  true,  true,  true,  // 171 - 180
        false, true,  false, true,  true,  true,  false, true,  false, true,  // 181 - 190
        false, true,  false, false, true,  true,  true,  true,  true,  true   // 191 - 200
    };
    return easymode_map[index];
}

#endif // PLAIN_SPEC_ONLY
// only tables following!

const char *oxyd::peroxyd_floor_map[256] = {
    "fl_abyss",                 // PerOxyd floor 0x00
    "fl_gray",                  // PerOxyd floor 0x01
    "fl_metal",                 // PerOxyd floor 0x02
    "fl_metal",                 // PerOxyd floor 0x03
    "fl_metal",                 // PerOxyd floor 0x04
    "fl_metal",                 // PerOxyd floor 0x05
    "fl_metal",                 // PerOxyd floor 0x06
    "fl_metal",                 // PerOxyd floor 0x07
    "fl_redslab",               // PerOxyd floor 0x08
    "fl_blueslab",              // PerOxyd floor 0x09
    "fl_blueslab",              // PerOxyd floor 0x0a
    "fl_redslab",               // PerOxyd floor 0x0b
    "fl_inverse",               // PerOxyd floor 0x0c
    "fl_yinyang_yin",           // PerOxyd floor 0x0d
    "fl_yinyang_yang",          // PerOxyd floor 0x0e
    "fl_swamp",                 // PerOxyd floor 0x0f
    UNUSED,                     // PerOxyd floor 0x10
    UNUSED,                     // PerOxyd floor 0x11
    UNUSED,                     // PerOxyd floor 0x12
    "fl_water",                 // PerOxyd floor 0x13
    "fl_water",                 // PerOxyd floor 0x14
    UNUSED,                     // PerOxyd floor 0x15
    UNUSED,                     // PerOxyd floor 0x16
    "fl_ice",                   // PerOxyd floor 0x17
    UNUSED,                     // PerOxyd floor 0x18
    UNUSED,                     // PerOxyd floor 0x19
    UNUSED,                     // PerOxyd floor 0x1a
    UNUSED,                     // PerOxyd floor 0x1b
    "fl_space",                 // PerOxyd floor 0x1c
    UNUSED,                     // PerOxyd floor 0x1d
    UNUSED,                     // PerOxyd floor 0x1e
    UNUSED,                     // PerOxyd floor 0x1f
    UNUSED,                     // PerOxyd floor 0x20
    "fl_space_force",           // PerOxyd floor 0x21 (flat force)
    UNUSED,                     // PerOxyd floor 0x22
    "fl-gradient1",             // PerOxyd floor 0x23
    "fl-gradient2",             // PerOxyd floor 0x24
    "fl-gradient3",             // PerOxyd floor 0x25
    "fl-gradient4",             // PerOxyd floor 0x26
    "fl-gradient5",             // PerOxyd floor 0x27
    "fl-gradient6",             // PerOxyd floor 0x28
    "fl-gradient7",             // PerOxyd floor 0x29
    "fl-gradient8",             // PerOxyd floor 0x2a
    "fl-gradient9",             // PerOxyd floor 0x2b
    "fl-gradient10",            // PerOxyd floor 0x2c
    "fl-gradient11",            // PerOxyd floor 0x2d
    "fl-gradient12",            // PerOxyd floor 0x2e
    "fl-gradient13",            // PerOxyd floor 0x2f (flat force)
    "fl-gradient14",            // PerOxyd floor 0x30 (flat force)
    "fl-gradient15",            // PerOxyd floor 0x31 (flat force)
    "fl-gradient16",            // PerOxyd floor 0x32 (flat force)
    "fl_rough",                 // PerOxyd floor 0x33 (common was 'fl-rough', should be a 'fl-paper')(only level 75)
    "fl_rough",                 // PerOxyd floor 0x34
    UNUSED,                     // PerOxyd floor 0x35
    UNUSED,                     // PerOxyd floor 0x36
    UNUSED,                     // PerOxyd floor 0x37
    "fl_concrete",              // PerOxyd floor 0x38 (Tan/green horizontal Grain)
    "fl_concrete",              // PerOxyd floor 0x39 (Tan/green horizontal Grain, same as 0x38)(only level 118)
    UNUSED,                     // PerOxyd floor 0x3a
    UNUSED,                     // PerOxyd floor 0x3b
    "fl_wood_framed_h",         // PerOxyd floor 0x3c
    "fl_samba",                 // PerOxyd floor 0x3d (common was 'fl-wood', horizontal or vertical slats)(many)
    "fl_samba_h",               // PerOxyd floor 0x3e (common was 'fl-wood', horizontal slats)(only level 32)
    "fl_samba_v",               // PerOxyd floor 0x3f (common was 'fl-wood', vertical slats)(levels: 35, 152)
    UNUSED,                     // PerOxyd floor 0x40
    UNUSED,                     // PerOxyd floor 0x41
    "fl_pinkbumps",             // PerOxyd floor 0x42
    "fl_pinkbumps",             // PerOxyd floor 0x43
    UNUSED,                     // PerOxyd floor 0x44
    UNUSED,                     // PerOxyd floor 0x45
    UNUSED,                     // PerOxyd floor 0x46
    UNUSED,                     // PerOxyd floor 0x47
    "fl_concrete",              // PerOxyd floor 0x48
    UNUSED,                     // PerOxyd floor 0x49
    UNUSED,                     // PerOxyd floor 0x4a
    UNUSED,                     // PerOxyd floor 0x4b
    "fl_bridge",                // PerOxyd floor 0x4c
    UNUSED,                     // PerOxyd floor 0x4d
    "fl_gray",                  // PerOxyd floor 0x4e
    "fl-gradient13",            // PerOxyd floor 0x4f (flat force up, beige)        (maybe gradient22?)
    "fl_dark",                  // PerOxyd floor 0x50 (flat force down, dark olive) (maybe gradient21?)
    "fl_bright",                // PerOxyd floor 0x51 (flat force left, beige)      (maybe gradient24?)
    "fl_dark",                  // PerOxyd floor 0x52 (flat force right, dark olive)(maybe gradient23?)
    "fl_bast",                  // PerOxyd floor 0x53
    UNUSED,                     // PerOxyd floor 0x54
    "fl_plank",                 // PerOxyd floor 0x55
    UNUSED,                     // PerOxyd floor 0x56
    "fl_adhesionless",          // PerOxyd floor 0x57 (only used in Link-Level 91, friction (2.5) questionable!)
    UNUSED,                     // PerOxyd floor 0x58
    UNUSED,                     // PerOxyd floor 0x59
    UNUSED,                     // PerOxyd floor 0x5a
    "fl_gravel",                // PerOxyd floor 0x5b
    UNUSED,                     // PerOxyd floor 0x5c
    UNUSED,                     // PerOxyd floor 0x5d
    "fl_gravel",                // PerOxyd floor 0x5e (same as 0x5b)(only level 192)
    // codes >= 0x5f are unused
};

const char *oxyd::peroxyd_stone_map[256] = {
    0,                          // PerOxyd stone 0x00 no stone
    0,0,0,0,0,0,0,0,            // PerOxyd stone 0x01 - 0x08   oxyd stones
    0,0,0,0,0,0,0,0,            // PerOxyd stone 0x08 - 0x10   oxyd stones
    "st_fake_oxydc",            // PerOxyd stone 0x11
    "st_flat_breakable",        // PerOxyd stone 0x12   no, should be destructible with hammer
    "st_purplemarble",          // PerOxyd stone 0x13
    "st_beads",                 // PerOxyd stone 0x14
    "st_tigris",                // PerOxyd stone 0x15
    "st_greengray",             // PerOxyd stone 0x16
    "st-white1",                // PerOxyd stone 0x17 (Looks a little bit different, but function should be the same.)
    "st_grate_cross",           // PerOxyd stone 0x18
    UNUSED,                     // PerOxyd stone 0x19
    "st_surprise",              // PerOxyd stone 0x1a
    "st_chameleon",             // PerOxyd stone 0x1b (levels: 23 142 178 183)
    "st_magic_brick",           // PerOxyd stone 0x1c
    "st_camouflage",            // PerOxyd stone 0x1d (green camouflage piece)(if st-camouflage is implemented, put it in here)
    "st_camouflage",            // PerOxyd stone 0x1e (green camouflage piece)
    UNUSED,                     // PerOxyd stone 0x1f
    "st_redmarble",             // PerOxyd stone 0x20
    "st_greenbrown",            // PerOxyd stone 0x21
    "st_rawglass",              // PerOxyd stone 0x22 (semi opaque piece, not really glass)
    "st_rawglass",              // PerOxyd stone 0x23 (must be st-glass, see link level 92)
    "st_flat_breakable",        // PerOxyd stone 0x24
    "st_bluegray_hollow",       // PerOxyd stone 0x25
    "st_flat_movable",          // PerOxyd stone 0x26
    "st_box_wood",              // PerOxyd stone 0x27
    "st_switch_instant",        // PerOxyd stone 0x28
    "st_switch_black_instant",  // PerOxyd stone 0x29
    "st_switch_white_instant",  // PerOxyd stone 0x2a
    "st_fourswitch",            // PerOxyd stone 0x2b
    "st_key",                   // PerOxyd stone 0x2c
    "st_floppy",                // PerOxyd stone 0x2d
    "",                         // PerOxyd stone 0x2e (?)
    "st_death",                 // PerOxyd stone 0x2f
    "st_death_invisible",       // PerOxyd stone 0x30
    "st_oneway_black_w",        // PerOxyd stone 0x31
    "st_oneway_black_e",        // PerOxyd stone 0x32
    "st_oneway_black_n",        // PerOxyd stone 0x33
    "st_oneway_black_s",        // PerOxyd stone 0x34
    "st_oneway_white_w",        // PerOxyd stone 0x35
    "st_oneway_white_e",        // PerOxyd stone 0x36
    "st_oneway_white_n",        // PerOxyd stone 0x37
    "st_oneway_white_s",        // PerOxyd stone 0x38
    "st_dongle",                 // PerOxyd stone 0x39 ('st-magic')
    "st_dongle",                 // PerOxyd stone 0x3a ('st-magic')
    "st_dongle",                 // PerOxyd stone 0x3b ('st-magic')
    "st_actorimpulse",          // PerOxyd stone 0x3c
    "st_actorimpulse_invisible", // PerOxyd stone 0x3d
    "st-laser-1",               // PerOxyd stone 0x3e  The laser-names are fake names!
    "st-laser-2",               // PerOxyd stone 0x3f  Direction and state are generated by Enigma.
    "st-laser-3",               // PerOxyd stone 0x40
    "st_mirror_slab_n",         // PerOxyd stone 0x41
    "st_mirror_slab_e",         // PerOxyd stone 0x42
    "st_mirror_slab_s",         // PerOxyd stone 0x43
    "st_mirror_slab_w",         // PerOxyd stone 0x44
    UNUSED,                     // PerOxyd stone 0x45
    "st_mirror_slab_em",        // PerOxyd stone 0x46
    "st_mirror_slab_sm",        // PerOxyd stone 0x47
    "st_mirror_slab_wm",        // PerOxyd stone 0x48
    "st_mirror_slab_nt",        // PerOxyd stone 0x49
    "st_mirror_slab_et",        // PerOxyd stone 0x4a
    "st_mirror_slab_st",        // PerOxyd stone 0x4b
    "st_mirror_slab_wt",        // PerOxyd stone 0x4c
    "st_mirror_triangle_ec",    // PerOxyd stone 0x4d
    "st_mirror_triangle_nc",    // PerOxyd stone 0x4e
    "st_mirror_triangle_wc",    // PerOxyd stone 0x4f
    "st_mirror_triangle_sc",    // PerOxyd stone 0x50
    "st_puzzle_blue_es",        // PerOxyd stone 0x51
    "st_puzzle_blue_sw",        // PerOxyd stone 0x52
    "st_puzzle_blue_nw",        // PerOxyd stone 0x53
    "st_puzzle_blue_ne",        // PerOxyd stone 0x54
    "st_puzzle_blue_ew",        // PerOxyd stone 0x55
    "st_puzzle_blue_ns",        // PerOxyd stone 0x56
    "st_puzzle_blue_n",         // PerOxyd stone 0x57
    "st_puzzle_blue_s",         // PerOxyd stone 0x58
    "st_puzzle_blue_w",         // PerOxyd stone 0x59
    "st_puzzle_blue_e",         // PerOxyd stone 0x5a
    "st_puzzle_blue_nesw",      // PerOxyd stone 0x5b
    "st_puzzle_blue_nesw_hollow", // PerOxyd stone 0x5c
    "st_break_laser",           // PerOxyd stone 0x5d
    "st_magic_oxydc",           // PerOxyd stone 0x5e
    "st_shogun",                // PerOxyd stone 0x5f (oxyd with a hole, movable ... strange stone! st-shogun as workaround, only link level 74)
    "st_disco_dark",            // PerOxyd stone 0x60
    "st_disco_medium",          // PerOxyd stone 0x61
    "st-bombs",                 // PerOxyd stone 0x62
    "st_flash",                 // PerOxyd stone 0x63
    "st_coinslot_instant",      // PerOxyd stone 0x64
    "st_thief",                 // PerOxyd stone 0x65
    "st_shogun_s",              // PerOxyd stone 0x66
    "st_shogun_m",              // PerOxyd stone 0x67
    "st_shogun_l",              // PerOxyd stone 0x68
    "st_shogun_sml",            // PerOxyd stone 0x69
    "st_shogun_ml",             // PerOxyd stone 0x6a
    "st_shogun_sl",             // PerOxyd stone 0x6b
    "st_shogun_sm",             // PerOxyd stone 0x6c
    "st_stoneimpulse",          // PerOxyd stone 0x6d
    "st_laserflop",             // PerOxyd stone 0x6e
    "st_mail_n",                // PerOxyd stone 0x6f
    "st_mail_w",                // PerOxyd stone 0x70
    "st_mail_e",                // PerOxyd stone 0x71
    "st_mail_s",                // PerOxyd stone 0x72
    "st_door_d",                // PerOxyd stone 0x73
    "st_door_d_ew",             // PerOxyd stone 0x74
    "st_metal",                 // PerOxyd stone 0x75
    "st_ghost_purplemarble",    // PerOxyd stone 0x76
    "st_door_d_open",           // PerOxyd stone 0x77
    "st_door_d_ew_open",        // PerOxyd stone 0x78
    "st_passage_white_square",  // PerOxyd stone 0x79 (Can be either st-white1 or st-white4)
    "st_passage_black_square",  // PerOxyd stone 0x7a (Can be either st-black1 or st-black4)
    "st_metal_hollow",          // PerOxyd stone 0x7b (because it looks similar ...; only link level 79)
    0,                          // PerOxyd stone 0x7c (probably complex stone, behavior unknown)(only levels: 23 142, these levels are identical!)
    "st_blur_magic",            // PerOxyd stone 0x7d
    "st_break_white",           // PerOxyd stone 0x7e
    "st_break_black",           // PerOxyd stone 0x7f
    "st_charge_plus",           // PerOxyd stone 0x80
    "st_charge_minus",          // PerOxyd stone 0x81
    "st_charge_zero",           // PerOxyd stone 0x82
    "st_boulder_n",             // PerOxyd stone 0x83
    "st_boulder_s",             // PerOxyd stone 0x84
    "st_boulder_w",             // PerOxyd stone 0x85
    "st_boulder_e",             // PerOxyd stone 0x86
    "st_tigris",                // PerOxyd stone 0x87
    UNUSED,                     // PerOxyd stone 0x88
    UNUSED,                     // PerOxyd stone 0x89
    "st_purplegray",            // PerOxyd stone 0x8a
    "st_bluesand",              // PerOxyd stone 0x8b
    "st_granite",               // PerOxyd stone 0x8c
    "st_darkgray",              // PerOxyd stone 0x8d (simple border stone, not exactly st-rock2 ...; only link level 92)
    "st_darkgray",              // PerOxyd stone 0x8e
    UNUSED,                     // PerOxyd stone 0x8f
    "st_concrete",              // PerOxyd stone 0x90
    "st_rotator_ccw_movable",   // PerOxyd stone 0x91
    "st_rotator_cw_movable",    // PerOxyd stone 0x92
    "st_swap",                  // PerOxyd stone 0x93
    "st-spitter",               // PerOxyd stone 0x94
    0,                          // PerOxyd stone 0x95 (dynamite holder, will implememnt it)(levels: 41 184 200)
    "st_rubberband",            // PerOxyd stone 0x96
    "st_scissors",              // PerOxyd stone 0x97
    "st_portal_horse",          // PerOxyd stone 0x98
    "st_blocker",               // PerOxyd stone 0x99
    "st-grate1",                // PerOxyd stone 0x9a
    "st_metal_hollow",          // PerOxyd stone 0x9b
    "st_brownpyramide",         // PerOxyd stone 0x9c
    "st_quake",                 // PerOxyd stone 0x9d
    "st_turnstile_red",         // PerOxyd stone 0x9e
    "st_turnstilearm_n",        // PerOxyd stone 0x9f
    "st_turnstilearm_s",        // PerOxyd stone 0xa0
    "st_turnstilearm_w",        // PerOxyd stone 0xa1
    "st_turnstilearm_e",        // PerOxyd stone 0xa2
    UNUSED,                     // PerOxyd stone 0xa3
    "st_laserflop_on",        // PerOxyd stone 0xa4 laserswitch on
    UNUSED,                     // PerOxyd stone 0xa5
    UNUSED,                     // PerOxyd stone 0xa6
    UNUSED,                     // PerOxyd stone 0xa7
    UNUSED,                     // PerOxyd stone 0xa8
    UNUSED,                     // PerOxyd stone 0xa9
    UNUSED,                     // PerOxyd stone 0xaa
    UNUSED,                     // PerOxyd stone 0xab
    UNUSED,                     // PerOxyd stone 0xac
    UNUSED,                     // PerOxyd stone 0xad
    UNUSED,                     // PerOxyd stone 0xae
    UNUSED,                     // PerOxyd stone 0xaf
    UNUSED,                     // PerOxyd stone 0xb0
    UNUSED,                     // PerOxyd stone 0xb1
    UNUSED,                     // PerOxyd stone 0xb2
    UNUSED,                     // PerOxyd stone 0xb3
    UNUSED,                     // PerOxyd stone 0xb4
    UNUSED,                     // PerOxyd stone 0xb5
    UNUSED,                     // PerOxyd stone 0xb6
    "st-easymode",              // PerOxyd stone 0xb7
    "st-peroxyd-0xb8",          // PerOxyd stone 0xb8
    "st-peroxyd-0xb9",          // PerOxyd stone 0xb9
    // codes >= 0xba are unused
};


const char *oxyd::peroxyd_item_map[256] = {
    IT_INVALID,                   // 0x00
    "it_extralife",               // 0x01
    IT_EXTERNAL,                  // 0x02 document 1
    IT_EXTERNAL,                  // 0x03 document 2
    "it_hammer",                  // 0x04
    "it_coffee",                  // 0x05
    "it_cherry",                  // 0x06
    "it_umbrella",                // 0x07
    "it_glasses",                 // 0x08
    "it_glasses_broken",          // 0x09
    "it-dynamite",                // 0x0a
    "it-blackbomb",               // 0x0b
    "it-whitebomb",               // 0x0c
    "it_crack_i",                 // 0x0d
    "it_crack_s",                 // 0x0e
    "it_crack_m",                 // 0x0f
    "it_crack_l",                 // 0x10
    "it_coin_s",                  // 0x11
    "it_coin_m",                  // 0x12
    "it_coin_l",                  // 0x13
    "it_key_a",                   // 0x14
    "it_key_b",                   // 0x15
    "it_key_c",                   // 0x16
    "it_floppy",                  // 0x17
    "it_flag_white",              // 0x18
    "it_flag_black",              // 0x19
    "it_ring",                    // 0x1a
    "it_pipe_nw",                 // 0x1b
    "it_pipe_sw",                 // 0x1c
    "it_pipe_ne",                 // 0x1d
    "it_pipe_es",                 // 0x1e
    "it_pipe_ns",                  // 0x1f
    "it_pipe_ew",                  // 0x20
    "it_spade",                   // 0x21
    "it_surprise",                // 0x22
    "it_pin",                     // 0x23
    "it_seed",                    // 0x24
    "it_spring_board",            // 0x25
    "it_spring_keep",             // 0x26
    "it_bag",                     // 0x27
    "it_magnet_off",              // 0x28
    "it_sensor_filter0",          // 0x29
    "it_sensor_filter1",          // 0x2a
    "it_shogun_s",                // 0x2b
    "it_shogun_l",                // 0x2c
    "it_vortex_open",             // 0x2d
    "it_vortex_closed",           // 0x2e
    "it_wormhole_on",             // 0x2f XXX
    "it_meditation_hill",         // 0x30
    "it_meditation_bump",         // 0x31
    "it_meditation_hollow",       // 0x32
    "it_meditation_dent",         // 0x33
    "it_strip_ns",                // 0x34
    "it_strip_ew",                // 0x35
    "it_springboard",             // 0x36
    IT_MISSING,                   // 0x37
    "it-bridge-oxyd",             // 0x38
    UNUSED,                  // 0x39
    UNUSED,                  // 0x3a
    UNUSED,                  // 0x3b
    "it_cross",                   // 0x3c
    "it_spoon",                   // 0x3d
    IT_MISSING,                   // 0x3e rubber band
    "it_sensor_exit",             // 0x3f
    "it_trigger",                 // 0x40
    "it_brush",                   // 0x41
    "it_banana",                  // 0x42
    "it_pencil",                  // 0x43
    "it_brake",                   // 0x44
    "it_squashed",                // 0x45
    "it_blocker",                 // 0x46
    "it_magicwand",               // 0x47
    "it_wrench",                  // 0x48
    UNUSED,                  // 0x49
    "it-odometer",                // 0x4a
    "it_puller_n",                // 0x4b
    "it_puller_s",                // 0x4c
    "it_puller_w",                // 0x4d
    "it_puller_e",                // 0x4e
    UNUSED,                       // 0x4f
    UNUSED,                       // 0x50
    IT_MISSING,                   // 0x51 puller left, active
    UNUSED,                       // 0x52
    IT_MISSING,                   // 0x53 oxyd on floor (?"
    UNUSED,                       // 0x54
    UNUSED,                       // 0x55
    UNUSED,                       // 0x56
    UNUSED,                       // 0x57
    IT_MISSING,                   // 0x58 oxyd on floor
    IT_MISSING,                   // 0x59 oxyd on floor
    UNUSED,                  // 0x5a
    UNUSED,                  // 0x5b
    UNUSED,                  // 0x5c
    UNUSED,                  // 0x5d
    UNUSED,                  // 0x5e
    UNUSED,                  // 0x5f
    UNUSED,                  // 0x60
    UNUSED,                  // 0x61
    UNUSED,                  // 0x62
    UNUSED,                  // 0x63
    UNUSED,                  // 0x64
    UNUSED,                  // 0x65
    UNUSED,                  // 0x66
    UNUSED,                  // 0x67
    UNUSED,                  // 0x68
    UNUSED,                  // 0x69
    "it-blackbomb-burning",       // 0x6a
    UNUSED,                  // 0x6b
    UNUSED,                  // 0x6c
    UNUSED,                  // 0x6d
    UNUSED,                  // 0x6e
    UNUSED,                  // 0x6f
    UNUSED,                  // 0x70
    UNUSED,                  // 0x71
    UNUSED,                  // 0x72
    UNUSED,                  // 0x73
    UNUSED,                  // 0x74
    UNUSED,                  // 0x75
    "it-easykillstone",           // 0x76
    "it-easykeepstone",           // 0x77
    "it-2pkillstone",             // 0x78
    "it-1pkillstone",             // 0x79
    // codes >= 0x7a are unused
};

