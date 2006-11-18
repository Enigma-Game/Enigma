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
    FullScreen        = 1,
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


----------------------------------
-- Define some new object types --
----------------------------------
function def_stone(name, sound)
    world.DefineSimpleStone(name,sound or "stone",0,0)
end
function def_stone_hollow(name)
    world.DefineSimpleStone(name,"",1,0)
end
function def_stone_glass(name, sound)
    world.DefineSimpleStone(name,sound or "stone",0,1)
end
function def_stone_movable_glass(name, sound)
    world.DefineSimpleStoneMovable(name,sound or "stone",1)
end
function def_stone_movable(name, sound)
    world.DefineSimpleStoneMovable(name,sound or "stone",0)
end
function def_floor(name, frict, mfactor, burnable, firetransform)
    world.DefineSimpleFloor(name, frict, mfactor, burnable, firetransform)
end

def_stone_movable("st-brownie", "cloth")
def_stone("st-invisible")
def_stone("st-marble")
def_stone_movable("st-marble_move")
def_stone_hollow("st-marble_hole")
def_stone("st-rock1")
def_stone_movable("st-rock1_move")
def_stone_hollow("st-rock1_hole")
def_stone("st-rock2")
def_stone_hollow("st-rock2_hole")
def_stone("st-rock3")
def_stone_movable("st-rock3_move")
def_stone_hollow("st-rock3_hole")
def_stone("st-rock4")
def_stone("st-rock5")
def_stone("st-rock6")
def_stone("st-rock7")
def_stone("st-rock8")
def_stone("st-rock9")
def_stone("st-redrock")
def_stone("st-camouflage", "cloth")
def_stone_movable("st-camouflage_move", "cloth")
def_stone_hollow("st-camouflage_hole")
def_stone("st-greenbrown")
def_stone_movable("st-greenbrown_move", "cloth")
def_stone_hollow("st-greenbrown_hole")
def_stone("st-brick")
def_stone("st-woven")
def_stone("st-wood_001")
def_stone("st-bluegray")
def_stone_hollow("st-bluegray_hole")
def_stone("st-stone1")
def_stone("st-stone2")
def_stone("st-bumps")
def_stone("st-beads")
def_stone("st-metal")
def_stone_hollow("st-metal_hole")
def_stone("st-blue-sand")
def_stone("st-yellow")

function mkoxydlikes(flavor)
   def_stone("st-likeoxyd"..flavor)
   def_stone("st-likeoxyd"..flavor.."-open")
end
mkoxydlikes("a")
mkoxydlikes("b")
mkoxydlikes("c")
mkoxydlikes("d")

def_stone_glass("st-glass")
def_stone_movable_glass("st-glass_move")
def_stone_glass("st-glass1")
def_stone_hollow("st-glass1_hole")
def_stone_movable_glass("st-glass1_move")
def_stone_glass("st-glass2")
def_stone_hollow("st-glass2_hole")
def_stone_glass("st-glass3")

