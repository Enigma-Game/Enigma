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
------------------------------------------------------------------------

-- This file defines the models used in Enigma.
dofile(FindDataFile("models.lua"))

DefImage("dummy")
DefImage("invisible")

--------------------------------------------------------------------------------
--                                 ACTOR MODELS                               --
--------------------------------------------------------------------------------
Progress(0, "Loading actor models")

------------------------
-- ball-shaped actors --
------------------------

-- ac-blackball and ac-whiteball--
do
    local images,frames,shadows

    -- Normal Blackball
    images = SpriteImages("fg-blackball", 2, 0.5, 0.32)
    shadows = SpriteImage("sh-blackball", 0.4, 0.29)
    DefShModel("ac-blackball", "fg-blackball1", "sh-blackball")
    DefShModel("ac-blackball-shine", "fg-blackball2", "sh-blackball")

    -- Normal Whiteball
    -- Use shadow from black ball
    DefAlias("sh-whiteball", "sh-blackball")
    images = SpriteImages("fg-whiteball", 2, 0.5, 0.32)
    DefShModel("ac-whiteball", "fg-whiteball1", "sh-whiteball")
    DefShModel("ac-whiteball-shine", "fg-whiteball2", "sh-whiteball")

    -- Falling Blackball
    images = SpriteImages("ac-blackball-fall", 10)
    frames = ComposeFrames(images,{70,65,60,55,50,50,50,50,50,50,50})
    DefAnim("ac-blackball-fall", frames)
    DefAlias("ac-blackball-fallen", "invisible")

    -- Appearing / disappearing Blackball
    -- use the images from falling
    DefAnim("ac-blackball-appear", ReverseFrames(BuildFrames(images, 25)))
    DefAnim("ac-blackball-disappear", BuildFrames(images, 25))

    -- Falling Whiteball
    images = SpriteImages("ac-whiteball-fall", 10)
    frames = ComposeFrames(images,{70,65,60,55,50,50,50,50,50,50,50})
    DefAnim("ac-whiteball-fall", frames)
    DefAlias("ac-whiteball-fallen", "invisible")

    -- Appearing / disappearing Whiteball
    -- use the images from falling
    DefAnim("ac-whiteball-appear", ReverseFrames(BuildFrames(images, 25)))
    DefAnim("ac-whiteball-disappear", BuildFrames(images, 25))

    -- Jumping Blackball
    images  = SpriteImages("ac-blackball-jump", 4)
    shadows = SpriteImages("sh-blackball-jump", 4, 0.4)
    frames  = {}
    for i=1,4 do
        DefShModel("bb-jump"..i, images[i], shadows[i])
        table.insert(frames, "bb-jump"..i)
    end
    DefAnim("ac-blackball-jump", PingPong(BuildFrames(frames, 70)))

    -- Jumping Whiteball
    -- Use shadow from black ball
    images  = SpriteImages("ac-whiteball-jump", 4)
    frames  = {}
    for i=1,4 do
        DefShModel("wb-jump"..i, images[i], shadows[i])
        table.insert(frames, "wb-jump"..i)
    end
    DefAnim("ac-whiteball-jump", PingPong(BuildFrames(frames, 70)))

    -- Sinking Blackball
    shadows = SpriteImages("sh-blackball-sink", 7, 0.4)
    images = SpriteImages("fg-blackball-sink", 7)
    for i=1,table.getn(images) do
        DefShModel("ac-blackball-sink"..(i-1), images[i], shadows[i])
    end
    DefAlias("ac-blackball-sunk", "invisible")

    -- Sinking Whiteball
    -- Use shadow from black ball
    images = SpriteImages("fg-whiteball-sink", 7)
    for i=1,table.getn(images) do
        DefShModel("ac-whiteball-sink"..(i-1), images[i], shadows[i])
    end
    DefAlias("ac-whiteball-sunk", "invisible")

    -- Shattering Blackball
    Sprite({name="ac-blackball-shatter", nimages=5, framelen=60})
    DefAlias("ac-blackball-shattered", "ac-blackball-shatter5")

    -- Shattering Whiteball
    Sprite({name="ac-whiteball-shatter", nimages=5, framelen=60})
    DefAlias("ac-whiteball-shattered", "ac-whiteball-shatter5")
end

-- ac-whiteball-small --
do
    local images, frames, shadows

    -- Normal
    SpriteImage ("sh-whiteball-small", 0.4, 0.41)
    SpriteImage ("fg-whiteball-small", 0.5, 0.43)
    DefShModel("ac-whiteball-small", "fg-whiteball-small", "sh-whiteball-small")
    DefAlias ("ac-whiteball-small-shine", "ac-whiteball-small")

    -- Falling
    images = SpriteImages ("ac-whiteball-small-fall", 5, 0.5, 0.43)
    table.insert(images, "invisible")
    DefAnim("ac-whiteball-small-fall", ComposeFrames(images,{70,65,60,55,50,30}))
    DefAlias("ac-whiteball-small-fallen", "invisible")

    -- Appearing / disappearing
    -- use the images from falling
    DefAnim("ac-whiteball-small-appear", ReverseFrames(BuildFrames(images, 25)))
    DefAnim("ac-whiteball-small-disappear", BuildFrames(images, 25))

    -- Jumping
    images  = SpriteImages("ac-whiteball-small-jump", 4)
    shadows = SpriteImages("sh-whiteball-small-jump", 4, 0.4)
    frames  = {}
    for i=1,4 do
        DefShModel("sb-jump"..i, images[i], shadows[i])
        table.insert(frames, "sb-jump"..i)
    end
    DefAnim("ac-whiteball-small-jump", PingPong(BuildFrames(frames, 70)))

    -- sinking
    DefAlias ("ac-whiteball-small-sink0", "ac-whiteball-small-fall1")
    DefAlias ("ac-whiteball-small-sink1", "ac-whiteball-small-fall2")
    DefAlias ("ac-whiteball-small-sink2", "ac-whiteball-small-fall3")
    DefAlias ("ac-whiteball-small-sink3", "ac-whiteball-small-fall4")
    DefAlias ("ac-whiteball-small-sink4", "ac-whiteball-small-fall5")
    DefAlias ("ac-whiteball-small-sink5", "ac-whiteball-small-fall5")
    DefAlias ("ac-whiteball-small-sink6", "ac-whiteball-small-fall5")
    DefAlias ("ac-whiteball-small-sunk", "invisible")

    -- Shattering
    images = SpriteImages ("ac-whiteball-small-shatter", 5)
    DefAnim("ac-whiteball-small-shatter", BuildFrames(images, 60))
    SpriteImage ("ac-whiteball-small-shattered")
end

-- ac-killerball --
do
    DefAlias("ac-killerball", "ac-whiteball-small")
end

-- Marbles in inventory  --
do
    DefImage("inv-blackball")
    DefImage("inv-whiteball")
end

----------------------------
-- not ball-shaped actors --
----------------------------

-- ac-bug --
do
    SpriteImage("fg-bug",0.5,0.44)
    DefShModel("ac-bug", "fg-bug", "sh-whiteball-small")
end

-- ac-cannonball --
do
    DefAlias("ac-cannonball", "ac-blackball-jump")
end

-- ac-horse --
do
    SpriteImage("fg-horse",0.5,0.18)
    SpriteImage("sh-horse", 0.4)
    DefShModel("ac-horse", "fg-horse", "sh-horse")
end

-- ac-rotor --
do
    local fg = SpriteImages("fg-rotor",9)
    local bg = SpriteImages("sh-rotor", 9, 0.4)

    frames = {}
    for i=1,9 do
        DefShModel("ac-rotor"..i, fg[i], bg[i])
        table.insert(frames, "ac-rotor"..i)
    end
    DefAnim("ac-rotor", BuildFrames(frames, 30), true)
end

-- ac-top --
do
    local fg = SpriteImages("fg-top", 9, 0.5, 0.3)
    local bg = SpriteImage("sh-top", 0.4, 0.3)

    frames = {}
    for i=1,9 do
        DefShModel("ac-top"..i, fg[i], bg)
        table.insert(frames, "ac-top"..i)
    end
    DefAnim("ac-top", BuildFrames(frames, 25), true)
end

--------------------------------------------------------------------------------
--                                Floor models                                --
--------------------------------------------------------------------------------
Progress(15, "Loading floor models")

