------------------------------------------------------------------------
-- Copyright (C) 2002,2003,2004,2005 Daniel Heck
-- Copyright (C) 2007,2008 Ronald Lamprecht
--
-- This program is free software; you can redistribute it and/or
-- modify it under the terms of the GNU General Public License
-- as published by the Free Software Foundation; either version 2
-- of the License, or (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License along
-- with this program; if not, write to the Free Software Foundation, Inc.,
-- 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
--
------------------------------------------------------------------------

-----------------------------------------------
-- Use Enigma internal random implementation --
-----------------------------------------------

if type(en) == "table" then
    math.random = en.random
else
    math.random = enigma.random
end
random = math.random
-- randseed is issued by Enigma application and must not be disturbed
math.randomseed = function () end

-------------------------------
-- Compatibility for old API --
-------------------------------

enigma._MakeObject = enigma.MakeObject
enigma._GetKind = enigma.GetKind
enigma._SetAttrib = enigma.SetAttrib
enigma._GetAttrib = enigma.GetAttrib
enigma._GetObjectTemplate = enigma.GetObjectTemplate

RenamingObjectsOld2New = {
}

RenamingObjectsNew2Old = {
    ac_bug = "ac-bug",
    ac_horse = "ac-horse",
    ac_killer = "ac-killerball",
    ac_marble_black = "ac-blackball",
    ac_marble_white = "ac-whiteball",
    ac_pearl_white = "ac-whiteball-small",
    ac_rotor = "ac-rotor",
    ac_top = "ac-top",
    fl_abyss = "fl-abyss",
    fl_adhesionless = "fl-nomouse",
    fl_aquamarine = "fl-bluegreenx",
    fl_bast = "fl-floor_001",
    fl_bluegray = "fl-bluegray",
    fl_bluegreen = "fl-bluegreen",
    fl_blueslab = "fl-rough-blue",
    fl_brick = "fl-brick",
    fl_bridge = "fl-bridge",
    fl_bright = "fl-white",
    fl_concrete = "fl-concrete",
    fl_dark = "fl-black",
    fl_darkgray = "fl-darkgray",
    fl_dunes = "fl-dunes",
    fl_fake_abyss = "fl-abyss_fake",
    fl_fake_trigger = "fl-trigger",
    fl_gravel = "fl-gravel",
    fl_gray = "fl-gray",
    fl_hay = "fl-hay",
    fl_himalaya = "fl-himalaya",
    fl_ice = "fl-ice",
    fl_inverse_white = "fl-inverse",
    fl_inverse_gray = "fl-inverse2",
    fl_ivory = "fl-light",
    fl_lawn_a = "fl-leaves",
    fl_lawn_b = "fl-leavesb",
    fl_lawn_c1 = "fl-leavesc1",
    fl_lawn_c2 = "fl-leavesc2",
    fl_lawn_c3 = "fl-leavesc3",
    fl_lawn_c4 = "fl-leavesc4",
    fl_lawn_d1 = "fl-leavesd1",
    fl_lawn_d2 = "fl-leavesd2",
    fl_lawn_d3 = "fl-leavesd3",
    fl_lawn_d4 = "fl-leavesd4",
    fl_lawn_e1 = "fl-leavese1",
    fl_lawn_e2 = "fl-leavese2",
    fl_lawn_e3 = "fl-leavese3",
    fl_lawn_e4 = "fl-leavese4",
    fl_marble = "fl-marble",
    fl_metal = "fl-metal",
    fl_metal_1 = "fl-metal1",
    fl_metal_2 = "fl-metal2",
    fl_metal_3 = "fl-metal3",
    fl_metal_4 = "fl-metal4",
    fl_metal_5 = "fl-metal5",
    fl_metal_6 = "fl-metal6",
    fl_metal_7 = "fl-normal_x",
    fl_metal_7n = "fl-normal",
    fl_mortar = "fl-mortar",
    fl_pinkbumps = "fl-bumps",
    fl_plank = "fl-plank",
    fl_platinum = "fl-lightgray",
    fl_red = "fl-red",
    fl_redslab = "fl-rough-red",
    fl_rock = "fl-rock",
    fl_rough = "fl-rough",
    fl_sahara = "fl-sahara",
    fl_sand = "fl-sand",
    fl_samba = "fl-samba",
    fl_samba_h = "fl-samba1",
    fl_samba_v = "fl-samba2",
    fl_slope_ps = "fl-gradient1",
    fl_slope_pn = "fl-gradient2",
    fl_slope_pe = "fl-gradient3",
    fl_slope_pw = "fl-gradient4",
    fl_slope_ose = "fl-gradient5",
    fl_slope_osw = "fl-gradient6",
    fl_slope_one = "fl-gradient7",
    fl_slope_onw = "fl-gradient8",
    fl_slope_ise = "fl-gradient9",
    fl_slope_ine = "fl-gradient10",
    fl_slope_isw = "fl-gradient11",
    fl_slope_inw = "fl-gradient12",
    fl_slope_ts = "fl-gradient14",
    fl_slope_tn = "fl-gradient13",
    fl_slope_te = "fl-gradient16",
    fl_slope_tw = "fl-gradient15",
    fl_space = "fl-space",
    fl_stone = "fl-stone",
    fl_swamp = "fl-swamp",
    fl_thief = "fl-thief",
    fl_tigris = "fl-tigris",
    fl_water = "fl-water",
    fl_white = "fl-springboard",
    fl_wood = "fl-wood",
    fl_wood_h = "fl-wood1",
    fl_wood_v = "fl-wood2",
    fl_wood_framed = "fl-stwood",
    fl_wood_framed_h = "fl-stwood1",
    fl_wood_framed_v = "fl-stwood2",
    fl_woven = "fl-woven",
    fl_yinyang_yin_invisible = "fl-acblack",
    fl_yinyang_yang_invisible = "fl-acwhite",    
    it_banana = "it-banana",
    it_bag = "it-bag",
    it_blocker = "it-blocker",
    it_blocker_new = "it-blocker-new",
    it_bomb_black = "it-blackbomb",
    it_bomb_white = "it-whitebomb",
    it_bottle_idle = "it-booze",
    it_bottle_broken = "it-booze-broken",
    it_brake = "it-brake",
    it_brush = "it-brush",
    it_burnable_invisible = "it-burnable",
    it_burnable_oil = "it-burnable_oil",
    it_burnable_ignited = "it-burnable_ignited",
    it_burnable_burning = "it-burnable_burning",
    it_burnable_ash = "it-burnable_ash",
    it_burnable_fireproof = "it-burnable_fireproof",
    it_cherry = "it-cherry",
    it_coffee = "it-coffee",
    it_coin_s = "it-coin1",
    it_coin_m = "it-coin2",
    it_coin_l = "it-coin4",
    it_crack_i = "it-crack0",
    it_crack_s = "it-crack1",
    it_crack_m = "it-crack2",
    it_crack_l = "it-crack3",
    it_cross = "it-cross",
    it_death = "it-death",
    it_document = "it-document",
    it_drop = "it-drop",
    it_dynamite = "it-dynamite",
    it_explosion_nil = "it-explosion1",
    it_explosion_hollow = "it-explosion2",
    it_explosion_debris = "it-explosion3",    
    it_extinguisher_empty = "it-extinguisher_empty",
    it_extinguisher_medium = "it-extinguisher_medium",
    it_extinguisher_full = "it-extinguisher",
    it_extralife = "it-extralife",
    it_flag_black = "it-flagblack",
    it_flag_white = "it-flagwhite",
    it_floppy = "it-floppy",
    it_glasses = "it-glasses",
    it_glasses_broken = "it-glasses-broken",
    it_hammer = "it-hammer",
    it_landmine = "it-landmine",
    it_magicwand = "it-magicwand",
    it_magnet = "it-magnet",
    it_magnet_on = "it-magnet-on",
    it_magnet_off = "it-magnet-off",
    it_meditation_hollow = "it-hollow",
    it_meditation_dent = "it-tinyhollow",
    it_meditation_bump = "it-tinyhill",
    it_meditation_hill = "it-hill",
    it_odometer = "it-odometer",
    it_pencil = "it-pencil",
    it_pin = "it-pin",
    it_pipe_w = "it-pipe-w",
    it_pipe_s = "it-pipe-s",
    it_pipe_sw = "it-pipe-sw",
    it_pipe_e = "it-pipe-e",
    it_pipe_ew = "it-pipe-h",
    it_pipe_es = "it-pipe-es",
    it_pipe_n = "it-pipe-n",
    it_pipe_nw = "it-pipe-wn",
    it_pipe_ns = "it-pipe-v",
    it_pipe_ne = "it-pipe-ne",
    it_puller_w = "it-puller-w",
    it_puller_s = "it-puller-s",
    it_puller_e = "it-puller-e",
    it_puller_n = "it-puller-n",
    it_ring = "it-ring",
    it_rubberband = "it-rubberband",
    it_seed = "it-seed",
    it_seed_greenbrown = "it-seed_nowood",
    it_seed_volcano = "it-seed_volcano",
    it_spade = "it-spade",
    it_spoon = "it-spoon",
    it_spring_keep = "it-spring1",
    it_spring_drop = "it-spring2",
    it_springboard = "it-springboard",
    it_squashed = "it-squashed",
    it_shogun_s = "it-shogun-s",
    it_shogun_m = "it-shogun-m",
    it_shogun_l = "it-shogun-l",
    it_strip_ew = "it-hstrip",
    it_strip_ns = "it-vstrip",
    it_surprise = "it-surprise",
    it_sword = "it-sword",
    it_trap = "it-abyss",
    it_trigger = "it-trigger",
    it_umbrella = "it-umbrella",
    it_vortex = "it-vortex-open",
    it_vortex_open = "it-vortex-open",
    it_vortex_closed = "it-vortex-closed",
    it_weight = "it-weight",
    it_wormhole_on = "it-wormhole",
    it_wormhole_off = "it-wormhole-off",
    it_wrench = "it-wrench",
    it_yinyang = "it-yinyang",
    st_actorimpulse = "st-actorimpulse",
    st_actorimpulse_invisible = "st-actorimpulse_invisible",
    st_ancient = "st-rock9",
    st_beads = "st-beads",
    st_blocker = "st-blocker",
    st_blocker_new = "st-blocker-growing",
    st_bluegray = "st-bluegray",
    st_bluegray_hollow = "st-bluegray_hole",
    st_bluesand = "st-blue-sand",
    st_bluesand_w = "st-bigbluesand-w",
    st_bluesand_s = "st-bigbluesand-s",
    st_bluesand_sw = "st-bigbluesand-sw",
    st_bluesand_e = "st-bigbluesand-e",
    st_bluesand_ew = "st-bigbluesand-ew",
    st_bluesand_es = "st-bigbluesand-es",
    st_bluesand_esw = "st-bigbluesand-esw",
    st_bluesand_n = "st-bigbluesand-n",
    st_bluesand_nw = "st-bigbluesand-nw",
    st_bluesand_ns = "st-bigbluesand-ns",
    st_bluesand_nsw = "st-bigbluesand-nsw",
    st_bluesand_ne = "st-bigbluesand-ne",
    st_bluesand_new = "st-bigbluesand-new",
    st_bluesand_nes = "st-bigbluesand-nes",
    st_bluesand_nesw = "st-bigbluesand-nesw",
    st_blur_cross = "st-yinyang1",
    st_blur_magic = "st-yinyang3",
    st_blur_straight = "st-yinyang2",
    st_boulder = "st-bolder",
    st_boulder_n = "st-bolder-n",
    st_boulder_e = "st-bolder-e",
    st_boulder_s = "st-bolder-s",
    st_boulder_w = "st-bolder-w",
    st_box_hay = "st-flhay",
    st_box_rock = "st-flrock",
    st_box_wood = "st-wood",
    st_box_wood_growing = "st-wood-growing",
    st_box_wood_h = "st-wood1",
    st_box_wood_v = "st-wood2",
    st_brake = "st-brake",
    st_break_black = "st-break_acblack",
    st_break_boulder = "st-break_bolder",
    st_break_bug = "st-bug",
    st_break_laser = "st-laserbreak",
    st_break_oxydc = "st-stone_break",
    st_break_plain = "st-break_gray",
    st_break_white = "st-break_acwhite",
    st_brick = "st-brick",
    st_brick_w = "st-bigbrick-w",
    st_brick_s = "st-bigbrick-s",
    st_brick_sw = "st-bigbrick-sw",
    st_brick_e = "st-bigbrick-e",
    st_brick_ew = "st-bigbrick-ew",
    st_brick_es = "st-bigbrick-es",
    st_brick_esw = "st-bigbrick-esw",
    st_brick_n = "st-bigbrick-n",
    st_brick_nw = "st-bigbrick-nw",
    st_brick_ns = "st-bigbrick-ns",
    st_brick_nsw = "st-bigbrick-nsw",
    st_brick_ne = "st-bigbrick-ne",
    st_brick_new = "st-bigbrick-new",
    st_brick_nes = "st-bigbrick-nes",
    st_brick_nesw = "st-bigbrick-nesw",
    st_brownie = "st-brownie",
    st_brownmarble = "st-rock10",
    st_brownpyramid = "st-stone1",
    st_camouflage = "st-camouflage",
    st_camouflage_hollow = "st-camouflage_hole",
    st_camouflage_movable = "st-camouflage_move",
    st_chameleon = "st-chameleon",
    st_charge_minus = "st-chargeminus",
    st_charge_plus = "st-chargeplus",
    st_charge_zero = "st-chargezero",
    st_chess = "st-chess",
    st_chess_black = "st-chess_black",
    st_chess_white = "st-chess_white",
    st_coinslot = "st-coinslot",
    st_concrete = "st-rock8",
    st_dongle = "st-magic",
    st_darkglass = "st-glass2",
    st_darkglass_hollow = "st-glass2_hole",
    st_darkglass_movable = "st-glass2_move",
    st_darkgray = "st-rock2",
    st_darkgray_hollow = "st-rock2_hole",
    st_death = "st-death",
    st_death_invisible = "st-death_invisible",
    st_disco_light = "st-disco-light",
    st_disco_medium = "st-disco-medium",
    st_disco_dark = "st-disco-dark",
    st_dispenser_bombblack = "st-bombs",
    st_door_a = "st-door_a",
    st_door_b = "st-door_b",
    st_door_c = "st-door_c",
    st_fake_quake = "st-fakeoxyd",
    st_fake_oxyda = "st-likeoxyda",
    st_fake_oxydb = "st-likeoxydb",
    st_fake_oxydc = "st-likeoxydc",
    st_fake_oxydd = "st-likeoxydd",
    st_flash = "st-flash",
    st_flat = "st-plain",
    st_flat_breakable = "st-plain_break",
    st_flat_hollow = "st-plain_hole",
    st_flat_movable = "st-plain_move",
    st_flat_cracked = "st-plain_cracked",
    st_flat_burnable = "st-firebreak",
    st_flat_moveburnable = "st-firebreak_move",
    st_floppy = "st-floppy",
    st_fourswitch = "st-fourswitch",
    st_ghost_break = "st-break_invisible",
    st_ghost_greenbrown = "st-invisible_magic",
    st_ghost_purplemarble = "st-stonebrush",
    st_granite = "st-rock1",
    st_granite_hollow = "st-rock1_hole",
    st_granite_movable = "st-rock1_move",
    st_grate_cross = "st-grate1",
    st_grate_framed = "st-grate2",
    st_greenbrown = "st-greenbrown",
    st_greenbrown_hollow = "st-greenbrown_hole",
    st_greenbrown_movable = "st-greenbrown_move",
    st_greengray = "st-rock7",
    st_invisible = "st-invisible",
    st_invisible_hollow = "st-invisible_hole",
    st_invisible_movable = "st-invisible_move",
    st_jamb_black = "st-blackballs",
    st_jamb_white = "st-whiteballs",
    st_knight = "st-knight",
    st_laser_w = "st-laser-w",
    st_laser_s = "st-laser-s",
    st_laser_e = "st-laser-e",
    st_laser_n = "st-laser-n",
    st_laserswitch = "st-laserswitch",
    st_laserflop = "st-lasertimeswitch",
    st_lightglass = "st-glass1",
    st_lightglass_hollow = "st-glass1_hole",
    st_lightglass_movable = "st-glass1_move",
    st_lightpassenger = "st-lightpassenger",
    st_lightpassenger_off = "st-lightpassenger_off",
    st_magic_brick = "st-brick_magic",
    st_magic_oxyda = "st-fakeoxyda",
    st_magic_oxydc = "st-coffee",
    st_mail_w = "st-mail-w",
    st_mail_s = "st-mail-s",
    st_mail_e = "st-mail-e",
    st_mail_n = "st-mail-n",
    st_metal = "st-metal",
    st_metal_hollow = "st-metal_hole",

    st_mirror_slab_n = "st-mirror-p|",
    st_mirror_slab_e = "st-mirror-p/",
    st_mirror_slab_s = "st-mirror-p-",
    st_mirror_slab_w = "st-mirror-p\\",
    st_mirror_slab_nt = "st-mirror-p|t",
    st_mirror_slab_et = "st-mirror-p/t",
    st_mirror_slab_st = "st-mirror-p-t",
    st_mirror_slab_wt = "st-mirror-p\\t",
    st_mirror_slab_nm = "st-mirror-p|m",
    st_mirror_slab_em = "st-mirror-p/m",
    st_mirror_slab_sm = "st-mirror-p-m",
    st_mirror_slab_wm = "st-mirror-p\\m",
    st_mirror_slab_ntm = "st-mirror-p|tm",
    st_mirror_slab_etm = "st-mirror-p/tm",
    st_mirror_slab_stm = "st-mirror-p-tm",
    st_mirror_slab_wtm = "st-mirror-p\\tm",
    st_mirror_triangle_n = "st-mirror-3^",
    st_mirror_triangle_e = "st-mirror-3>",
    st_mirror_triangle_s = "st-mirror-3v",
    st_mirror_triangle_w = "st-mirror-3<",
    st_mirror_triangle_nt = "st-mirror-3^t",
    st_mirror_triangle_et = "st-mirror-3>t",
    st_mirror_triangle_st = "st-mirror-3vt",
    st_mirror_triangle_wt = "st-mirror-3<t",
    st_mirror_triangle_nm = "st-mirror-3^m",
    st_mirror_triangle_em = "st-mirror-3>m",
    st_mirror_triangle_sm = "st-mirror-3vm",
    st_mirror_triangle_wm = "st-mirror-3<m",
    st_mirror_triangle_ntm = "st-mirror-3^tm",
    st_mirror_triangle_etm = "st-mirror-3>tm",
    st_mirror_triangle_stm = "st-mirror-3vtm",
    st_mirror_triangle_wtm = "st-mirror-3<tm",

    st_monoflop = "st-timeswitch",
    st_oxyd = "st-oxyd",
    st_oneway = "st-oneway",
    st_oneway_n = "st-oneway-n",
    st_oneway_e = "st-oneway-e",
    st_oneway_s = "st-oneway-s",
    st_oneway_w = "st-oneway-w",
    st_oneway_black = "st-oneway_black",
    st_oneway_black_n = "st-oneway_black-n",
    st_oneway_black_e = "st-oneway_black-e",
    st_oneway_black_s = "st-oneway_black-s",
    st_oneway_black_w = "st-oneway_black-w",
    st_oneway_white = "st-oneway_white",
    st_oneway_white_n = "st-oneway_white-n",
    st_oneway_white_e = "st-oneway_white-e",
    st_oneway_white_s = "st-oneway_white-s",
    st_oneway_white_w = "st-oneway_white-w",
    st_panel = "st-wood_001",
    st_polarswitch = "st-polarswitch",
    st_passage_black_square = "st-black1",
    st_passage_black_slash = "st-black2",
    st_passage_black_cross = "st-black3",
    st_passage_black_frame = "st-black4",
    st_passage_white_square = "st-white1",
    st_passage_white_slash = "st-white2",
    st_passage_white_cross = "st-white3",
    st_passage_white_frame = "st-white4",
    st_pinkbumps = "st-bumps",
    st_plaster = "st-rock3",
    st_plaster_hollow = "st-rock3_hole",
    st_plaster_movable = "st-rock3_move",
    st_plaster_breakable = "st-rock3_break",
    st_plaster_movebreakable = "st-rock3_movebreak",
    st_plop = "st-block",
    st_portal_horse = "st-grate3",
    st_pull = "st-pull",
    st_purplegray = "st-rock6",
    st_purplemarble = "st-rock4",
    
    st_puzzle = "st-puzzle",
    st_puzzle_blue_w = "st-puzzle-w",
    st_puzzle_blue_s = "st-puzzle-s",
    st_puzzle_blue_sw = "st-puzzle-sw",
    st_puzzle_blue_e = "st-puzzle-e",
    st_puzzle_blue_ew = "st-puzzle-ew",
    st_puzzle_blue_es = "st-puzzle-es",
    st_puzzle_blue_esw = "st-puzzle-esw",
    st_puzzle_blue_n = "st-puzzle-n",
    st_puzzle_blue_nw = "st-puzzle-nw",
    st_puzzle_blue_ns = "st-puzzle-ns",
    st_puzzle_blue_nsw = "st-puzzle-nsw",
    st_puzzle_blue_ne = "st-puzzle-ne",
    st_puzzle_blue_new = "st-puzzle-new",
    st_puzzle_blue_nes = "st-puzzle-nes",
    st_puzzle_blue_nesw = "st-puzzle-nesw",
    st_puzzle_blue_nesw_hollow = "st-puzzle-hollow",
    st_puzzle_yellow_w = "st-puzzle2-w",
    st_puzzle_yellow_s = "st-puzzle2-s",
    st_puzzle_yellow_sw = "st-puzzle2-sw",
    st_puzzle_yellow_e = "st-puzzle2-e",
    st_puzzle_yellow_ew = "st-puzzle2-ew",
    st_puzzle_yellow_es = "st-puzzle2-es",
    st_puzzle_yellow_esw = "st-puzzle2-esw",
    st_puzzle_yellow_n = "st-puzzle2-n",
    st_puzzle_yellow_nw = "st-puzzle2-nw",
    st_puzzle_yellow_ns = "st-puzzle2-ns",
    st_puzzle_yellow_nsw = "st-puzzle2-nsw",
    st_puzzle_yellow_ne = "st-puzzle2-ne",
    st_puzzle_yellow_new = "st-puzzle2-new",
    st_puzzle_yellow_nes = "st-puzzle2-nes",
    st_puzzle_yellow_nesw = "st-puzzle2-nesw",
    st_puzzle_yellow_nesw_hollow = "st-puzzle2-hollow",
    st_quake = "st-fart",
    st_rawglass = "st-glass",
    st_rawglass_movable = "st-glass_move",
    st_redbrown = "st-marble",
    st_redbrown_hollow = "st-marble_hole",
    st_redbrown_movable = "st-marble_move",
    st_redfiber = "st-glass3",
    st_redmarble = "st-stone2",
    st_redrock = "st-redrock",
    st_rotator_cw = "st-rotator-right",
    st_rotator_ccw = "st-rotator-left",
    st_rotator_cw_movable = "st-rotator_move-right",
    st_rotator_ccw_movable = "st-rotator_move-left",
    st_rubberband = "st-rubberband",
    st_scissors = "st-scissors",
    st_shogun = "st-shogun",
    st_shogun_s = "st-shogun-s",
    st_shogun_m = "st-shogun-m",
    st_shogun_sm = "st-shogun-sm",
    st_shogun_l = "st-shogun-l",
    st_shogun_sl = "st-shogun-sl",
    st_shogun_ml = "st-shogun-ml",
    st_shogun_sml = "st-shogun-sml",
    st_spitter = "st-spitter",
    st_stoneimpulse = "st-stoneimpulse",
    st_stoneimpulse_hollow = "st-stoneimpulse-hollow",
    st_stoneimpulse_movable = "st-stoneimpulse_movable",
    st_surprise = "st-surprise",
    st_swap = "st-swap",
    st_switch = "st-switch",
    st_switch_black = "st-switch_black",
    st_switch_white = "st-switch_white",
    st_thief = "st-thief",
    st_tigris = "st-rock5",
    st_timer = "st-timer",
    st_turnstile_red = "st-turnstile",
    st_turnstile_green = "st-turnstile-green",
    st_turnstilearm_n = "st-turnstile-n",
    st_turnstilearm_e = "st-turnstile-e",
    st_turnstilearm_s = "st-turnstile-s",
    st_turnstilearm_w = "st-turnstile-w",
    st_volcano = "st-volcano",
    st_volcano_new = "st-volcano-growing",
    st_volcano_active = "st-volcano_active",
    st_volcano_idle = "st-volcano_inactive",
    st_window = "st-window",
    st_woven = "st-woven",
    st_yellow = "st-yellow"
}

for k,v in pairs(RenamingObjectsNew2Old) do
    RenamingObjectsOld2New[v] = k
end

function enigma.MakeObject(name)
    if name.sub(2,2) == "_" then
        error("Unknown object kind '"..name.."' in api 1.")
    end
    if name == "ac-whiteball-small" then
        local obj = enigma._MakeObject("ac_pearl_white")
        enigma._SetAttrib(obj, "owner", nil)
        return obj
    elseif name == "fl-gradient" then
        local obj = enigma._MakeObject("fl_slope_ps")
        return obj
    elseif name == "fl-bridge-open" then
        local obj = enigma._MakeObject("fl_bridge")
        return obj
    elseif name == "fl-bridge-closed" then
        local obj = enigma._MakeObject("fl_bridge")
        enigma._SetAttrib(obj, "state", 0)
        return obj
    elseif name == "fl-rough_medium" then
        local obj = enigma._MakeObject("fl_rough")
        enigma._SetAttrib(obj, "friction", 5.0)
        enigma._SetAttrib(obj, "adhesion", 1.2)
        return obj
    elseif name == "fl-rough_slow" then
        local obj = enigma._MakeObject("fl_rough")
        enigma._SetAttrib(obj, "friction", 7.0)
        enigma._SetAttrib(obj, "adhesion", 0.5)
        return obj
    elseif name == "st-key" then
        local obj = enigma._MakeObject("st_key")
        -- Old API keycode default was 0 for st-key, but 1 for it-key!
        enigma._SetAttrib(obj, "code", 0)
        return obj
    elseif name == "st-key_a" then
        local obj = enigma._MakeObject("st_key")
        enigma._SetAttrib(obj, "code", 1)
        return obj
    elseif name == "st-key_b" then
        local obj = enigma._MakeObject("st_key")
        enigma._SetAttrib(obj, "code", 2)
        return obj
    elseif name == "st-key_c" then
        local obj = enigma._MakeObject("st_key")
        enigma._SetAttrib(obj, "code", 3)
        return obj
    elseif name == "st-switch" then
        local obj = enigma._MakeObject("st_switch")
        enigma._SetAttrib(obj, "instant", true)
        return obj
    elseif name == "st-switch_black" then
        local obj = enigma._MakeObject("st_switch_black")
        enigma._SetAttrib(obj, "instant", true)
        return obj
    elseif name == "st-switch_white" then
        local obj = enigma._MakeObject("st_switch_white")
        enigma._SetAttrib(obj, "instant", true)
        return obj
    elseif name == "it-key_a" or  name == "it-key" then
        local obj = enigma._MakeObject("it_key")
        enigma._SetAttrib(obj, "code", 1)
        return obj
    elseif name == "it-key_b" then
        local obj = enigma._MakeObject("it_key")
        enigma._SetAttrib(obj, "code", 2)
        return obj
    elseif name == "it-key_c" then
        local obj = enigma._MakeObject("it_key")
        enigma._SetAttrib(obj, "code", 3)
        return obj
    elseif name == "it-sensor" then
        local obj = enigma._MakeObject("it_sensor")
        enigma._SetAttrib(obj, "invisible", true)
        return obj
    elseif name == "it-inversesensor" then
        local obj = enigma._MakeObject("it_sensor")
        enigma._SetAttrib(obj, "invisible", true)
        enigma._SetAttrib(obj, "inverse", true)
        return obj
    elseif name == "it-wormhole" then
        local obj = enigma._MakeObject("it_wormhole_on")
        enigma._SetAttrib(obj, "scissor", false)
        return obj
    elseif name == "it-wormhole-off" then
        local obj = enigma._MakeObject("it_wormhole_off")
        enigma._SetAttrib(obj, "scissor", false)
        return obj
    elseif name == "it-vortex-open" then
        local obj = enigma._MakeObject("it_vortex_open")
        enigma._SetAttrib(obj, "scissor", false)
        return obj
    elseif name == "it-vortex-closed" then
        local obj = enigma._MakeObject("it_vortex_closed")
        enigma._SetAttrib(obj, "scissor", false)
        enigma._SetAttrib(obj, "autoclose", true)
        return obj
    elseif name == "st-pmirror" then
        return enigma._MakeObject("st_mirror_slab_e")
    elseif name == "st-3mirror" then
        return enigma._MakeObject("st_mirror_triangle_s")
    elseif name == "st-scissors" then
        local obj = enigma._MakeObject("st_scissors")
        enigma._SetAttrib(obj, "inverse", true)
        return obj
    elseif name == "st-door-h" or name == "st-door" then
        local obj = enigma._MakeObject("st_door_d")
        enigma._SetAttrib(obj, "faces", "ns")
        return obj
    elseif name == "st-door-h-open" then
        local obj = enigma._MakeObject("st_door_d")
        enigma._SetAttrib(obj, "faces", "ns")
        enigma._SetAttrib(obj, "state", 1)
        return obj
    elseif name == "st-door-v" then
        local obj = enigma._MakeObject("st_door_d")
        enigma._SetAttrib(obj, "faces", "ew")
        return obj
    elseif name == "st-door-v-open" then
        local obj = enigma._MakeObject("st_door_d")
        enigma._SetAttrib(obj, "faces", "ew")
        enigma._SetAttrib(obj, "state", 1)
        return obj
    elseif name == "st-likeoxyda-open" then
        local obj = enigma._MakeObject("st_fake_oxyda")
        enigma._SetAttrib(obj, "state", 1)
        return obj
    elseif name == "st-likeoxydb-open" then
        local obj = enigma._MakeObject("st_fake_oxydb")
        enigma._SetAttrib(obj, "state", 1)
        return obj
    elseif name == "st-likeoxydc-open" then
        local obj = enigma._MakeObject("st_fake_oxydc")
        enigma._SetAttrib(obj, "state", 1)
        return obj
    elseif name == "st-likeoxydd-open" then
        local obj = enigma._MakeObject("st_fake_oxydd")
        enigma._SetAttrib(obj, "state", 1)
        return obj
    elseif name == "st-spitter" then
        local obj = enigma._MakeObject("st_spitter")
        enigma._SetAttrib(obj, "secure", true)
        return obj
    end
    newname = RenamingObjectsOld2New[name]

    if name == "st-laser" then
        newname = "st_laser_e"
    end

    if newname ~= nil then
        return enigma._MakeObject(newname)
    else
        error("Unknown object kind '"..name.."' in api 1.")
    end
end

function enigma.GetKind(obj)
    local _newname = enigma.GetClass(obj)
    local _oldname = RenamingObjectsNew2Old[_newname]
    
    if _oldname == nil then
        _oldname = RenamingObjectsNew2Old[enigma._GetKind(obj)]
    end

    if _newname == "st_key" then
        local code = enigma._GetAttrib(obj, "code")
        if code == 0 then
            return "st-key"
        elseif code == 1 then
            return "st-key_a"
        elseif code == 2 then
            return "st-key_b"
        elseif code == 3 then
            return "st-key_c"
        else -- arbitrary keycodes
            return "st-key"
        end
    end
    if _newname == "it_key" then
        local code = enigma._GetAttrib(obj, "code")
        if code == 1 then
            return "it-key_a"
        elseif code == 2 then
            return "it-key_b"
        elseif code == 3 then
            return "it-key_c"
        else -- arbitrary keycodes
            return "it-key_a"
        end
    end
    if _newname == "st_door" then
        local flavor = enigma._GetAttrib(obj, "flavor")
        if flavor == "d" then
            return "st-door"
        else
            return "st-door_" .. flavor
        end
    end
    if _newname == "st_puzzle" then
        return "st-puzzle"
    end
    if _newname == "it_sensor" then
        local code = enigma._GetAttrib(obj, "inverse")
        if code == false then
            return "it-sensor"
        else
            return "it-inversesensor"
        end
    end
    if string.sub(_newname, 1, 8) == "st_laser" then
        return "st-laser"
    end
    if _newname == "st_mirror" then
        return "st-mirror"
    end
    if _newname == "fl_slope" then
        return "fl-gradient"
    end
    if _newname == "st_fake_oxydc" then
        local state = enigma._GetAttrib(obj, "state")
        if state == 0 then
            return "st-likeoxydc"
        else
            return "st-likeoxydc-open"
        end
    end
    if _oldname ~= nil then
        return _oldname
    else
        return _newname
    end
end

function enigma.SetAttrib(obj, key, val)
     local _val = val
     local _key = key
     local _obj_name = enigma.GetKind(obj)

     if _obj_name == "st-oxyd" then
         if key == "color" then
	     _val = 0 + val   -- convert to int
             _key = "oxydcolor"
	 end
     end
     if key == "connections" then
         if  val == 2  then _val = "w"
         elseif  val == 3  then _val = "s"
         elseif  val == 4  then _val = "sw"
         elseif  val == 5  then _val = "e"
         elseif  val == 6  then _val = "ew"
         elseif  val == 7  then _val = "es"
         elseif  val == 8  then _val = "esw"
         elseif  val == 9  then _val = "n"
         elseif  val == 10 then _val = "nw"
         elseif  val == 11 then _val = "ns"
         elseif  val == 12 then _val = "nsw"
         elseif  val == 13 then _val = "ne"
         elseif  val == 14 then _val = "new"
         elseif  val == 15 then _val = "nes"
         elseif  val == 16 then _val = "nesw"
         elseif  val == 1 and _obj_name ~= "st-puzzle" then _val = ""
         elseif  val == 1 and _obj_name == "st-puzzle" then
             _key = "hollow"
             _val = true
             enigma._SetAttrib(obj, "connections", "nesw")
         end
     end
     if key == "oxyd" then
        _key = "color"
        if val == 0 then _val = 2 else _val = 3 end
     end
     if key == "keycode" then
         _key = "code"
     end
     if key == "delay" then
         _key = "interval"
     end
     if key == "dir" or key == "direction" then
         _key = "orientation"
     end
     if key == "on" then
         _key = "state"
     end
     if key == "value" then
         _key = "coin_value"
     end
     if key == "load" then
         _key = "state"
     end
     if key == "force" then
         _key = "strength"
     end
     if key == "friction_factor" then
         _key = "friction"
     end
     if key == "gradient_factor" then
         _key = "gradient"
     end
     if key == "minlength"  then
         _key = "threshold"
     end
     if key == "target" and (_obj_name == "it-rubberband") then
         _key = "anchor2"
     end
     if key == "targetx" then
         local d = enigma._GetAttrib(obj, "destination")
         if (en.usertype(d) == "position") then
             _val = po(0 + val, d.y)
         else
             _val = po(0 + val, 0)
         end
         _key = "destination"
     end
     if key == "targety" then
         local d = enigma._GetAttrib(obj, "destination")
         if (en.usertype(d) == "position") then
             _val = po(d.x, 0 + val)
         else
             _val = po(0, 0 + val)
         end
         _key = "destination"
     end
     if key == "target1" or key == "target2" or key == "target3" or key == "target4" then
        local i = 0 + string.sub(key, -1)  -- the target number
        local d = enigma._GetAttrib(obj, "destination")
        if (d == nil) or (type(d) ~= "table") then
            d = {}
        end

        local c = {}

        for str in string.gmatch(val, "%d+") do
            table.insert(c, 0 + str)
        end
        d[i] = enigma.GetFloor(c[1], c[2]) -- requires floor to be set first (invalid for Nat method - level Bodyguard)
        if d[i] == nil then
             error("Actor Horse without floor error")
        end
        for j = 1, i do
           if d[j] == nil then
              d[j] = d[i]
           end
        end
        _key = "destination"
        _val = d
     end
     if key == "invisible" then
         if val == 1 then _val = true else _val = false end
     end
     if key == "loop" then
         if val == 1 then _val = true else _val = false end
     end
     if key == "scissor" then
         if val == 1 then _val = true else _val = false end
     end
     if key == "static" then
         if val == 1 then _val = true else _val = false end
     end
     if key == "orientation" and _obj_name == "st-mirror" then
         _val = (6 - val) % 4
     end
     if key == "transparent" then
         if val == 1 then _val = true else _val = false end
     end
     if key == "movable" then
         if val == 1 then _val = true else _val = false end
     end
     if key == "burnable" then
         if val == 1 then _val = true else _val = false end
     end
     if key == "eternal" then
         if val == 1 then _val = true else _val = false end
     end
     if key == "fastfire" then
         if val == 1 then _val = true else _val = false end
     end
     if key == "ignitable" then
         if val == 1 then _val = true else _val = false end
     end
     if key == "initfire" then
         if val == 1 then _val = true else _val = false end
     end
     if key == "noash" then
         if val == 1 then _val = true else _val = false end
     end
     if key == "secure" then
         if val == 1 then _val = true else _val = false end
     end
     if key == "type" and _obj_name == "st-door" then
         _key = "faces"
         if val == "h" then _val = "ns" else _val = "ew" end
     end
     if key == "type" and _obj_name == "fl-bridge" then
         _key = "flavor"
         if val == "x" then _val = "bw"
         elseif val == "y" then _val = "bn"
         else _val = "gc"
         end
     end
     if key == "type" and string.find(_obj_name, "fl-gradient", 1, true) == 1 then
         if val == 1 then
             enigma._SetAttrib(obj, "shape", "ps")
             enigma._SetAttrib(obj, "slope", 1)
         elseif val == 2 then
             enigma._SetAttrib(obj, "shape", "pn")
             enigma._SetAttrib(obj, "slope", 3)
         elseif val == 3 then
             enigma._SetAttrib(obj, "shape", "pe")
             enigma._SetAttrib(obj, "slope", 2)
         elseif val == 4 then
             enigma._SetAttrib(obj, "shape", "pw")
             enigma._SetAttrib(obj, "slope", 0)
         elseif val == 5 then
             enigma._SetAttrib(obj, "shape", "ose")
             enigma._SetAttrib(obj, "slope", 6)
         elseif val == 6 then
             enigma._SetAttrib(obj, "shape", "osw")
             enigma._SetAttrib(obj, "slope", 5)
         elseif val == 7 then
             enigma._SetAttrib(obj, "shape", "one")
             enigma._SetAttrib(obj, "slope", 7)
         elseif val == 8 then
             enigma._SetAttrib(obj, "shape", "onw")
             enigma._SetAttrib(obj, "slope", 4)
         elseif val == 9 then
             enigma._SetAttrib(obj, "shape", "ise")
             enigma._SetAttrib(obj, "slope", 6)
         elseif val == 10 then
             enigma._SetAttrib(obj, "shape", "ine")
             enigma._SetAttrib(obj, "slope", 7)
         elseif val == 11 then
             enigma._SetAttrib(obj, "shape", "isw")
             enigma._SetAttrib(obj, "slope", 5)
         elseif val == 12 then
             enigma._SetAttrib(obj, "shape", "inw")
             enigma._SetAttrib(obj, "slope", 4)
         elseif val == 13 then
             enigma._SetAttrib(obj, "shape", "twr")
             enigma._SetAttrib(obj, "slope", 5)
         elseif val == 14 then
             enigma._SetAttrib(obj, "shape", "twl")
             enigma._SetAttrib(obj, "slope", 4)
         elseif val == 15 then
             enigma._SetAttrib(obj, "shape", "tel")
             enigma._SetAttrib(obj, "slope", 6)
         elseif val == 16 then
             enigma._SetAttrib(obj, "shape", "ter")
             enigma._SetAttrib(obj, "slope", 7)
         elseif val == 17 then
             enigma._SetAttrib(obj, "shape", "tnr")
             enigma._SetAttrib(obj, "slope", 4)
         elseif val == 18 then
             enigma._SetAttrib(obj, "shape", "tnl")
             enigma._SetAttrib(obj, "slope", 7)
         elseif val == 19 then
             enigma._SetAttrib(obj, "shape", "tsl")
             enigma._SetAttrib(obj, "slope", 5)
         elseif val == 20 then
             enigma._SetAttrib(obj, "shape", "tsr")
             enigma._SetAttrib(obj, "slope", 6)
         elseif val == 21 then
             enigma._SetAttrib(obj, "shape", "ts")
             enigma._SetAttrib(obj, "slope", 1)
         elseif val == 22 then
             enigma._SetAttrib(obj, "shape", "tn")
             enigma._SetAttrib(obj, "slope", 3)
         elseif val == 23 then
             enigma._SetAttrib(obj, "shape", "te")
             enigma._SetAttrib(obj, "slope", 2)
         elseif val == 24 then
             enigma._SetAttrib(obj, "shape", "tw")
             enigma._SetAttrib(obj, "slope", 0)
         end
         return
     end
     
     if key == "hit_factor" then
         _key = "hit_strength"
     end
     if key == "mousefactor" then
         _key = "adhesion"
     end
     if key == "mouseforce" then
         _key = "adhesion"
     end
     if key == "blinking" then
         _key = "state"
     end
     if key == "attacknearest" then
         if val == 1 then _val = true else _val = false end
     end
     if key == "gohome" then
         if val == 1 then _val = true else _val = false end
     end
     if key == "fixed" then
         if val == 1 then
             enigma._SetAttrib(obj, "fragility", 0.0)
             enigma._SetAttrib(obj, "spreading", 0.0)
         else
             enigma._SetAttrib(obj, "fragility", nil)
             enigma._SetAttrib(obj, "spreading", nil)
         end
         return
     end
     if key == "player" then
         _key = "owner"
     end
     if key == "essential" then
        if val == -1 then
            _val = 1
        elseif val == 1 then
            _val = 2
        end
     end
     enigma._SetAttrib(obj, _key, _val)
end

function enigma.GetAttrib(obj, key)
     local _key = key
     local _obj_name = enigma.GetKind(obj)
     if key == "keycode" then
         _key = "code"
     end
     if key == "delay" then
         _key = "interval"
     end
     if key == "value" then
         _key = "coin_value"
     end
     if key == "force" then
         _key = "strength"
     end
     if key == "friction_factor" then
         _key = "friction"
     end
     if key == "gradient_factor" then
         _key = "gradient"
     end
     if key == "minlength"  then
         _key = "threshold"
     end
     if key == "target" and (_obj_name == "it-rubberband") then
         _key = "anchor2"
     end
     if key == "blackball" or key == "whiteball" then
         _key = "color"
     end
     if _obj_name == "st-oxyd" then
         _key = "oxydcolor"
     end
     if key == "dir" or key == "direction" then
         _key = "orientation"
     end
     if key == "on" then
         _key = "state"
     end
     if key == "load" then
         _key = "state"
     end
     if key == "blinking" then
         _key = "state"
     end
     if key == "mouseforce" then
         _key = "adhesion"
     end
     if key == "hit_factor" then
         _key = "hit_strength"
     end
     if key == "type" and _obj_name == "st-door" then
         local faces = enigma._GetAttrib(obj, "faces")
         if faces == "ns" then val = "h" else val = "v" end
         return val
     end
     if key == "type" and _obj_name == "fl-bridge" then
         local flavor = enigma._GetAttrib(obj, "flavor")
         if flavor == "bw" then return "x"
         elseif flavor == "bn" then return "y"
         else return "a"
         end
     end
     if key == "oxyd" and _obj_name == "st-puzzle" then
         local color = enigma._GetAttrib(obj, "color")
         if color == 2 then val = 0 else val = 1 end
         return val
     end
     if key == "player" then
         _key = "owner"
     end

     local val = enigma._GetAttrib(obj, _key)

     if key == "blackball" then
        if val == 0 then val = 1 else val = 0 end
     end
     if key == "whiteball" then
        if val == 1 then val = 1 else val = 0 end
     end
     if key == "connections" then
         if val == "" then val = 1
         elseif  val == "w"    then val = 2
         elseif  val == "s"    then val = 3
         elseif  val == "sw"   then val = 4
         elseif  val == "e"    then val = 5
         elseif  val == "ew"   then val = 6
         elseif  val == "es"   then val = 7
         elseif  val == "esw"  then val = 8
         elseif  val == "n"    then val = 9
         elseif  val == "nw"   then val = 10
         elseif  val == "ns"   then val = 11
         elseif  val == "nsw"  then val = 12
         elseif  val == "ne"   then val = 13
         elseif  val == "new"  then val = 14
         elseif  val == "nes"  then val = 15
         elseif  val == "nesw" then val = 16
         end
     end
     if _obj_name == "st-oxyd" then
         if key == "color" then
	     val = "" .. val   -- convert to string
	 end
     end
     if key == "targetx" then
         local d = enigma._GetAttrib(obj, "destination")
         if (en.usertype(d) == "position") then
             val = d.x
         end
     end
     if key == "targety" then
         local d = enigma._GetAttrib(obj, "destination")
         if (en.usertype(d) == "position") then
             val = d.y
         end
     end
     if key == "invisible" then
         if val == false then val = 0 else val = 1 end
     end
     if key == "orientation" and _obj_name == "st-mirror" then
         val = (5 - val) % 4 + 1
     end
     if key == "transparent" then
         if val == false then val = 0 else val = 1 end
     end
     if key == "movable" then
         if val == false then val = 0 else val = 1 end
     end
     if key == "attacknearest" then
         if val == false then val = 0 else val = 1 end
     end
     if key == "gohome" then
         if val == false then val = 0 else val = 1 end
     end
     if key == "essential" then
        if val == 1 then
            val = -1
        elseif val == 2 then
            val = 1
        end
     end
     return val
end

function enigma.GetObjectTemplate(kind)
    local _kind = RenamingObjectsOld2New[kind]
    if _kind == nil then
        _kind = kind
    end
    if kind == "st-key" or kind == "st-key_a" or kind == "st-key_b" or kind == "st-key_c" then
        _kind = "st_key"
    end
    return enigma._GetObjectTemplate(_kind)
end

MessageRenaming = {
    fl_bridge__openclose = "toggle",
    fl_bridge__signal = "toggle",
    it_blocker__trigger = "toggle",
    it_blocker__openclose = "toggle",
    it_burnable__trigger = "ignite",
    it_magnet__onoff = "toggle",
    it_meditation__trigger = "flip",
    it_vortex__openclose = "toggle",
    it_vortex__trigger = "toggle",
    it_wormhole__onoff = "toggle",
    st_blocker__openclose = "toggle",
    st_blocker__trigger = "toggle",
    st_blocker_new__openclose = "toggle",
    st_blocker_new__trigger = "toggle",
    st_boulder__direction = "orientate",
    st_door__openclose = "toggle",
    st_fourswitch__trigger = "toggle",
    st_floppy__onoff = "toggle",
    st_key__onoff = "toggle",
    st_laser__onoff = "toggle",
    st_lightpassenger__trigger = "toggle",
    st_lightpassenger__onoff = "toggle",
    st_mirror__trigger = "turn",
    st_oneway__direction = "orientate",
    st_oxyd__trigger = "open",
    st_passage__trigger = "signal",
    st_flat__trigger = "signal",
    st_polarswitch__onoff = "toggle",
    st_quake__trigger = "toggle",
    st_stoneimpulse__trigger = "signal",
    st_switch__onoff = "toggle",
    st_volcano__trigger = "toggle"
}
----------------------
-- old API 1.0      --
----------------------

TRUE = 1
FALSE = 0

EASY = 1
HARD = 2

difficult = options.Difficulty==2

function Tick (deltatime)
    -- do nothing by default
end

if difficult then
    enigma.SetScrambleIntensity(10)
else
    enigma.SetScrambleIntensity(3)
end

----------------------
-- Global variables --
----------------------

level_width = 0
level_height = 0

oxyd_default_flavor = "b"       -- Default flavor for oxyd stones.
oxyd_count = 0
oxyd_current_color = 0

DefaultAttribs = {}

---------------------
-- Helper routines --
---------------------

function PrintTable(t)
    for i,v in pairs(t) do
        if type(v)=="table" then
            print (i.." - "..v[1]..","..v[2])
        else
            print (i.." - "..v)
        end
    end
end


function Require(filename)
    enigma.LoadLib(string.sub(filename,8,string.len(filename)-4))
end


----------------------------------
-- Routines for creating levels --
----------------------------------

function CreateWorld(w, h)
    oxyd_default_flavor = "b"
    oxyd_count = 0
    oxyd_current_color = 0
    DefaultAttribs = {}

    level_width = w
    level_height = h
    world.Resize(w, h)
end


function SetDefaultAttribs(objname, attribs)
    local list = DefaultAttribs[objname]
    if list then
        for key,val in pairs(attribs) do list[key] = val end
    else
        DefaultAttribs[objname]= attribs
    end
end

function SetAttrib(obj, key,val)
    if key == "name" then
        enigma.NameObject(obj, val)
    end
    enigma.SetAttrib(obj, key, val)
end

GetAttrib = enigma.GetAttrib
GetKind   = enigma.GetKind

function set_attribs(obj, attrs)
    if not attrs then return end
    if type(attrs) ~= "table" then
        error("Can't assign attributes, expected table but got "..type(attrs)..".")
    end
    for key,val in pairs(attrs) do
	set_attrib(obj, key, val)
    end
end

function MakeObject(name, attrs)
    local obj = enigma.MakeObject(name)
    local a=DefaultAttribs[name]
    if a then set_attribs(obj, a) end
    set_attribs(obj, attrs)
    return obj
end

function set_floor(name, x, y, attrs)
    local obj = make_object(name, attrs)
    enigma.SetFloor(x,y,obj)
    return obj
end
function set_item(name, x, y, attrs)
    local obj = make_object(name, attrs)
    enigma.SetItem(x,y,obj)
    return obj
end
function set_stone(name, x,y, attrs)
    local obj=make_object(name, attrs)
    enigma.SetStone(x,y,obj)
    return obj
end

kill_stone = enigma.KillStone
kill_item = enigma.KillItem

function SendMessage (target, msg, arg)
    if type(target) == "string" then
        enigma.SendMessage (enigma.GetNamedObject (target), msg, arg)
    else
        enigma.SendMessage (target, msg, arg)
    end
end

function SetAttribs (target, attribs)
    if type (target) == "string" then
        set_attribs(enigma.GetNamedObject(target), attribs)
    else
        set_attribs(target, attribs)
    end
end


function AddRubberBand(obj1,obj2, strength,length, minlen)
    enigma.AddRubberBand(obj1,obj2, strength or 10, length or 2, minlen or 0)
end

function fill_floor(name, x0,y0, w,h)
    if x0 == nil then x0 = 0 end
    if y0 == nil then y0 = 0 end
    if w  == nil then w  = level_width end
    if h  == nil then h  = level_height end
    for y=y0,y0+h-1 do
	for x=x0,x0+w-1 do
            set_floor(name, x, y)
        end
    end
end

function fill_items(name, x0,y0,w,h)
    for y=y0,y0+h-1 do
	for x=x0,x0+w-1 do set_item(name, x, y) end
    end
end

function fill_stones(name, x0,y0, w,h)
    for y=y0,y0+h-1 do
        for x=x0,x0+w-1 do set_stone(name, x, y) end
    end
end

function draw_floor(name, xy0, xystep, n, attrs)
    local x,y = xy0[1],xy0[2]
    for i=1,n do
	set_floor(name, x, y, attrs)
	x = x+xystep[1]
	y = y+xystep[2]
    end
end

function draw_checkerboard_floor (name1, name2, x, y, w, h, attrs)
    if x == nil then x = 0 end
    if y == nil then y = 0 end
    if w  == nil then w  = level_width end
    if h  == nil then h  = level_height end
    for i=1,h do
        for j=1,w do
             if mod(i,2) == mod(j,2) then
                 set_floor (name1, x+j-1, y+i-1, attrs )
             else
                 set_floor (name2, x+j-1, y+i-1, attrs )
            end
        end
    end
end

function draw_items(name, xy0, xystep, n, attrs)
    local x,y = xy0[1],xy0[2]
    for i=1,n do
	set_item(name, x, y, attrs)
	x = x+xystep[1]
	y = y+xystep[2]
    end
end

function draw_stones(name, xy0, xystep, n, attrs)
    local x,y = xy0[1],xy0[2]
    for i=1,n do
	set_stone(name, x, y, attrs)
	x = x+xystep[1]
	y = y+xystep[2]
    end
end

function draw_border(stonename, x0, y0, w, h)
    if x0 == nil then x0 = 0 end
    if y0 == nil then y0 = 0 end
    if w  == nil then w  = level_width end
    if h  == nil then h  = level_height end
    draw_stones(stonename, {x0,y0}, {1,0}, w)
    draw_stones(stonename, {x0,y0+h-1},{1,0}, w)
    draw_stones(stonename, {x0,y0}, {0,1}, h)
    draw_stones(stonename, {x0+w-1,y0},{0,1}, h)
end

function set_stones(name, poslist, attrs)
    for i,xy in pairs(poslist) do
	set_stone(name, xy[1], xy[2], attrs)
    end
end

function SetActor(name, x, y, attrs)
    local obj=make_object(name, attrs)
    enigma.SetActor(x, y,obj)
    return obj
end

---------------------------------------------
-- Creation of particular kinds of objects --
---------------------------------------------
function fakeoxyd(x,y)
    return set_stone("st-fakeoxyd",x,y)
end


-- Create an oxyd stone with the current default flavor.
function oxyd(x, y, flavor, color)
    local f = flavor or oxyd_default_flavor
    local c = color or oxyd_current_color
    local a = {flavor=f, color=""..c}
    local obj = set_stone("st-oxyd", x, y, a)

    -- if "color" argument not provided, use next available color
    if not color then
        oxyd_count = oxyd_count +1

        if oxyd_count == 2 then
            oxyd_count = 0
            oxyd_current_color = oxyd_current_color + 1
        end
    end

    return obj
end

-- Shuffle the colors of all oxyd stones in the current landscape.
function oxyd_shuffle()
    enigma.SendMessage(enigma.GetObjectTemplate("st-oxyd"), "shuffle", nil)
end

-- Close all oxyd stones in the current landscape
function oxyd_closeall()
    enigma.SendMessage(enigma.GetObjectTemplate("st-oxyd"), "closeall", nil)
end

function oneway(x,y,orient)
    return set_stone("st-oneway",x,y,{orientation=orient})
end
function laser(x,y,is_on,dir)
    return set_stone("st-laser",x,y,{on=is_on, dir=dir})
end

function mirrorp(x,y,movable, transp,orient)
    return set_stone("st-pmirror", x, y, {movable=movable, transparent=transp,
                         orientation=orient})
end
function mirror3(x,y,movable, transp, orient)
    return set_stone("st-3mirror", x, y, {movable=movable, transparent=transp,
                         orientation = orient})
end

PUZ_0000=1                      -- hollow
PUZ_0001=2                      -- w
PUZ_0010=3                      -- s
PUZ_0011=4                      -- sw
PUZ_0100=5                      -- e
PUZ_0101=6                      -- ew
PUZ_0110=7                      -- es
PUZ_0111=8                      -- esw
PUZ_1000=9                      -- n
PUZ_1001=10                     -- nw
PUZ_1010=11                     -- ns
PUZ_1011=12                     -- nsw
PUZ_1100=13                     -- ne
PUZ_1101=14                     -- new
PUZ_1110=15                     -- nes
PUZ_1111=16                     -- nesw

-- functions using puzzle-style tiles:

function puzzle(x, y, conn)
    return set_stone("st-puzzle", x,y, {connections=conn})
end
function puzzle2(x, y, conn)
    return set_stone("st-puzzle", x,y, {connections=conn,oxyd=1})
end
function bigbrick(x, y, conn)
   return set_stone("st-bigbrick-n",x,y,{connections=conn})
end

--

function switch(x,y,target,action)
    return set_stone("st-switch", x,y, {target=target, action=action})
end

function abyss(x,y) set_floor("fl-abyss",x,y) end




-----------
-- ITEMS --
-----------
function hollow(x,y) set_item("it-hollow", x,y) end
function Document(x,y,t) set_item("it-document", x, y, {text=t}) end
function hammer(x,y) set_item("it-hammer",x,y) end
function dynamite(x,y) set_item("it-dynamite",x,y) end
function bomb(x,y) set_item("it-blackbomb",x,y) end
function keya(x,y) set_item("it-key", x,y, {keycode=1.0}) end
function keyb(x,y) set_item("it-key", x,y, {keycode=2.0}) end
function keyc(x,y) set_item("it-key", x,y, {keycode=3.0}) end

function shogundot1(x,y,attrs) set_item("it-shogun-s", x, y, attrs) end
function shogundot2(x,y,attrs) set_item("it-shogun-m", x, y, attrs) end
function shogundot3(x,y,attrs) set_item("it-shogun-l", x, y, attrs) end

function Wormhole(x,y,targetx, targety, attribs)
    local attrs = attribs or {}
    attrs.targetx = targetx
    attrs.targety = targety

    set_item("it-wormhole", x,y, attrs)
end

function Doorh(x,y,attrs)
    local attrs = attrs or {}
    attrs.type="h"
    set_stone("st-door",x,y,attrs)
end
function Doorv(x,y,attrs)
    local attrs = attrs or {}
    attrs.type="v"
    set_stone("st-door",x,y,attrs)
end

EAST = enigma.EAST
NORTH = enigma.NORTH
SOUTH = enigma.SOUTH
WEST = enigma.WEST

---------------
-- GRADIENTS --
---------------

SLOPE_S         = 1
SLOPE_N         = 2
SLOPE_E         = 3
SLOPE_W         = 4
SLOPE_LARGE_SE  = 5
SLOPE_LARGE_SW  = 6
SLOPE_LARGE_NE  = 7
SLOPE_LARGE_NW  = 8
SLOPE_SMALL_SE  = 9
SLOPE_SMALL_NE  = 10
SLOPE_SMALL_SW  = 11
SLOPE_SMALL_NW  = 12
SLOPE_S_FORCE_W = 13
SLOPE_N_FORCE_W = 14
SLOPE_S_FORCE_E = 15
SLOPE_N_FORCE_E = 16
SLOPE_E_FORCE_N = 17
SLOPE_W_FORCE_N = 18
SLOPE_E_FORCE_S = 19
SLOPE_W_FORCE_S = 20
FLAT_FORCE_S    = 21
FLAT_FORCE_N    = 22
FLAT_FORCE_E    = 23
FLAT_FORCE_W    = 24

function Gradient( x, y, type )
   if (type==nil) then error("Illegal gradient type"); end
   if (type>=1 and type<=24) then
      set_floor( "fl-gradient", x, y, {type=type} )
   else
      error("Unknown gradient type '"..type.."'");
   end
end


Signal = enigma.AddSignal
SetItem = set_item

----------------------------------------------------
-- Define lowercase aliases for various functions --
----------------------------------------------------

create_world = CreateWorld
get_attrib = GetAttrib
get_kind = GetKind
set_attrib = SetAttrib
make_object = MakeObject
set_actor = SetActor

document = Document
doorh    = Doorh
doorv    = Doorv
gradient = Gradient
wormhole = Wormhole