-- Floor(kind, friction, mousefactor, burnable, firetransform)
def_floor("fl-bluegray",     4.5,   1.5,    true,   "")
def_floor("fl-bluegreen",    6.0,   2.0,    true,   "")
def_floor("fl-bluegreenx",   0.4,   1.0,    true,   "")
def_floor("fl-brick",        3.5,   2.0,   false,   "")
def_floor("fl-bumps",        5.0,   1.2,   false,   "")
def_floor("fl-concrete",     4.5,   1.3,   false,   "")
def_floor("fl-dunes",        1.3,   1.0,   false,   "")
def_floor("fl-floor_001",    1.5,   2.5,    true,   "fl-abyss")
def_floor("fl-gravel",       3.2,   1.5,   false,   "")
def_floor("fl-gray",         5.0,   3.0,   false,   "")
def_floor("fl-hay",          5.0,   1.5,    true,   "")
def_floor("fl-himalaya",     5.0,   2.0,   false,   "")
def_floor("fl-inverse",      3.0,  -2.0,   false,   "")
def_floor("fl-inverse2",     3.0,  -2.0,   false,   "")
def_floor("fl-leaves",       4.0,   1.5,    true,   "fl-dunes")
def_floor("fl-leavesb",      4.0,   1.5,    true,   "fl-dunes")
def_floor("fl-leavesc1",     4.0,   1.5,    true,   "fl-dunes")
def_floor("fl-leavesc2",     4.0,   1.5,    true,   "fl-dunes")
def_floor("fl-leavesc3",     4.0,   1.5,    true,   "fl-dunes")
def_floor("fl-leavesc4",     4.0,   1.5,    true,   "fl-dunes")
def_floor("fl-leavesd1",     4.0,   1.5,    true,   "fl-dunes")
def_floor("fl-leavesd2",     4.0,   1.5,    true,   "fl-dunes")
def_floor("fl-leavesd3",     4.0,   1.5,    true,   "fl-dunes")
def_floor("fl-leavesd4",     4.0,   1.5,    true,   "fl-dunes")
def_floor("fl-leavese1",     4.0,   1.5,    true,   "fl-dunes")
def_floor("fl-leavese2",     4.0,   1.5,    true,   "fl-dunes")
def_floor("fl-leavese3",     4.0,   1.5,    true,   "fl-dunes")
def_floor("fl-leavese4",     4.0,   1.5,    true,   "fl-dunes")
def_floor("fl-light",        2.2,   1.6,    true,   "")
def_floor("fl-lightgray",    3.0,   1.6,    true,   "")
def_floor("fl-darkgray",     3.0,   1.6,   false,   "")
def_floor("fl-marble",       6.4,   2.0,    true,   "")
def_floor("fl-metal",        3.0,   2.0,   false,   "")
def_floor("fl-metal1",       3.0,   2.0,   false,   "")
def_floor("fl-metal2",       3.0,   2.0,   false,   "")
def_floor("fl-metal3",       3.0,   2.0,   false,   "")
def_floor("fl-metal4",       3.0,   2.0,   false,   "")
def_floor("fl-metal5",       3.0,   2.0,   false,   "")
def_floor("fl-metal6",       3.0,   2.0,   false,   "")
def_floor("fl-mortar",       7.2,   1.8,   false,   "")
def_floor("fl-normal",       4.0,   2.0,   false,   "")
def_floor("fl-normal_x",     3.0,   2.0,   false,   "")
def_floor("fl-plank",        5.5,   2.0,    true,   "fl-abyss")
def_floor("fl-red",          0.9,   1.0,    true,   "")
def_floor("fl-rock",         6.5,   2.2,   false,   "")
def_floor("fl-rough",        7.0,   2.0,    true,   "")
def_floor("fl-rough_medium", 5.0,   1.2,    true,   "")
def_floor("fl-rough_slow",   7.0,   0.5,    true,   "")
def_floor("fl-rough-blue",   7.0,   2.0,    true,   "")
def_floor("fl-rough-red",    7.0,   2.0,    true,   "")
def_floor("fl-sahara",       6.4,   2.0,   false,   "")
def_floor("fl-samba",        6.0,   2.0,    true,   "fl-abyss")
def_floor("fl-samba1",       6.0,   2.0,    true,   "fl-abyss")
def_floor("fl-samba2",       6.0,   2.0,    true,   "fl-abyss")
def_floor("fl-sand",         6.0,   2.0,   false,   "")
def_floor("fl-space",        0.0,   0.0,   false,   "")
def_floor("fl-springboard",  4.0,   2.0,   false,   "")
def_floor("fl-stone",        1.4,   1.0,   false,   "")
def_floor("fl-tigris",       6.0,   2.0,    true,   "")
def_floor("fl-wood",         6.4,   2.0,    true,   "fl-abyss")
def_floor("fl-wood1",        6.4,   2.0,    true,   "fl-abyss")
def_floor("fl-wood2",        6.4,   2.0,    true,   "fl-abyss")
def_floor("fl-woven",        6.5,   3.0,    true,   "")
def_floor("fl-woven_orange", 2.5,   0.0,    true,   "")
def_floor("fl-black",        3.0,   1.5,   false,   "")
def_floor("fl-white",        3.0,   1.5,   false,   "")
def_floor("fl-trigger",      3.0,   1.5,    true,   "")
def_floor("fl-abyss_fake",   3.0,   2.0,   false,   "")

----------------------------------------------------------------------
-- Sound handling
----------------------------------------------------------------------

-- Create a new sound definition
function Sound(t)
    local tt = {}
    tt.file     = t.file
    tt.volume   = t.volume or 1.0
    tt.loop     = t.loop or nil
    tt.global   = t.global or nil
    tt.priority = t.priority or 1
    return tt
end

-- Copy all key/value pairs only present in t1 to t2.
function copy_missing (t1, t2)
    for k,v in pairs(t1) do
        if not t2[k] then
            t2[k] = v
        end
    end
end


-- The list of available sound tables
soundtables = {}

-- The currently active sound table
soundtable = {}
soundtable_name = ""