------------------------
-- single tile floors --
------------------------
do
    floorlist = {
        "fl-abyss",
        "fl-acblack",
        "fl-acwhite",
        "fl-black",
        "fl-bluegreen",
        "fl-darkgray",
        "fl-dummy",
        "fl-dunes",
        "fl-floor_001",
        "fl-ice",
        "fl-inverse",
        "fl-inverse2",
        "fl-light",
        "fl-lightgray",
        "fl-nomouse",
        "fl-normal",
        "fl-sand",
        "fl-stone",
        "fl-springboard",
        "fl-trigger",
        "fl-white",
    }

    DefImages(floorlist)
end

-----------------------------------------
-- Floors with multiple (random) tiles --
-----------------------------------------
do
    DefSubimages("fl-gradient2", {w=6, h=4, modelname="fl-gradient"})

    DefRandFloorSi("fl-bluegray", 4)
    DefRandFloorSi("fl-bluegreenx", 2)
    DefRandFloorSi("fl-brick", 3)
    DefRandFloorSi("fl-bumps", 2,2)
    DefRandFloorSi("fl-concrete", 4)
    DefRandFloorSi("fl-gravel", 4)
    DefRandFloorSi("fl-gray", 5)
    DefRandFloorSi("fl-hay", 4)
    DefRandFloorSi("fl-himalaya", 4)
    DefRandFloorSi("fl-marble", 4)
    DefRandFloorSi("fl-metal", 6)
    DefRandFloorSi("fl-mortar", 2, 2)
    DefRandFloorSi("fl-plank", 4)
    DefRandFloorSi("fl-red", 4)
    DefRandFloorSi("fl-rock", 2)
    DefRandFloorSi("fl-rough", 5)
    DefRandFloorSi("fl-rough-blue", 4)
    DefRandFloorSi("fl-rough-red", 4)
    DefRandFloorSi("fl-sahara", 4)
    DefRandFloorSi("fl-samba", 2)
    DefRandFloorSi("fl-space", 3,2)
    DefRandFloorSi("fl-stwood", 2)
    DefRandFloorSi("fl-swamp", 4)
    DefRandFloorSi("fl-tigris", 4)
    DefRandFloorSi("fl-water", 4)
    DefRandFloorSi("fl-woven", 5)
end

-- leaves --
do
    DefSubimages("fl-leaves", {w=10, h=2, modelname="fl-leavesx"})
    DefRandFloor("fl-leaves", {"fl-leavesx1", "fl-leavesx2", "fl-leavesx3", "fl-leavesx4"})
    DefRandFloor("fl-leavesb", {"fl-leavesx5", "fl-leavesx6", "fl-leavesx7", "fl-leavesx8"})
    DefAlias("fl-leavesc1", "fl-leavesx9")
    DefAlias("fl-leavesc2", "fl-leavesx10")
    DefAlias("fl-leavesc3", "fl-leavesx11")
    DefAlias("fl-leavesc4", "fl-leavesx12")
    DefAlias("fl-leavesd1", "fl-leavesx13")
    DefAlias("fl-leavesd2", "fl-leavesx14")
    DefAlias("fl-leavesd3", "fl-leavesx15")
    DefAlias("fl-leavesd4", "fl-leavesx16")
    DefAlias("fl-leavese1", "fl-leavesx17")
    DefAlias("fl-leavese2", "fl-leavesx18")
    DefAlias("fl-leavese3", "fl-leavesx19")
    DefAlias("fl-leavese4", "fl-leavesx20")
end

-- wooden floor --
do
    DefSubimages("fl-wood", {w=1, h=4, modelname="fl-woodx"})
    DefRandFloor("fl-wood", {"fl-woodx1", "fl-woodx2", "fl-woodx3", "fl-woodx4"})
    DefRandFloor("fl-wood1", {"fl-woodx3", "fl-woodx4"})
    DefRandFloor("fl-wood2", {"fl-woodx1", "fl-woodx2"})
end

--------------------------
-- Simple floor aliases --
--------------------------
do
    DefAlias("fl-abyss_fake", "fl-abyss")
    DefAlias("fl-normal_x", "fl-normal")
    DefAlias("fl-rough_medium", "fl-rough")
    DefAlias("fl-rough_slow", "fl-rough")
    DefAlias("fl-space-force", "fl-space")
end

----------------------
-- Floor animations --
----------------------

-- bridges --
do
    DefImage("fl-bridgea-open")
    DefImage("fl-bridgea-closed")
    local namelist=DefSubimages("fl-bridgea", {h=10})
    local frames = BuildFrames(namelist,70)
    DefAnim("fl-bridgea-opening", ReverseFrames(frames))
    DefAnim("fl-bridgea-closing", frames)
end

do
    DefImage("fl-bridgex-open")
    DefImage("fl-bridgex-closed")
    local namelist=DefSubimages("fl-bridgex", {h=9})
    local frames = BuildFrames(namelist,70)
    DefAnim("fl-bridgex-opening", frames)
    DefAnim("fl-bridgex-closing", ReverseFrames(frames))
end

do
    DefImage("fl-bridgey-open")
    DefImage("fl-bridgey-closed")
    local namelist=DefSubimages("fl-bridgey", {h=9})
    local frames = BuildFrames(namelist,70)
    DefAnim("fl-bridgey-opening", frames)
    DefAnim("fl-bridgey-closing", ReverseFrames(frames))
end

------------------------
-- Heating animations --
------------------------
do
    function heating_animation(basemodel)
        local images = DefSubimages(basemodel.."-heating", {h=8});
        local frames = BuildFrames(images, 240)
        DefAnim(basemodel.."-heating", frames);
    end

    heating_animation("fl-ice")
    heating_animation("fl-water")
    heating_animation("fl-swamp")
end

--------------------------------------------------------------------------------
--                                ITEM MODELS                                 --
--------------------------------------------------------------------------------
Progress(25, "Loading item models")
------------------------------------------------------
-- Single-Image-Items, non animated, e.g. it-banana --
------------------------------------------------------

do
    itemlist = {
        "it-bag",
        "it-banana",
        "it-blackbomb",
        "it-blocker",
        "it-booze",
        "it-booze-broken",
        "it-cherry",
        "it-cross",
        "it-document",
        "it-drop",
        "it-dynamite",
        "it-flagblack",
        "it-flagwhite",
        "it-hill",
        "it-hollow",
        "it-odometer",
        "it-pencil",
        "it-pin",
        "it-ring",
        "it-spade",
        "it-spoon",
        "it-squashed",
        "it-spring1",
        "it-spring2",
        "it-surprise",
        "it-tinyhill",
        "it-tinyhollow",
        "it-weight",
        "it-whitebomb"
    }

    DefImages(itemlist)

    DefImage("it-brake", {filename="st-brake"})
    DefImage("it_brush", {filename="it-brush"})
    DefImage("it_coin_s", {filename="it-coin1"})
    DefImage("it_coin_m", {filename="it-coin2"})
    DefImage("it_coin_l", {filename="it-coin4"})
    DefImage("it_cross", {filename="it-cross"})
    DefImage("it_extralife", {filename="it-extralife"})
    DefImage("it_floppy", {filename="it-floppy"})
    DefImage("it_glasses", {filename="it-glasses"})
    DefImage("it_glasses_broken", {filename="it-glasses-broken"})
    DefImage("it_hammer", {filename="it-hammer"})
    DefImage("it_key", {filename="it-key"})
    DefImage("it_landmine", {filename="it-landmine"})
    DefImage("it_magicwand", {filename="it-magicwand"})
    DefImage("it_rubberband", {filename="it-rubberband"})
    DefImage("it_sword", {filename="it-sword"})
    DefImage("it_umbrella", {filename="it-umbrella"})
    DefImage("it_wrench", {filename="it-wrench"})
end

-------------------------
-- Simple item aliases --
-------------------------

do
    DefAlias("it-key_a", "it-key")
    DefAlias("it-key_b", "it-key")
    DefAlias("it-key_c", "it-key")
    -- DefAlias("it-bridge-oxyd", "invisible")
    -- DefAlias("it-sensor", "invisible")
    -- DefAlias("it-inversesensor", "invisible")
end

--------------------------------------------------------------
-- Multiple-Image-Items, non animated, e.g. it-burnable-oil --
--------------------------------------------------------------

-- it-extinguisher --
do
    DefTiles("it-extinguisher", {"it-extinguisher", "it-extinguisher_medium", "it-extinguisher_empty"})
end

-- it-dummy
do
    DefTiles("it-dummy", {"it-dummy", "it-dummy_egg"})
end

-- Oil --
do
    DefSubimages("it-burnable_oil", {h=4})
end

-- Laserbeams --
do
    DefTiles("it-laser", {"it-laserh", "it-laserv", "it-laserhv"})
end

