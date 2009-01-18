------------------------------------------------------------------------
-- Copyright (C) 2002,2003,2004,2005 Daniel Heck
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
-- $Id: startup.lua,v 1.21 2004/02/15 11:54:36 dheck Exp $
------------------------------------------------------------------------

function Require(filename)
    dofile(enigma.FindDataFile(filename))
end

---------------------
-- Option handling --
---------------------

-- Set default values for options
options = {
    MouseSpeed        = 7.0,
    FullScreen        = 0,
    Nozoom            = 1,
    InGameMusic       = 0,                  -- no music during the game
    Difficulty        = 2,                  -- hard
    Tournament        = 0,                  -- tournament mode off
    VideoMode         = 0,
    Gamma             = 1.0,
    LevelMenuPosition = 0.0,

    SoundSet          = 0,
    -- 0   = 'enigma' for enigma, appropriate oxyd sound sets for oxyd versions
    -- 1   = 'enigma'
    -- 2.. = OxydVersion-2

    SkipSolvedLevels = 0,
    TimeHunting      = 0,

    SoundVolume      = 1.0,
    MusicVolume      = 1.0,
    StereoSeparation = 10.0,

    MenuMusicFile  = "sound/menu.s3m",
    LevelMusicFile = "sound/Emilie.xm",

    Language = "",
    History = "",
}

-- Table of level stats is initially empty
stats = {}


-- For backwards compatibility only --
function options.LevelStat2 (key, stat)
    if stat[1] < 0 then stat[1] = -1 end
    if stat[2] < 0 then stat[2] = -1 end
    if stat[3] < 0 then stat[3] = 0 end

    gsub(key, "([^#]*)#(.+)", function (pack,lev) stats[lev] = stat end)
end

-- For backwards compatibility only --
function options.LevelStat (key, stat)
    -- old level status (containing modtime of .lua file)
    -- [keep this function here for compatibility with old config files]

    -- Enigma accidentally used to save times in the wrong numeric format.
    -- The following two lines work around this problem
    if stat[1] < 0 then stat[1] = -1 end
    if stat[2] < 0 then stat[2] = -1 end
    if stat[3] < 0 then stat[3] = 0 end

    stat[4] = stat[4] or 0
    if stat[4] > 0 then
        stat[4] = 1
    end -- level status has 'solved_at' time -> default to revision 1

    gsub(key, "([^#]*)#(.+)", function (pack,lev) stats[lev] = stat end)
end

function LoadOptions (filename)
    dofile(filename)
end

------------------------------
-- Define the simple stones --
------------------------------
function def_stone(name, sound)
    world.DefineSimpleStone(name,sound or "stone",0,0)
end
function def_stone_hollow(name)
    world.DefineSimpleStone(name,"",1,0)
end
function def_stone_movable(name, sound)
    world.DefineSimpleStoneMovable(name,sound or "stone",0)
end


------------------------------
-- Define the simple floors --
------------------------------
function def_floor(name, frict, mfactor, burnable, firetransform, frames)
    world.DefineSimpleFloor(name, frict, mfactor, burnable, firetransform)
    if frames then
        world.DefineSimpleFloor(name.."_framed", frict, mfactor, burnable, firetransform)    
    end
end