function AddSoundTable (name, t)
    e = {}
    e.name = name
    e.table = t
    soundtables[name] = e

    -- current soundtable has been replaced
    if (name == soundtable_name) then
        ActivateSoundTable (name)
    end
end


function ActivateSoundTable (name)
    st = soundtables[name]
    if st then
        soundtable = st.table
        soundtable_name = name
    end
end

function SoundEvent (eventname, xpos, ypos, volume) 
    local s = soundtable[eventname]
    if s then
        if type(s) == "string" then
            enigma.PlaySound (s, xpos, ypos, volume, 1.0)
        elseif s.global then
            enigma.PlaySoundGlobal (s.file, s.volume * volume, s.priority)
        else
            enigma.PlaySound (s.file, xpos, ypos, s.volume * volume, s.priority)
        end
        return 1
    end
    print ("Undefined sound event " .. eventname .. " @ " .. xpos .. "," .. ypos)
    return nil
end


---------------------------------
-- Definition of sound effects --
---------------------------------

------------------------
-- Enigma Sound table --
------------------------

soundtable_enigma = {
    [""]           = "",        -- empty sound
    ballcollision  = "ballcollision",
    blackbomb      = "explosion1",
    blockerdown    = "",
    blockerup      = "",
    booze          = "",
    bumper         = "bumper",
    cloth          = "st-thud",
    coinslotoff    = "",
    coinsloton     = "st-coinslot",
    crack          = "",
    doorclose      = "doorclose",  -- missing
    dooropen       = "dooropen",   -- missing
    drown          = "drown",
    dynamite       = "explosion2", -- replace?
    electric       = "st-stone",   -- replace
    exit           = Sound { file="exit", global=1 },  -- missing
    extinguish     = "",
    fakeoxyd       = Sound { file="st-fakeoxyd", volume=0.3 },
    falldown       = "falldown",   -- missing, unused (falling is shatter!)
    fart           = "fart",
    finished       = Sound { file="finished", global=1 },  -- missing
    floordestroy   = "",
    fourswitch     = "st-switch", 
    glass          = "st-metal",   -- replace
    hitfloor       = "stone",      -- missing, used by it-vortex
    impulse        = "",
    intro          = Sound { file="intro", global=1 },  -- missing
    invrotate      = Sound { file="invrotate", global=1 },
    itemtransform  = "st-magic",
    jump           = "boink",
    jumppad        = "",
    landmine       = "explosion2",  -- replace?
    laserloop      = "",
    laseron        = "st-laser",
    laseroff       = "",
    lock           = "",
    magneton       = "",
    magnetoff      = "",
    mail           = "",
    menuexit        = Sound { file="menuexit", global=1 },
    menumove        = Sound { file="menumove",global=1 },
    menuok          = Sound { file="menuok",global=1 },
    menustop        = Sound { file="menustop",global=1 },
    menuswitch      = Sound { file="menuswitch",global=1 },
    metal          = "st-metal",
    mirrorturn     = "st-mirrorturn",
    movebig        = "st-move",
    moveslow       = "st-move",
    movesmall      = "st-move",
    oxydclose      = "st-oxydclose",  -- missing, not neccessary I think
    oxydopen       = "st-oxydopen",
    oxydopened     = "st-oxydopened",
    pickup         = Sound {file="pickup", global=1},
    puller         = "",
    puzzlerotate   = "st-move",
    rubberband     = "boing",
    scissors       = "",
    seedgrow       = "seedgrow",  -- missing
    shatter        = "shatter",
    shattersmall   = "shatter",   -- replace, "shattersmall" is missing
    shogunoff      = "",
    shogunon       = "",
    skull          = "",
    spade          = "",
    squish         = "",
    stone          = "st-stone",
    stonedestroy   = "explosion0",
    stonepaint     = "st-magic",
    stonetransform = "st-magic",
    swamp          = "swamped",
    switchmarbles  = "warp",      -- missing
    switchoff      = "",
    switchon       = "st-switch",
    switchplayer   = "switch",
    sword          = "",
    thief          = "thief",     -- missing
    triggerdown    = "it-triggerdown",
    triggerup      = "it-triggerup",
    turnstileleft  = "turnstileleft",  -- missing
    turnstileright = "turnstileright", -- missing
    umbrellaoff    = "",
    umbrellaon     = "",
    umbrellawarn   = "",
    unlock         = "unlock",    -- missing
    vortexclose    = "doorclose", -- missing
    vortexopen     = "dooropen",  -- missing
    warp           = "warp",      -- missing, maybe suck2?
    whitebomb      = "explosion1",
    wood           = "wood",
    yinyang        = "st-magic",
}