-- it-pipe --
do
    DefTiles("it-pipe", {"it_pipe_e", "it_pipe_s", "it_pipe_es", "it_pipe_sw", "it_pipe_ew",
                            "it_pipe_w", "it_pipe_n", "it_pipe_ne", "it_pipe_nw", "it_pipe_ns"})
end

-- it-trigger --
do
    DefTiles("it-trigger", {"it-trigger","it-trigger1"})
end

-- it-yinyang --
do
    DefTiles("it-yinyang", {"it-yinyang", "it-yanying"})
end

-- it_strip --
do
    DefSubimages("it_strip", {modelname="it_strip", w=4, h=4, startindex=0})
    DefAlias("it_strip", "it_strip0")
end

----------------------------------------
-- Animated Items and Item Animations --
----------------------------------------

-- Burning black bomb --
do
    local images = DefSubimages("it-blackbomb-burning", {h=9})
    local frames = BuildFrames(images, 100)
    DefAnim("it-blackbomb-burning", frames)
end

-- Burning white bomb --
do
    local images = DefSubimages("it-whitebomb-burning", {h=9})
    local frames = BuildFrames(images, 100)
    DefAnim("it-whitebomb-burning", frames)
end

-- Burning dynamite --
do
    local images = DefSubimages("it-dynamite-burning", {h=15})
    local frames = BuildFrames(images, 100)
    DefAnim("it-dynamite-burning", frames)
end

-- Burning Floor --
do
    local images = DefSubimages("it-burnable_ignite", {h=8})
    local frames = BuildFrames(images, 100)
    DefAnim("it-burnable_ignited", frames)

    local images = DefSubimages("it-burnable_burning", {h=8})
    local frames = BuildFrames(images, 100)
    DefAnim("it-burnable_burning", frames)

    -- Simple fire related items:
    DefImages({"it-burnable_ash", "it-burnable_fireproof"})
    DefAlias("it-burnable", "invisible")
end

-- it-coffee --
do
    local images = DefSubimages("it-coffee", {h=4})
    local frames = BuildFrames(images,150)
    DefAnim("it-coffee", frames, true)
end

-- Cracks --
do
    local images = DefSubimages("it-crack", {h=8})
    local frames = BuildFrames(images,50)
    DefAnim("it-debris", frames)

    local frames = BuildFrames({"it-crack4", "it-crack5"},120)
    DefAnim("it-crack_anim1", frames)

    local frames = BuildFrames({"it-crack6", "it-crack7", "it-crack8"},120)
    DefAnim("it-crack_anim2", frames)
end

-- it-death --
do
    local images = DefSubimages("it-death", {h=4})
    DefAlias("it_death", images[1])
    DefAnim("it_death_anim", BuildFrames(images, 100))
end

-- Explosion --
do
    DefAnimImages("expl", {{"expl", 50}})
    DefAlias("it-explosion1", "expl")
    DefAlias("it-explosion2", "expl")
    DefAlias("it-explosion3", "expl")
end

-- it-magnet --
do
    DefImage("it-magnet-off")
    local images = DefSubimages("it-magnet-on", {h=5})
    local frames = BuildFrames(images, 100)
    DefAnim("it-magnet-on", frames, 1)
end

-- it-puller --
do
    local images = {"it-puller-n", "it-puller-e", "it-puller-s", "it-puller-w"}
    DefTiles("it-puller", images)
    local frames = BuildFrames(images, 100)
    DefAnim("it-puller-active", RepeatAnim(frames, 4), false)
end

-- it-seed --
do
    local images = DefSubimages("it-seed", {h=5})
    DefAlias("it-seed", "it-seed1")
    DefAlias("it-seed_nowood", "it-seed1")
    DefAlias("it-seed_volcano", "it-seed1")
    local frames = {
        "it-seed1", "it-seed2", "it-seed1", "it-seed3", "it-seed1", "it-seed2",
        "it-seed1", "it-seed4", "it-seed5", "it-seed4", "it-seed1",
        "it-seed4", "it-seed5",
    }
    DefAnim("it-seed-growing", BuildFrames(frames, 120))
end

-- it-sensor --
do
    DefTiles("it_sensor", {"it_sensor","it_sensor1"})
    local frames = BuildFrames({"it_sensor","it_sensor1"}, 300)
    DefAnim("it_sensor_hit", RepeatAnim(frames, 3), false)
end

-- it-shogun --
do
    NewAnim("it_shogun_s", {img="it-shogun-small", h=3, speed=160, pingpong=1, loop=1})
    NewAnim("it_shogun_m", {img="it-shogun-med",   h=3, speed=160, pingpong=1, loop=1})
    NewAnim("it_shogun_l", {img="it-shogun-big",   h=3, speed=160, pingpong=1, loop=1})
end

-- it-springboard --
do
    local images = {"it-springboard1", "it-springboard2"}
    DefTiles ("it-springboard", images)
    DefAlias ("it-springboard", images[1])
    DefAnim("it-springboard_anim", BuildFrames(ReverseFrames(images),120))
end

-- it-vortex --
do
    local images = DefSubimages("it-vortex", {h=4})
    DefAlias("it-vortex-open", images[1])
    DefAlias("it-vortex-closed", images[4])

    DefAnim("it-vortex-opening", ReverseFrames(BuildFrames(images, 100)))
    DefAnim("it-vortex-closing", BuildFrames(images, 100))
end

-- it-wormhole --
do
    local images = DefSubimages("it-wormhole", {h=2})
    local frames = BuildFrames(images, 100)
    DefAnim("it-wormhole", frames, true)
    DefAlias("it-wormhole-off", "it-wormhole1")
end

-- it_trap --
do
    local images = DefSubimages("it_trap", {h=8})
    local frames = BuildFrames(images, 100)
    DefAnim("it_trap_breaking", frames)
    DefAlias("it_trap", "it_trap8")
end

--------------------------------------------------------------------------------
--                                STONE MODELS                                --
--------------------------------------------------------------------------------
Progress(40, "Loading stone models")

-------------------
-- Stone shadows --
-------------------
do
    DefImage("sh-solid")
    DefImage("sh-round")
    DefImage("sh-round2")
    DefImage("sh-grate1")
    DefImage("sh-grate2")
    DefImage("sh-grate3")
    DefImage("sh-glass")
    DefImage("sh-white4")
    DefImage("sh-puzzle1")
    DefImage("sh-brake")
    DefImage("sh-floating")
    -- Note: Window shadows are defined where needed.
end

-------------------
-- simple stones --
-------------------
do
    DefStone("st-beads", "sh-glass")
    DefStone("st-black1")
    DefStone("st-black2")
    DefStone("st-black3")
    DefStone("st-black4", "sh-white4")
    DefStone("st-blackballs")
    DefStone("st-block")
    DefStone("st-bluegray", "sh-round")
    DefStone("st-brake", "sh-brake")
    DefStone("st-brownie", "sh-round")
    DefStone("st-bug")
    DefStone("st-bumps")
    DefStone("st-camouflage")
    DefStone("st-dummy")
    DefImage("st-easymode")
    DefStone("st-flhay")
    DefStone("st-firebreak")
    DefStone("st-floppy0", "sh-round", {filename="st-floppy1"})
    DefStone("st-floppy1", "sh-round", {filename="st-floppy2"})
    DefStone("st-flrock")
    DefStone("st-glass", "sh-glass");
    DefStone("st-glass1", "sh-glass")
    DefStone("st-glass2", "sh-glass")
    DefStone("st-glass3", "sh-glass")
    DefStone("st-grate1", "sh-grate1")
    DefStone("st-grate2", "sh-grate2")
    DefStone("st-grate3", "sh-grate3")
    DefStone("st-greenbrown", "sh-round")
    DefStone("st-key0", "sh-round", {filename="st-key1"})
    DefStone("st-key1", "sh-round", {filename="st-key2"})
    DefStone("st-marble", "sh-round")
    DefStone("st-metal")
    DefStone("st-redrock")
    DefStone("st-rock1", "sh-round")
    DefStone("st-rock2")
    DefStone("st-rock3")
    DefStone("st-rock3_cracked")
    DefStone("st-rock4")
    DefStone("st-rock5")
    DefStone("st-rock6")
    DefStone("st-rock7")
    DefStone("st-rock8", "sh-round")
    DefStone("st-rock9", "sh-round")
    DefStone("st-rock10")
    DefStone("st-rubberband")
    DefStone("st-stone1")
    DefStone("st-stone2")
    DefStone("st-surprise")
    DefStone("st-white1")
    DefStone("st-white2")
    DefStone("st-white3")
    DefStone("st-white4", "sh-white4")
    DefStone("st-whiteballs")
    DefStone("st-woven")
    DefStone("st-yellow")
    DefStone("st-yinyang1")
    DefStone("st-yinyang2")