-- Floor(kind, friction, mousefactor, burnable, firetransform, frames)
def_floor("fl_bluegray",     4.5,   1.5,    true,   "",         true)
def_floor("fl_bluegreen",    6.0,   2.0,    true,   "",         true)
def_floor("fl_aquamarine",   0.4,   1.0,    true,   "",         true)
def_floor("fl_brick",        3.5,   2.0,   false,   "",         true)
def_floor("fl_pinkbumps",    5.0,   1.2,   false,   "",         true)
def_floor("fl_concrete",     4.5,   1.3,   false,   "",         true)
def_floor("fl_dunes",        1.3,   1.0,   false,   "",         true)
def_floor("fl_bast",         1.5,   2.5,    true,   "fl_abyss", true)
def_floor("fl_gravel",       3.2,   1.5,   false,   "",         true)
def_floor("fl_gray",         5.0,   3.0,   false,   "",         true)
def_floor("fl_himalaya",     5.0,   2.0,   false,   "",         true)
def_floor("fl_inverse",      3.0,  -2.0,   false,   "",         false)
def_floor("fl_inverse_white",3.0,  -2.0,   false,   "",         false)
def_floor("fl_inverse_gray", 3.0,  -2.0,   false,   "",         false)
def_floor("fl_lawn",         4.0,   1.5,    true,   "fl_dunes", false)
def_floor("fl_lawn_a",       4.0,   1.5,    true,   "fl_dunes", false)
def_floor("fl_lawn_b",       4.0,   1.5,    true,   "fl_dunes", false)
def_floor("fl_lawn_c1",      4.0,   1.5,    true,   "fl_dunes", false)
def_floor("fl_lawn_c2",      4.0,   1.5,    true,   "fl_dunes", false)
def_floor("fl_lawn_c3",      4.0,   1.5,    true,   "fl_dunes", false)
def_floor("fl_lawn_c4",      4.0,   1.5,    true,   "fl_dunes", false)
def_floor("fl_lawn_d1",      4.0,   1.5,    true,   "fl_dunes", false)
def_floor("fl_lawn_d2",      4.0,   1.5,    true,   "fl_dunes", false)
def_floor("fl_lawn_d3",      4.0,   1.5,    true,   "fl_dunes", false)
def_floor("fl_lawn_d4",      4.0,   1.5,    true,   "fl_dunes", false)
def_floor("fl_lawn_e1",      4.0,   1.5,    true,   "fl_dunes", false)
def_floor("fl_lawn_e2",      4.0,   1.5,    true,   "fl_dunes", false)
def_floor("fl_lawn_e3",      4.0,   1.5,    true,   "fl_dunes", false)
def_floor("fl_lawn_e4",      4.0,   1.5,    true,   "fl_dunes", false)
def_floor("fl_ivory",        2.2,   1.6,    true,   "",         true)
def_floor("fl_platinum",     3.0,   1.6,    true,   "",         true)
def_floor("fl_darkgray",     3.0,   1.6,   false,   "",         true)
def_floor("fl_marble",       6.4,   2.0,    true,   "",         true)
def_floor("fl_metal",        3.0,   2.0,   false,   "",         true)
def_floor("fl_metal_1",      3.0,   2.0,   false,   "",         true)
def_floor("fl_metal_2",      3.0,   2.0,   false,   "",         true)
def_floor("fl_metal_3",      3.0,   2.0,   false,   "",         true)
def_floor("fl_metal_4",      3.0,   2.0,   false,   "",         true)
def_floor("fl_metal_5",      3.0,   2.0,   false,   "",         true)
def_floor("fl_metal_6",      3.0,   2.0,   false,   "",         true)
def_floor("fl_metal_7",      3.0,   2.0,   false,   "",         true)
def_floor("fl_metal_7n",     4.0,   2.0,   false,   "",         true)
def_floor("fl_mortar",       7.2,   1.8,   false,   "",         true)
def_floor("fl_plank",        5.5,   2.0,    true,   "fl_abyss", true)
def_floor("fl_red",          0.9,   1.0,    true,   "",         true)
def_floor("fl_rough",        7.0,   2.0,    true,   "",         true)
def_floor("fl_blueslab",     7.0,   2.0,    true,   "",         true)
def_floor("fl_redslab",      7.0,   2.0,    true,   "",         true)
def_floor("fl_sahara",       6.4,   2.0,   false,   "",         true)
def_floor("fl_samba",        6.0,   2.0,    true,   "fl_abyss", true)
def_floor("fl_samba_h",      6.0,   2.0,    true,   "fl_abyss", true)
def_floor("fl_samba_v",      6.0,   2.0,    true,   "fl_abyss", true)
def_floor("fl_sand",         6.0,   2.0,   false,   "",         true)
def_floor("fl_white",        4.0,   2.0,   false,   "",         true)
def_floor("fl_stone",        1.4,   1.0,   false,   "",         true)
def_floor("fl_tigris",       6.0,   2.0,    true,   "",         true)
def_floor("fl_woven",        6.5,   3.0,    true,   "",         true)
def_floor("fl_adhesionless", 2.5,   0.0,    true,   "",         false)
def_floor("fl_dark",         3.0,   1.5,   false,   "",         true)
def_floor("fl_bright",       3.0,   1.5,   false,   "",         true)
def_floor("fl_trigger",      3.0,   1.5,    true,   "",         true)
def_floor("fl_fake_abyss",   3.0,   2.0,   false,   "",         false)