soundtable_oxyd = {
    [""]           = "",        -- empty sound
    ballcollision  = "OXKLICK3.SDD",
    blackbomb      = "OXCRASH2.SDD",
    blockerdown    = "",
    blockerup      = "",
    booze          = "",
    bumper         = "OXWOUOU.SDD",
    cloth          = "OXKLICK4.SDD",
    coinslotoff    = "",
    coinsloton     = "OXMONEY.SDD",
    crack          = Sound { file="OXCRACK.SDD", volume=0.7 },
    doorclose      = "OXMOTOR.SDD",
    dooropen       = "OXMOTOR.SDD",
    drown          = "OXBLOOP.SDD",
    dynamite       = "OXCRASH1.SDD",
    electric       = "OXKLICK6.SDD",
    exit           = Sound { file="OXEXIT.SDD", global=1 },
    extinguish     = "",
    fakeoxyd       = "",
    falldown       = "",
    fart           = "OXUNTITL.SDD",
    finished       = Sound { file="OXFINITO.SDD", global=1 },
    floordestroy   = "OXCRASH2",
    fourswitch     = "",
    glass          = "OXKLICK1.SDD",
    hitfloor       = "OXKLICK1.SDD",
    impulse        = "OXWOUOU.SDD",
    intro          = Sound { file="OXINTRO.SDD", global=1 },
    invrotate      = Sound { file="OXINVROT.SDD", global=1 },
    itemtransform  = "OXMAGIC1.SDD",
    jump           = "OXJUMP.SDD",
    jumppad        = "",
    landmine       = "explosion1",
    laserloop      = "",
    laseron        = "OXLASER.SDD",
    laseroff       = "",
    lock           = "",
    magneton       = "",
    magnetoff      = "",
    mail           = "",
    metal          = "OXKLICK2.SDD",
    mirrorturn     = "OXTURN.SDD",
    movebig        = "OXMOVE.SDD",
    moveslow       = "OXMOVE.SDD",
    movesmall      = "OXMOVE.SDD",
    oxydclose      = "OXMEMCL.SDD",
    oxydopen       = "OXMEMOP.SDD",
    oxydopened     = "OXMEMOK.SDD",
    pickup         = "OXINVENT.SDD",
    puller         = "OXPULLER.SDD",
    puzzlerotate   = "OXMOVE.SDD",
    rubberband     = "OXBOING.SDD",
    scissors       = "OXCUT.SDD",
    seedgrow       = "",
    shatter        = "OXKLIRR.SDD",
    shattersmall   = "OXKLIRR.SDD",
    shogunoff      = "",
    shogunon       = "",
    skull          = "",
    spade          = "",
    squish         = "OXMATSCH.SDD",
    stone          = "OXKLICK1.SDD",
    stonedestroy   = "OXCRASH2.SDD",
    stonepaint     = "OXMAGIC.SDD",
    stonetransform = "OXMAGIC.SDD",
    swamp          = "OXBLOOP.SDD",
    switchmarbles  = "",
    switchoff      = "",
    switchon       = "",
    switchplayer   = "",
    sword          = "",
    thief          = "OXTHIEF.SDD",
    triggerdown    = "OXSWON.SDD",
    triggerup      = "OXSWOFF.SDD",
    turnstileleft  = "OXMAGIC4.SDD",
    turnstileright = "OXMAGIC3.SDD",
    umbrellaoff    = "",
    umbrellaon     = "",
    umbrellawarn   = "",
    unlock         = "",
    vortexclose    = "OXMOTOR.SDD",
    vortexopen     = "OXMOTOR.SDD",
    warp           = "OXTRANS.SDD",
    whitebomb      = "OXCRASH2.SDD",
    wood           = "OXKLICK1.SDD",
    yinyang        = "OXMAGIC.SDD",
}    
copy_missing (soundtable_enigma, soundtable_oxyd)

soundtable_oxm = {
    turnstileleft = "OXTURNL.SDD",
    turnstileright = "OXTURNL.SDD"
}
copy_missing (soundtable_oxyd, soundtable_oxm)

--------------------------------------
-- Register predefined sound tables --
--------------------------------------

AddSoundTable ("Oxyd", soundtable_oxyd)
AddSoundTable ("OxydMag", soundtable_oxm)
AddSoundTable ("Enigma", soundtable_enigma)

ActivateSoundTable ("Enigma")

------------------------------------------
-- Load external sound description file --
------------------------------------------
-- local f = enigma.FindDataFile("sound/sound.lua")
-- if f then                       -- present?
--     dofile (f)
-- end

--soundtable = soundtable_enigma