end

---------------------
-- Animated stones --
---------------------

-- st-actorimpulse --
do
    namelist = DefSubimages("st-actorimpulse", {h=3})
    shnamelist = DefSubimages("sh-actorimpulse", {h=3,imgw=ShadowSize,imgh=ShadowSize})

    DefAnim("st-ai-fg", PingPong(BuildFrames(namelist, 30)))
    DefAnim("st-ai-sh", PingPong(BuildFrames(shnamelist, 30)))
    DefShModel("st-actorimpulse-anim", "st-ai-fg", "st-ai-sh")

    DefShModel("st-actorimpulse", namelist[1], shnamelist[1])
end

-- st-bigbluesand --
do
    DefSubimages("st-bigbluesand", {modelname="st-bigbluesandx",w=4,h=4})
    for i=1,16 do DefSolidStone("st-bigbluesand"..i, "st-bigbluesandx"..i) end
    DefSolidStone("st-blue-sand", "st-bigbluesandx1")
end

-- st-bigbrick --
do
    DefSubimages("st-bigbrick", {modelname="st-bigbrickx",w=4,h=4})
    for i=1,16 do DefSolidStone("st-bigbrick"..i, "st-bigbrickx"..i) end
    DefSolidStone("st-brick", "st-bigbrickx1")
    DefSolidStone("st-brick_magic", "st-bigbrickx1")
end

-- st-bigpanel --
do
    DefSubimages("st-bigpanel", {modelname="st-bigpanelx",w=4,h=4})
    for i=1,16 do DefSolidStone("st-bigpanel"..i, "st-bigpanelx"..i) end
    DefSolidStone("st-panel", "st-bigpanelx1")
    -- st-wood_001. Remove when it's time!
    DefSolidStone("st-wood_001", "st-bigpanelx1")
end

-- st_bluesand --
do
    DefSubimages("st-bigbluesand", {modelname="st_bluesandx",w=4,h=4})
    for i=0,15 do DefSolidStone("st_bluesand"..i, "st_bluesandx"..(i+1)) end
    DefSolidStone("st_bluesand", "st_bluesandx1")
end

-- st_brick --
do
    DefSubimages("st-bigbrick", {modelname="st_brickx",w=4,h=4})
    for i=0,15 do DefSolidStone("st_brick"..i, "st_brickx"..(i+1)) end
    DefSolidStone("st_brick", "st_brickx1")
end

-- st_panel --
do
    DefSubimages("st-bigpanel", {modelname="st_panelx",w=4,h=4})
    for i=0,15 do DefSolidStone("st_panel"..i, "st_panelx"..(i+1)) end
    DefSolidStone("st_panel", "st_panelx1")
end

-- st-bolder --
do
    local img=DefSubimages("st-bolder", {w=4,h=3})
    local imgfall=DefSubimages("st-bolder-fall", {w=4,h=3})

    function def_bolder(orient, start)
        local animname="st-bolder"..orient.."a"
        local frames={img[start], img[start+1], img[start+2]}
        DefAnim(animname, BuildFrames(frames, 120), false)
        DefShModel("st-bolder-"..orient, animname, "sh-round")

        animname="st-bolder-"..orient.."-fall-anim"
        frames={imgfall[start],imgfall[start+1],imgfall[start+2]}
        DefAnim(animname, BuildFrames(frames, 120), false)
    end

    def_bolder("n",1)
    def_bolder("e",4)
    def_bolder("s",7)
    def_bolder("w",10)
end

-- st-break_*, breakable stones --
do
    DefSolidStoneWithAnim ("st-stone_break", 10, 50)
    DefSolidStoneWithAnim ("st-break_bolder", 10, 50)
    DefRoundStoneWithAnim ("st-break_acwhite", 10, 50)
    DefRoundStoneWithAnim ("st-break_acblack", 10, 50)
    DefRoundStoneWithAnim ("st-break_gray", 10, 50)
end

-- st-chage-plus, st-charge-zero, st-charge-minus --
do
    function def_chargestone(basename)
        local n=DefSubimages(basename, {h=2})
        DefSolidStone(basename, n[1])
        DefAnim(basename.."-animfg", {{n[2], 140}})
        DefSolidStone(basename.."-anim", basename.."-animfg")
    end

    def_chargestone("st-chargeplus")
    def_chargestone("st-chargeminus")
    def_chargestone("st-chargezero")
end

-- st-chess_black, st-chess_white --
do
    function make_chess(colour)
        local img1 = DefSubimages("st-chess"..colour, {h=5});
        --local img2 = DefSubimages("st-chess"..colour.."-captured", {h=5});
        local f1 = BuildFrames(img1, 120)
        --local f2 = BuildFrames(img2, 40)
        local f2 = f1
        local f3 = BuildFrames(img1, 500)
        DefAnim("st_chess"..colour.."-disappearing", f1);
        DefAnim("st_chess"..colour.."-appearing", ReverseFrames(f1))
        DefAnim("st_chess"..colour.."-captured", f2);
        DefAnim("st_chess"..colour.."-swamp", f3);
        DefRoundStone("st_chess"..colour, img1[1])
    end

    make_chess("_black")
    make_chess("_white")
end

-- st-coinslot --
do
    DefStone("st-coinslot")
    local images = DefSubimages("st-coin2slot", {h=18})
    DefAnim("st-coin2slot-anim", BuildFrames(images, 20))
    DefSolidStone("st-coin2slot", "st-coin2slot-anim")
    DefSolidStone("st-coinslot-active", "st-coin2slot18")
end

-- st-disco --
do
    DefTiles("st-disco", {"st-disco-light","st-disco-medium","st-disco-dark"})
end

-- st-death --
do
    DefRoundStoneWithAnim ("st-death", 3, 140)
end

-- st-dynamite, st-bombs, st-whitebombs --
do
    function make_bombstone(name)
        local images = DefSubimages(name, {h=7})
        DefRoundStone(name, name.."1")
        DefAnim(name.."-anim", BuildFrames(images,50))
    end

    make_bombstone("st-bombs")
    make_bombstone("st-dynamite")
    make_bombstone("st-whitebombs")
end

-- st-flash --
do
    local images = DefSubimages("st-flash", {h=1})
    DefSolidStone("st-flash", "st-flash1")
end

-- st-fourswitch --
do
   DefSubimages("st-fourswitch",{h=4})
   DefShModel("st-fourswitch-n", "st-fourswitch1", "sh-round")
   DefShModel("st-fourswitch-e", "st-fourswitch2", "sh-round")
   DefShModel("st-fourswitch-s", "st-fourswitch3", "sh-round")
   DefShModel("st-fourswitch-w", "st-fourswitch4", "sh-round")
end

-- st-knight --
do
    local images = DefSubimages("st-knight", {modelname="knight", h=5})
    for i=1, table.getn(images) do
        DefSolidStone("st-knight"..i, images[i])
    end
end

-- st-laser --
do
    function make_laser(dir)
        laseron=FrameNames("st-laser"..dir, 1, 9)

        -- deactivated laser
        DefOverlay("laser"..dir, {"st-laser-base", laseron[1]})
        DefRoundStone("st-laser"..dir, "laser"..dir)

        -- activated laser
        names = {}
        for i=1,table.getn(laseron) do
                names[i] = "st-laseron"..dir .. format("_%04d", i)
                DefOverlay (names[i], {"st-laser-base", laseron[i]})
        end
        frames = BuildFrames(names, 100)
        DefAnim("st-laseron-anim"..dir, frames, 1)
        DefRoundStone("st-laseron"..dir, "st-laseron-anim"..dir)
    end

    make_laser("-e")
    make_laser("-s")
    make_laser("-w")
    make_laser("-n")
end

-- laserswitch --
do
    DefStone("st-laserswitch0", nil, {filename="st-oxydb"})
    DefAnimImages("laserswitch-blink",
        RepeatAnim(PingPong(BuildFrames(FrameNames("st-fakeoxyd-blink", 1,4),50))), {loop=1})
    DefSolidStone("st-laserswitch1", "laserswitch-blink")
end

-- st-lightpassenger --
do
    local img = DefSubimages("st-lightpassenger", {h=7})
    DefShModel("st-lightpassenger", img[1], "sh-glass")
    DefImage("st-lightpassenger_offx", {filename = "st-lightpassenger_off"})
    DefShModel("st-lightpassenger_off", "st-lightpassenger_offx", "sh-glass")
    DefAlias("st-lightpassenger_hidden", "st-glass2")
    -- Blink animation
    local frames = {img[2], img[3], img[4], img[5], img[6], img[7]}
    DefAnim("st-lightpassenger-blink1", PingPong(BuildFrames(frames, 75)), true)
    DefShModel("st-lightpassenger-blink", "st-lightpassenger-blink1", "sh-glass")
    -- Break animation
    img = DefSubimages("st-lightpassenger-break", {h=7})
    local anim_v, anim_h, anim_hv = {}, {}, {}
    for j = 1, 7 do
      anim_v[j] = "st-lightpassenger-break-v"..j
      display.DefineComposite(anim_v[j], "it-laserv", img[j])
      anim_h[j] = "st-lightpassenger-break-h"..j
      display.DefineComposite(anim_h[j], "it-laserh", img[j])
      anim_hv[j] = "st-lightpassenger-break-hv"..j
      display.DefineComposite(anim_hv[j], "it-laserhv", img[j])
    end
    DefAnim("st-lightpassenger-break-v", BuildFrames(anim_v, 50), false)
    DefAnim("st-lightpassenger-break-h", BuildFrames(anim_h, 50), false)
    DefAnim("st-lightpassenger-break-hv", BuildFrames(anim_hv, 50), false)
end

-- st-magic :-) --
do
    local img = DefSubimages("st-magic", {h=4, modelname="st-magic-fg"})
    local nlist = {}
    for i=1, table.getn(img) do
        nlist[i] = "st-magic"..i
        DefRoundStone(nlist[i], img[i])
    end
    display.DefineRandModel("st-magic", table.getn(nlist), nlist)
end

-- st-mail --
do
    local images = DefSubimages("st-mail", {h=4})
    DefSolidStone("st-mail-n", images[1])
    DefSolidStone("st-mail-e", images[2])
    DefSolidStone("st-mail-w", images[3])
    DefSolidStone("st-mail-s", images[4])
end

-- st-oneway, st-oneway_black, st-oneway_white --
do
    local model_names = {
        "st-oneway-e", "st-oneway-n", "st-oneway-s", "st-oneway-w",
        "st-oneway_black-e", "st-oneway_black-n", "st-oneway_black-s", "st-oneway_black-w",
        "st-oneway_white-e", "st-oneway_white-n", "st-oneway_white-s", "st-oneway_white-w"
    }
    DefSubimages("st-oneway", {modelname="st-onewayx",w=3,h=4})
    for i=1,12 do DefFloatingStone(model_names[i], "st-onewayx"..i) end
end

-- st-pull, st-swap --
do
   DefSubimages("st-pull",{modelname="st-pull-fg",h=5})
   DefSubimages("st-swap",{modelname="st-swap-fg",h=5})
   DefSubimages("sh-pull",{h=4,imgw=ShadowSize,imgh=ShadowSize})

   DefShModel("st-pull",   "st-pull-fg1", "sh-glass")
   DefShModel("st-pull-n", "st-pull-fg2", "sh-pull1")
   DefShModel("st-pull-s", "st-pull-fg3", "sh-pull2")
   DefShModel("st-pull-e", "st-pull-fg4", "sh-pull3")
   DefShModel("st-pull-w", "st-pull-fg5", "sh-pull4")

   DefShModel("st-swap",   "st-swap-fg1", "sh-round")
   DefShModel("st-swap-n", "st-swap-fg2", "sh-pull1")
   DefShModel("st-swap-s", "st-swap-fg3", "sh-pull2")
   DefShModel("st-swap-e", "st-swap-fg4", "sh-pull3")
   DefShModel("st-swap-w", "st-swap-fg5", "sh-pull4")
end

-- st-puzzle* --
do
    DefSubimages("st-puzzle", {modelname="st-puzzlex",w=8,h=4})
    for i=2,16  do DefSolidStone("st-puzzle"..i, "st-puzzlex"..i) end
    for i=18,32 do DefSolidStone("st-puzzle"..i, "st-puzzlex"..i) end
    DefShModel("st-puzzle1", "st-puzzlex1", "sh-puzzle1")
    DefShModel("st-puzzle17", "st-puzzlex17", "sh-puzzle1")
end

-- st-rock3-break --
do
    local images = DefSubimages("st-rock3-break", {h=6})
    DefAnim("rock3_break-anim",BuildFrames(images,50))
    DefSolidStone("st-rock3_break-anim", "rock3_break-anim")
    DefAlias("st-rock3_movebreak", "st-rock3")
end

-- st-rock3-falling --
do
    local images = DefSubimages("st-rock3-falling", {h=4})
    DefAnim("st-rock3-falling", BuildFrames(images, 100))
end

-- st-rotator --
do
    local images = DefSubimages("st-rotator-left", {h=8})
    DefAnim("st-rotator-left-anim", BuildFrames(images, 70), false)
    DefRoundStone("st-rotator-left", "st-rotator-left-anim")
    local images = DefSubimages("st-rotator-right", {h=8})
    DefAnim("st-rotator-right-anim", BuildFrames(images, 70), false)
    DefRoundStone("st-rotator-right", "st-rotator-right-anim")
end

-- st-scissors --
do
    DefStone("st-scissors")
    local images = DefSubimages("st-scissors-snip", {h=1})
    DefAnim("st-scissors-snip-anim", BuildFrames(images, 130))
    DefSolidStone("st-scissors-snip", "st-scissors-snip-anim")
end

-- st-shogun* --
do
    DefImages{"sh-shogun1","sh-shogun2", "sh-shogun4"}
    DefSubimages("st-shogun", {modelname="st-shogun-fg",h=7})

    DefShModel("st-shogun1", "st-shogun-fg1", "sh-shogun1")
    DefShModel("st-shogun2", "st-shogun-fg2", "sh-shogun2")
    DefShModel("st-shogun3", "st-shogun-fg3", "sh-shogun1")
    DefShModel("st-shogun4", "st-shogun-fg4", "sh-shogun4")
    DefShModel("st-shogun5", "st-shogun-fg5", "sh-shogun1")
    DefShModel("st-shogun6", "st-shogun-fg6", "sh-shogun2")
    DefShModel("st-shogun7", "st-shogun-fg7", "sh-shogun1")
end

-- st-stoneimpulse --
--
-- Note: It's important that the duration of the closing animation
-- (anim2) is longer than the opening animation (anim1). Otherwise
-- impulse stones do not work properly!
do
   local images = DefSubimages("st-stoneimpulse", {h=4})
   DefRoundStone("st-stoneimpulse", images[1])
   local frames = {}
   for i=1,4 do table.insert(frames, images[i]) end
   DefAnim("stoneimpulse-anim1", BuildFrames(frames, 55))
   DefRoundStone("st-stoneimpulse-anim1", "stoneimpulse-anim1")
   table.insert(frames, images[4]) -- add 1 frame to make closing anim longer!
   DefAnim("stoneimpulse-anim2", ReverseFrames(BuildFrames(frames, 55)))
   DefRoundStone("st-stoneimpulse-anim2", "stoneimpulse-anim2")
end

-- st-stoneimpulse-hollow --
do
   local images = DefSubimages("st-stoneimpulse-hollow", {h=4})
   DefShModel("st-stoneimpulse-hollow", images[1], "sh-floating")
   local frames = {}
   for i=1,4 do table.insert(frames, images[i]) end
   DefAnim("stoneimpulse-hollow-anim1", BuildFrames(frames, 55))
   DefShModel("st-stoneimpulse-hollow-anim1", "stoneimpulse-hollow-anim1", "sh-floating")
   table.insert(frames, images[4]) -- add 1 frame to make closing anim longer!
   DefAnim("stoneimpulse-hollow-anim2", ReverseFrames(BuildFrames(frames, 55)))
   DefShModel("st-stoneimpulse-hollow-anim2", "stoneimpulse-hollow-anim2", "sh-floating")
end

-- st-switch, st-switch_black, st-switch_white --
do
    function mkswitch(modelname, basename)
        local n = DefSubimages(modelname, {h=3, modelname=basename.."-fg"})
        local f = BuildFrames(n, 60)
        DefRoundStone(modelname.."-off", n[1])
        DefRoundStone(modelname.."-on", n[3])
        DefAnim(basename.."-turnon", f)
            DefAnim(basename.."-turnoff", ReverseFrames(f))
            DefRoundStone(modelname.."-turnon", basename.."-turnon")
            DefRoundStone(modelname.."-turnoff", basename.."-turnoff")
    end
    mkswitch("st-switch", "switch")
    mkswitch("st-switch_black", "switch_black")
    mkswitch("st-switch_white", "switch_white")
end

-- st-timer --
do
    local images = DefSubimages("st-timer", {h=4})
    DefAnim("timer-anim", BuildFrames(images, 120), 1)
    DefRoundStone("st-timer", "timer-anim")
    DefRoundStone("st-timeroff", "st-timer1")
end

-- st-timeswitch --
do
    DefStone("st-timeswitch")
    local images = DefSubimages("st-time1switch", {h=2})
    DefAnim("st-time1switch-anim", BuildFrames (images, 180), 1)
    DefSolidStone("st-time1switch", "st-time1switch-anim")
end

-- st-turnstile --
do
    local images = DefSubimages("st-turnstile", {h=2})
    DefAnim("turnstile-anim", ReverseFrames(BuildFrames(images, 30)))
    DefSolidStone("st-turnstile", images[1])
    DefSolidStone("st-turnstile-anim", "turnstile-anim")
end

-- st-turnstle, arms --
DefAlias("st-turnstile-e", "st-puzzle2")
DefAlias("st-turnstile-w", "st-puzzle5")
DefAlias("st-turnstile-s", "st-puzzle9")
DefAlias("st-turnstile-n", "st-puzzle3")

-- st-turnstile-green --
do
    local images = DefSubimages("st-turnstile-green", {h=2})
    DefAnim("turnstile-green-anim", ReverseFrames(BuildFrames(images, 30)))
    DefSolidStone("st-turnstile-green", images[1])
    DefSolidStone("st-turnstile-green-anim", "turnstile-green-anim")
end

-- st-wood*, growing stones, st-volcano, st-blocker --
do
   local sh=DefSubimages("sh-round2-growing", {h=3,imgw=ShadowSize,imgh=ShadowSize})

   -- Wooden stones --
   do
      DefSubimages("st-wood", {modelname="st-wood-fg",h=2})
      DefShModel("st-wood1", "st-wood-fg1", "sh-round")
      DefShModel("st-wood2", "st-wood-fg2", "sh-round")
      local n=DefSubimages("st-wood-growing", {h=3})
      DefAnim("wood-growing-fg", BuildFrames(n, 130))
      DefAnim("wood-growing-bg", BuildFrames(sh, 130))
      DefShModel("st-wood-growing", "wood-growing-fg", "wood-growing-bg")
      DefShModel("st-greenbrown-growing", "wood-growing-fg", "wood-growing-bg")
   end

   -- Blocker stone --
   do
      local n=DefSubimages("st-blocker", {h=4})
      DefRoundStone("st-blocker", n[1])
      frames={}
      for i=4,2,-1 do
         table.insert(frames, "st-blocker"..i)
      end
      DefAnim("blocker-growing-fg", BuildFrames(frames, 60))
      DefAnim("blocker-growing-bg", BuildFrames(sh, 60))
      DefAnim("blocker-shrinking-fg", ReverseFrames(BuildFrames(frames, 60)))
      DefAnim("blocker-shrinking-bg", ReverseFrames(BuildFrames(sh, 60)))
      DefShModel("st-blocker-shrinking", "blocker-shrinking-fg", "blocker-shrinking-bg");
      DefShModel("st-blocker-growing", "blocker-growing-fg", "blocker-growing-bg");
   end
end

do
    DefAlias("st-volcano-growing", "st-blocker-growing")
end

-- st-window --
do
    local fg_window = {blue = DefSubimages("st_window_blue", {modelname="fg-window_blue",w=4,h=4}),
                       green = DefSubimages("st_window_green", {modelname="fg-window_green",w=4,h=4}) }
    local sh_window = DefSubimages("sh_window", {w=2,h=4,imgw=ShadowSize,imgh=ShadowSize})
    local breaking_images = {blue = DefSubimages("st_window_blue_break", {h=4}),
                             green = DefSubimages("st_window_green_break", {h=4})}

    local function name(combination, separator)
      return combination.normal_faces..separator..combination.scratched_faces
    end

    -- The window images are constructed in two steps.
    -- 1) All possible combinations of the shadows are constructed
    --    (for each direction: "no shadow", "normal face", "scratched face",
    --    represented as "0, 1, 2" in a table).
    -- 2) Using the table of all possible combinations, all images
    --    and animations of the window stones are constructed.
    -- Step 1 works as follows: We start with direction 1 (West),
    -- and all three possibilities for shadows to the West. At this
    -- point, the images for shadows to West only are build.
    -- As next, for each of the three West-possibilities, we
    -- construct the South-possibilities, i.e. we now have 3*3 combinations
    -- and 3*3-1 shadow-images. We continue with East and North.
    -- The table "all_combinations" is after each pass of the "direction"-loop:
    --  0) {{}}
    --  1) {{0}, {1}, {2}}
    --  2) {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {2,0}, {2,1}, {2,2}}
    --  3) {{0,0,0}, {0,0,1}, {0,0,2}, {0,1,0}, {0,1,1}, ... }
    --  4) {{0,0,0,0}, {0,0,0,1}, {0,0,0,2}, {0,0,1,0}, ... }
    -- Plus extra entries to count the number of normal_faces and
    -- scratched_faces (to be exactly, the numbers binarily represent
    -- the existence of the face).
    local all_combinations = {{normal_faces = 0, scratched_faces = 0}}
    DefAlias("sh_window0-0", "invisible")
    local base = {0, 1, 2}
    for direction = 1, 4 do
        local next_list = {}
        for _, old_combination in pairs(all_combinations) do
            for _, new_digit in pairs(base) do
                -- Deep-copy old_combination to new_combination.
                local new_combination = {}
                for k, digit in pairs(old_combination) do
                    new_combination[k] = digit
                end
                -- Add a new digit to new_combination and save it.
                new_combination[direction] = new_digit
                table.insert(next_list, new_combination)
                -- If this combination adds a new face, create
                -- the corresponding composite image, based on the
                -- image of old_combination.
                if new_digit ~= 0 then
                    -- First, increase the "number" of faces.
                    if new_digit == 1 then
                        new_combination.normal_faces = new_combination.normal_faces
                                + ({1,2,4,8})[direction]
                    elseif new_digit == 2 then
                        new_combination.scratched_faces = new_combination.scratched_faces
                                + ({1,2,4,8})[direction]
                    end
                    -- Now compose the shadow image.
                    display.DefineComposite(
                            "sh_window"..name(new_combination, "-"),
                            "sh_window"..name(old_combination, "-"),
                            "sh_window"..(direction + ({0,4})[new_digit]))
                end
            end
        end
        all_combinations = next_list
    end

    for _, combination in pairs(all_combinations) do
        -- Abbreviations. Note that sometimes "-" and sometimes "_" is
        -- used as separator. Don't expect a pattern.
        local total_faces = combination.normal_faces + combination.scratched_faces
        local name1 = name(combination, "-")
        local name2 = name(combination, "_")
        for _, color in pairs({"blue", "green"}) do
            -- Normal stone (= not breaking)
            DefShModel("st_window_"..color..name2,
                       "fg-window_"..color..(total_faces + 1),
                       "sh_window"..name1)
            -- Breaking animation
            local breaking_window_names = {}
            for j = 1, table.getn(breaking_images[color]) do
                breaking_window_names[j] =
                          "st-window_"..color.."_breaking"..name1.."-"..j
                display.DefineComposite(breaking_window_names[j],
                          fg_window[color][total_faces + 1],
                          breaking_images[color][j])
            end
            local frames = BuildFrames(breaking_window_names, 130)
            DefAnim("st-window_"..color..name2.."_anim_fg", frames)
            DefShModel("st_window_"..color..name2.."_anim",
                       "st-window_"..color..name2.."_anim_fg",
                       "sh_window"..name1);
        end
    end
end

-----------------
-- Oxyd Stones --
-----------------
--
-- Oxyd stones occur in different colors and different flavors and
-- for each combination of these two attributes we need animations
-- for the following internal states or state transitions:
--
--   (a) opening and closing
--   (b) blinking
--   (c) opened
--
-- Creating the required image files by hand would be tedious, so
-- we use a couple of specialized functions and overlays to create
-- them automatically.

do
    local num_colors = 12
    local colorspots = FrameNames("st-oxyd-color", 1, num_colors)
    AddFrameNames(colorspots, "st-oxyd-color", 96, 97)
    local blink_ovls = FrameNames("st-oxyd-blink", 1, 5)
    local pseudospots = {}
    pseudospots[-3] = FrameNames("st-oxyd-pseudo-3", 1, 4)
    pseudospots[-4] = FrameNames("st-oxyd-pseudo-4", 1, 8)

-- Define "fading in" and "fading out" animations for oxyd stones.
-- These two animations are combined with the stone images to
-- produce the opening and closing animations for oxyd stones.
    local baseimg = {
        a="st-oxyda-open",
        b="st-oxydb-open",
        c="st-oxydc-open",
        d="st-oxydd-open"
    }
    local shadow = {
       a="sh-round",
       b="sh-round",
       c="sh-solid",
       d="sh-solid",
    }
    local fopening = {
        a = BuildFrames(DefSubimages("st-oxyda-opening", {h=9}), 60),
        b = BuildFrames(DefSubimages("st-oxydb-opening", {h=14}), 40),
        c = BuildFrames(DefSubimages("st-oxydc-opening", {h=5}), 70),
        d = BuildFrames(DefSubimages("st-oxydd-opening", {h=5}), 70),
    }
    local fclosing = {
        a = ReverseFrames(fopening["a"]),
        b = ReverseFrames(fopening["b"]),
        c = ReverseFrames(fopening["c"]),
        d = ReverseFrames(fopening["d"]),
    }

    function mkopenclose(flavor, color)
        local n = "st-oxyd" .. flavor .. color
        local fadein = "oxyd"..flavor.."-fadein"
        local fadeout= "oxyd"..flavor.."-fadeout"
        local spotcolor = color
        
        if (color >= 0) then
            spotcolor = color + 1 -- oxyd color 0..num_colors-1, file 1.., frames 1..
        else
            spotcolor = 100 + color -- pseudo colors
        end

        DefOverlay(n.."-base", {baseimg[flavor], colorspots[spotcolor]})
        display.DefineComposite(n.."-opening-fg", n.."-base", fadein)
        display.DefineComposite(n.."-closing-fg", n.."-base", fadeout)
        DefShModel (n.."-opening", n.."-opening-fg", shadow[flavor])
        DefShModel (n.."-closing", n.."-closing-fg", shadow[flavor])
    end

    function mkblink(flavor, color)
        local n = "st-oxyd"..flavor..color.."-blink"
        local img={baseimg[flavor],colorspots[color+1], "st-oxyd-questmark"}
        DefOverlay(n..1, img)
        DefOverlay(n..2, {baseimg[flavor], colorspots[color+1]})
        DefAnim(n.."-anim", BuildFrames({n..1,n..2}, 500), 1)
        DefShModel(n, n.."-anim", shadow[flavor])
    end

    function mkopened(flavor, color)
        local n = "st-oxyd" .. flavor .. color .. "-open"
        local names = {}

        for i=1, table.getn(blink_ovls) do
            local images={baseimg[flavor],colorspots[color+1],blink_ovls[i]}
            names[i] = n .. format("_%04d", i)
            DefOverlay(names[i], images)
        end

        -- compose these images into an animation
        frames = PingPong(BuildFrames(names, 100))
        DefAnim(n.."-anim", frames, true)

        -- and finally add a shadow to make the model complete
        DefShModel(n, n.."-anim", shadow[flavor])
    end

    function mkpseudo(flavor, color)
        local n = "st-oxyd" .. flavor .. "-pseudo" .. color
        local names = {}

        for i=1, table.getn(pseudospots[color]) do
            local images={baseimg[flavor],pseudospots[color][i]}
            names[i] = n .. format("_%04d", i)
            DefOverlay(names[i], images)
        end

        -- compose these images into an animation
        if (color == -3) then
            frames = RepeatAnim(PingPong(BuildFrames(names, 50)),2)
        elseif (color == -4) then
            frames = RepeatAnim(BuildFrames(names, 100),2)
        end
        DefAnim(n.."-anim", frames, false)

        -- and finally add a shadow to make the model complete
        DefShModel(n, n.."-anim", shadow[flavor])
    end

    function mkoxyd(flavor)
        DefStone("st-oxyd"..flavor, shadow[flavor])
        DefShModel("st-likeoxyd"..flavor, "st-oxyd"..flavor, shadow[flavor])
--        DefSolidStone("st-likeoxyd"..flavor, "st-oxyd"..flavor)
        img = DefImage("st-oxyd"..flavor.."-open")
        DefShModel("st-likeoxyd"..flavor.."-open", img, shadow[flavor])

        local fadein = "oxyd"..flavor.."-fadein"
        local fadeout= "oxyd"..flavor.."-fadeout"
        DefAnim(fadein, fopening[flavor])
        DefAnim(fadeout, fclosing[flavor])

        for color = 0, num_colors - 1 do
            mkopenclose(flavor, color)
            mkblink(flavor, color)
            mkopened(flavor, color)
        end
        mkopenclose(flavor, -3)
        mkpseudo(flavor, -3)
        mkopenclose(flavor, -4)
        mkpseudo(flavor, -4)
    end
    mkoxyd("a")
    mkoxyd("b")
    mkoxyd("c")
    mkoxyd("d")
end

-- st-coffee --
do
    DefAlias("st-coffee", "st-oxydc")
end

-- st-doorh, st-doorv --
do
    local f,img,sh

    img=DefSubimages("st-doorh", {h=7})
    sh=DefSubimages("sh-doorh", {h=7,imgw=ShadowSize,imgh=ShadowSize})
    DefShModel("st-doorh-open", img[1], sh[1])
    DefShModel("st-doorh-closed", img[7], sh[7])
    DefAnim("doorh-opening-fg", ReverseFrames(BuildFrames(img, 60)))
    DefAnim("doorh-opening-bg", ReverseFrames(BuildFrames(sh, 60)))
    DefShModel("st-doorh-opening", "doorh-opening-fg", "doorh-opening-bg")
    DefAnim("doorh-closing-fg", BuildFrames(img, 60))
    DefAnim("doorh-closing-bg", BuildFrames(sh, 60))
    DefShModel("st-doorh-closing", "doorh-closing-fg", "doorh-closing-bg")

    img=DefSubimages("st-doorv", {w=7})
    sh=DefSubimages("sh-doorv", {w=7,imgw=ShadowSize,imgh=ShadowSize})
    DefShModel("st-doorv-open", img[1], sh[1])
    DefShModel("st-doorv-closed", img[7], sh[7])
    DefAnim("doorv-opening-fg", ReverseFrames(BuildFrames(img, 60)))
    DefAnim("doorv-opening-bg", ReverseFrames(BuildFrames(sh, 60)))
    DefShModel("st-doorv-opening", "doorv-opening-fg", "doorv-opening-bg")
    DefAnim("doorv-closing-fg", BuildFrames(img, 60))
    DefAnim("doorv-closing-bg", BuildFrames(sh, 60))
    DefShModel("st-doorv-closing", "doorv-closing-fg", "doorv-closing-bg")
end

-- st-door_a --
do
    DefAlias("st-door_a-open", "st-grate1")
    DefAlias("st-door_a-closed", "st-oxyda")
    local f = BuildFrames({"st-door_a-closed", "st-door_a-open"},60)
    DefAnim("st-door_a-opening", f)
    DefAnim("st-door_a-closing", ReverseFrames(f))
end

-- st-door_b --
do
    DefAlias("st-door_b-open", "invisible")
    DefAlias("st-door_b-closed", "st-rock3")
    local img=DefSubimages("st-doorb", {modelname="doorb", h=8})
    local f = BuildFrames(img,60)
    DefAnim("st-door_b-opening", f)
    DefAnim("st-door_b-closing", ReverseFrames(f))
end

-- st-door_c --
do
    DefAlias("st-door_c-open", "st-grate2")
    DefAlias("st-door_c-closed", "st-rock3")
    frames=BuildFrames({"st-door_c-closed","st-door_c-open"},60)
    DefAnim("st-door_c-opening", frames)
    DefAnim("st-door_c-closing", ReverseFrames(frames))
end

-- st-fakeoxyd --
do
    DefStone("st-fakeoxyd", "sh-round", {filename="st-oxydb"})
    DefAnimImages("fakeoxyd-blink",
        RepeatAnim(PingPong(BuildFrames(FrameNames("st-fakeoxyd-blink", 1,4),50))))
    DefRoundStone("st-fakeoxyd-blink", "fakeoxyd-blink")

    DefAlias("st-fakeoxyda", "st-oxyda")
end

-- st-fart --
do
    DefStone("st-fart", "sh-round", {filename="st-oxydb"})
    DefAnimImages("farting",
        RepeatAnim(PingPong(BuildFrames(FrameNames("st-fakeoxyd-blink", 1,4),50))))
    DefRoundStone("st-farting", "farting")

    namelist = DefSubimages("st-fart-break",{h=6})
    DefAnim("fartbreak-anim", BuildFrames(namelist,50))
    DefRoundStone("st-fartbreak-anim", "fartbreak-anim")
end

-- st-spitter --
do
    DefAlias ("st-spitter", "st-timeswitch")
    DefAlias ("st-spitter-loading", "st-fakeoxyd-blink")
    DefAlias ("st-spitter-spitting", "st-fakeoxyd-blink")
end

---------------------------
-- Thief stone and floor --
---------------------------
do
    -- Base images

    local stonebase = "st-bluegray"
    local floorbase = "fl-bluegray"
    local thiefovl = DefSubimages("thief-template", {h = 7})
    local capturedovl = DefSubimages("thief-captured-template", {h = 12})

    -- Creating st-thief

    local names = {}
    for j = 1, table.getn(thiefovl) do
        names[j] = "st-thief"..format("_%04d", j)
        display.DefineComposite(names[j], stonebase, thiefovl[j])
    end

    local f1 = BuildFrames(names, 80)
    DefAnim("pre-st-thief-emerge", f1)
    DefAnim("pre-st-thief-retreat", ReverseFrames(f1))
    DefRoundStone("st-thief", stonebase)
    DefRoundStone("st-thief-emerge", "pre-st-thief-emerge")
    DefRoundStone("st-thief-retreat", "pre-st-thief-retreat")

    -- Creating st-thief-captured
    --
    --   Note that this is done without template, as the whole
    --   stone has to disappear (e.g. via shrinking)

    local img2 = DefSubimages("st-thief-captured", {h = 12})
    local f2 = BuildFrames(img2, 80)
    DefAnim("st-thief-captured", f2)

    -- Creating fl-thief

    local floornames = {}
    local floorcaptured = {}
    local floorbases = DefSubimages(floorbase, {h=4, modelname="fl-thief-base"})
    for k = 1,4 do
      floornames[k] = {}
      for j = 1, table.getn(thiefovl) do
        floornames[k][j] = "fl-thief"..k..format("_%04d", j)
        display.DefineComposite(floornames[k][j], floorbases[k], thiefovl[j])
      end
      floorcaptured[k] = {}
      for j = 1, table.getn(capturedovl) do
        floorcaptured[k][j] = "fl-thief"..k.."-captured"..format("_%04d", j)
        display.DefineComposite(floorcaptured[k][j], floorbases[k], capturedovl[j])
      end
      local f3 = BuildFrames(floornames[k], 80)
      local f4 = BuildFrames(floorcaptured[k], 80)
      DefAlias("fl-thief"..k, floorbases[k])
      DefAnim("fl-thief"..k.."-emerge", f3)
      DefAnim("fl-thief"..k.."-retreat", ReverseFrames(f3))
      DefAnim("fl-thief"..k.."-captured", f4)
    end

--do
--    local img = DefSubimages("fl-thief", {h=7})
--    local f = BuildFrames(img, 80)
--    DefAnim("thief-emerge", f)
--    DefAnim("thief-retreat", ReverseFrames(f))
--    DefRoundStone("fl-thief", img[1])
--    DefRoundStone("fl-thief-emerge", "thief-emerge")
--    DefRoundStone("fl-thief-retreat", "thief-retreat")
--end

end

-------------
-- Mirrors --
-------------
--
-- naming scheme for mirror models:
--
--      st-{3mirror,pmirror}-{m,s}{o,t}[1234]
--
-- {m,s} -> movable or static
-- {o,t,f} -> opaque or pane transparent or full transparent (pane + side)
--
-- The numbers map to actual orientations as follows:
--
--   NUMBER    TRIANG.M.   PLANE M.
--      1       south     "v"        "-"
--      2       west      "<"        "\"
--      3       north     "^"        "|"
--      4       east      ">"        "/"
do
    function make_mirror(basename, baseimg, overlays)
        for i=1,4 do
                mname = basename .. i
                DefOverlay (mname .. "-ovl", {baseimg, overlays[i]})
                DefShModel(mname, mname .. "-ovl", "sh-round2")
        end
    end

    mirror3_opaque = FrameNames("st-mirrortempl", 1, 4)
    mirror3_transp = FrameNames("st-mirrortempl", 5, 8)
    mirrorp_opaque = FrameNames("st-mirrortempl", 9, 12)
    mirrorp_transp1 = FrameNames("st-mirrortempl", 13, 16)
    mirrorp_transp2 = FrameNames("st-mirrortempl", 17, 20)

    make_mirror("st-3mirror-mo", "st-mirror-movable", mirror3_opaque)
    make_mirror("st-3mirror-so", "st-mirror-static",  mirror3_opaque)
    make_mirror("st-3mirror-mt", "st-mirror-movable", mirror3_transp)
    make_mirror("st-3mirror-st", "st-mirror-static",  mirror3_transp)

    make_mirror("st-pmirror-mo", "st-mirror-movable", mirrorp_opaque)
    make_mirror("st-pmirror-so", "st-mirror-static",  mirrorp_opaque)
    make_mirror("st-pmirror-mt", "st-mirror-movable", mirrorp_transp1)
    make_mirror("st-pmirror-st", "st-mirror-static",  mirrorp_transp1)
    make_mirror("st-pmirror-mf", "st-mirror-movable", mirrorp_transp2)
    make_mirror("st-pmirror-sf", "st-mirror-static",  mirrorp_transp2)
end

--------------------------
-- Simple stone aliases --
--------------------------
do
    DefAlias("st-bluegray_hole", "st-bluegray")
    DefAlias("st-breaking", "st-rock3_break-anim")
    DefAlias("st-camouflage_move", "st-camouflage")
    DefAlias("st-camouflage_hole", "st-camouflage")
    DefAlias("st-explosion", "expl")
    DefAlias("st-firebreak_move", "st-firebreak")
    DefAlias("st-glass_move", "st-glass");
    DefAlias("st-glass1_hole", "st-glass1")
    DefAlias("st-glass1_move", "st-glass1")
    DefAlias("st-glass2_hole", "st-glass2")
    DefAlias("st-glass2_move", "st-glass2")
    DefAlias("st-greenbrown_hole", "st-greenbrown")
    DefAlias("st-greenbrown_move", "st-greenbrown")
    DefAlias("st-laserbreak", "st-rock3")
    DefAlias("st-laserbreak-anim", "st-rock3_break-anim")
    DefAlias("st-marble_hole", "st-marble")
    DefAlias("st-marble_move", "st-marble")
    DefAlias("st-metal_hole", "st-metal")
    DefAlias("st-plain", "st-rock3")
    DefAlias("st-plain_break", "st-rock3")
    DefAlias("st-plain_breaking", "st-rock3_break-anim")
    DefAlias("st-plain_cracked", "st-rock3_cracked")
    DefAlias("st-plain_falling", "st-rock3-falling")
    DefAlias("st-plain_hole", "st-rock3")
    DefAlias("st-plain_move", "st-rock3")
    DefAlias("st-rock1_hole", "st-rock1")
    DefAlias("st-rock1_move", "st-rock1")
    DefAlias("st-rock2_hole", "st-rock2")
    DefAlias("st-rock3_hole", "st-rock3")
    DefAlias("st-rock3_move", "st-rock3")
    DefAlias("st-rock3_break", "st-rock3")
    DefAlias("st-yinyang3", "st-yinyang1")
end

-- Invisible stones --
do
    DefAlias("st-actorimpulse_invisible", "invisible")
    DefAlias("st-break_invisible", "invisible")
    DefAlias("st-death_invisible", "invisible")
    DefAlias("st-invisible", "invisible")
    DefAlias("st-invisible_magic", "invisible")
    DefAlias("st-stonebrush", "invisible")
end

--------------------------------------------------------------------------------
--                               Sprite Effects                               --
--------------------------------------------------------------------------------
Progress(95, "Loading Sprite Effects")

Sprite({
    name     = "ring-anim",
    nimages  = 8,
    framelen = 50,
    padding  = 0.3
})

Sprite({
    name     = "ac-drowned",
    nimages  = 5,
    framelen = 80,
    padding  = 0.3
})

-- Halo --
Sprite({
    name     = "halo",
    nimages  = 2,
    framelen = 30,
    loop     = true
})

Sprite({
    name     = "halo-blink",
    nimages  = 2,
    framelen = 30,
    loop     = true
})

-- Halo for small balls --
Sprite({
    name     = "halo-small",
    nimages  = 2,
    framelen = 30,
    loop     = true
})

Sprite({
    name     = "halo-small-blink",
    nimages  = 2,
    framelen = 30,
    loop     = true
})

Progress(100, "Loading Enigma Levels")
