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
    DefShModel("ac_marble_black", "fg-blackball1", "sh-blackball")
    DefShModel("ac_marble_black-shine", "fg-blackball2", "sh-blackball")

    -- Normal Whiteball
    -- Use shadow from black ball
    DefAlias("sh-whiteball", "sh-blackball")
    images = SpriteImages("fg-whiteball", 2, 0.5, 0.32)
    DefShModel("ac_marble_white", "fg-whiteball1", "sh-whiteball")
    DefShModel("ac_marble_white-shine", "fg-whiteball2", "sh-whiteball")

    -- Falling Blackball
    images = SpriteImages("ac-blackball-fall", 10)
    frames = ComposeFrames(images,{70,65,60,55,50,50,50,50,50,50,50})
    DefAnim("ac_marble_black-fall", frames)
    DefAlias("ac_marble_black-fallen", "invisible")

    -- Appearing / disappearing Blackball
    -- use the images from falling
    DefAnim("ac_marble_black-appear", ReverseFrames(BuildFrames(images, 25)))
    DefAnim("ac_marble_black-disappear", BuildFrames(images, 25))

    -- Falling Whiteball
    images = SpriteImages("ac-whiteball-fall", 10)
    frames = ComposeFrames(images,{70,65,60,55,50,50,50,50,50,50,50})
    DefAnim("ac_marble_white-fall", frames)
    DefAlias("ac_marble_white-fallen", "invisible")

    -- Appearing / disappearing Whiteball
    -- use the images from falling
    DefAnim("ac_marble_white-appear", ReverseFrames(BuildFrames(images, 25)))
    DefAnim("ac_marble_white-disappear", BuildFrames(images, 25))

    -- Jumping Blackball
    images  = SpriteImages("ac-blackball-jump", 4)
    shadows = SpriteImages("sh-blackball-jump", 4, 0.4)
    frames  = {}
    for i=1,4 do
        DefShModel("bb-jump"..i, images[i], shadows[i])
        table.insert(frames, "bb-jump"..i)
    end
    DefAnim("ac_marble_black-jump", PingPong(BuildFrames(frames, 70)))

    -- Jumping Whiteball
    -- Use shadow from black ball
    images  = SpriteImages("ac-whiteball-jump", 4)
    frames  = {}
    for i=1,4 do
        DefShModel("wb-jump"..i, images[i], shadows[i])
        table.insert(frames, "wb-jump"..i)
    end
    DefAnim("ac_marble_white-jump", PingPong(BuildFrames(frames, 70)))

    -- Sinking Blackball
    shadows = SpriteImages("sh-blackball-sink", 7, 0.4)
    images = SpriteImages("fg-blackball-sink", 7)
    for i=1,table.getn(images) do
        DefShModel("ac_marble_black-sink"..(i-1), images[i], shadows[i])
    end
    DefAlias("ac_marble_black-sunk", "invisible")

    -- Sinking Whiteball
    -- Use shadow from black ball
    images = SpriteImages("fg-whiteball-sink", 7)
    for i=1,table.getn(images) do
        DefShModel("ac_marble_white-sink"..(i-1), images[i], shadows[i])
    end
    DefAlias("ac_marble_white-sunk", "invisible")

    -- Shattering Blackball
    Sprite({name="ac-blackball-shatter", nimages=5, framelen=60})
    DefAlias("ac_marble_black-shatter", "ac-blackball-shatter")
    DefAlias("ac_marble_black-shattered", "ac-blackball-shatter5")

    -- Shattering Whiteball
    Sprite({name="ac-whiteball-shatter", nimages=5, framelen=60})
    DefAlias("ac_marble_white-shatter", "ac-whiteball-shatter")
    DefAlias("ac_marble_white-shattered", "ac-whiteball-shatter5")
end

-- ac-whiteball-small --
do
    local images, frames, shadows

    -- Normal
    SpriteImage ("sh-whiteball-small", 0.4, 0.41)
    SpriteImage ("fg-whiteball-small", 0.5, 0.43)
    DefShModel("ac_pearl_white", "fg-whiteball-small", "sh-whiteball-small")
    DefAlias ("ac_pearl_white-shine", "ac_pearl_white")

    -- Falling
    images = SpriteImages ("ac-whiteball-small-fall", 5, 0.5, 0.43)
    table.insert(images, "invisible")
    DefAnim("ac_pearl_white-fall", ComposeFrames(images,{70,65,60,55,50,30}))
    DefAlias("ac_pearl_white-fallen", "invisible")

    -- Appearing / disappearing
    -- use the images from falling
    DefAnim("ac_pearl_white-appear", ReverseFrames(BuildFrames(images, 25)))
    DefAnim("ac_pearl_white-disappear", BuildFrames(images, 25))

    -- Jumping
    images  = SpriteImages("ac-whiteball-small-jump", 4)
    shadows = SpriteImages("sh-whiteball-small-jump", 4, 0.4)
    frames  = {}
    for i=1,4 do
        DefShModel("sb-jump"..i, images[i], shadows[i])
        table.insert(frames, "sb-jump"..i)
    end
    DefAnim("ac_pearl_white-jump", PingPong(BuildFrames(frames, 70)))

    -- sinking
    DefAlias ("ac_pearl_white-sink0", "ac-whiteball-small-fall1")
    DefAlias ("ac_pearl_white-sink1", "ac-whiteball-small-fall2")
    DefAlias ("ac_pearl_white-sink2", "ac-whiteball-small-fall3")
    DefAlias ("ac_pearl_white-sink3", "ac-whiteball-small-fall3")
    DefAlias ("ac_pearl_white-sink4", "ac-whiteball-small-fall4")
    DefAlias ("ac_pearl_white-sink5", "ac-whiteball-small-fall4")
    DefAlias ("ac_pearl_white-sink6", "ac-whiteball-small-fall5")
    DefAlias ("ac_pearl_white-sunk", "invisible")

    -- Shattering
    images = SpriteImages ("ac-whiteball-small-shatter", 5)
    DefAnim("ac_pearl_white-shatter", BuildFrames(images, 60))
    SpriteImage ("ac-whiteball-small-shattered")
    DefAlias("ac_pearl_white-shattered", "ac-whiteball-small-shattered")
end

-- ac-killerball --
do
    DefAlias("ac_killer", "ac_pearl_white")
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
    DefShModel("ac_bug", "fg-bug", "sh-whiteball-small")
end

-- ac-cannonball --
do
    DefAlias("ac_cannonball", "ac_marble_black-jump")
end

-- ac-horse --
do
    SpriteImage("fg-horse",0.5,0.18)
    SpriteImage("sh-horse", 0.4)
    DefShModel("ac_horse", "fg-horse", "sh-horse")
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
    DefAnim("ac_rotor", BuildFrames(frames, 30), true)
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
    DefAnim("ac_top", BuildFrames(frames, 25), true)
end

--------------------------------------------------------------------------------
--                                Floor models                                --
--------------------------------------------------------------------------------
Progress(15, "Loading floor models")

-------------------
-- floor borders --
-------------------
do
    DefImage("floor_frame")
end

-------------------------------------------------------
-- simple tile floors (single and multiple (random)) --
-------------------------------------------------------
do
    floorlist = {
        -- single
        {"fl_abyss", noframe = true},
        {"fl_dark"},
        {"fl_bluegreen"},
        {"fl_darkgray"},
        {"fl-dummy", noframe = true},
        {"fl_dunes"},
        {"fl_bast"},
        {"fl_ice", noframe = true},
        {"fl_inverse_white", noframe = true},
        {"fl_inverse_gray", noframe = true},
        {"fl_ivory"},
        {"fl_platinum"},
        {"fl_adhesionless", noframe = true},
        {"fl-normal"},
        {"fl_sand"},
        {"fl_stone"},
        {"fl_white"},
        {"fl_fake_trigger"},
        {"fl_bright"},
        -- multiple, with height and width (or 1)
        {"fl_bluegray", 4},
        {"fl_aquamarine", 2},
        {"fl_brick", 3},
        {"fl_pinkbumps", 2, 2},
        {"fl_concrete", 4},
        {"fl_gravel", 4},
        {"fl_gray", 5},
        {"fl_hay", 4},
        {"fl_himalaya", 4},
        {"fl_marble", 4},
        {"fl_metal", 6},
        {"fl_mortar", 2, 2},
        {"fl_plank", 4},
        {"fl_red", 4},
        {"fl_rock", 2},
        {"fl_rough", 5},
        {"fl_blueslab", 4},
        {"fl_redslab", 4},
        {"fl_sahara", 4},
        {"fl_samba", 2},
        {"fl_space", 3, 2, noframe = true},
        {"fl-stwood", 2},
        {"fl_swamp", 4, noframe = true},
        {"fl_tigris", 4},
        {"fl_water", 4, noframe = true},
        {"fl_woven", 5},
    }

    for _, floor in pairs(floorlist) do
        local basename = floor[1]
        local height = floor[2] or 1
        local width = floor[3] or 1
        if (height == 1) and (width == 1) then
            -- single floor
            DefImage(basename)
            if not floor.noframe then
                display.DefineComposite(basename .. "_framed", basename, "floor_frame")
            end
        else
            -- multiple (random) floor
            local imagelist = DefSubimages(basename, {h = height, w = width})
            display.DefineRandModel(basename, getn(imagelist), imagelist)
            if not floor.noframe then
                local framedlist = {}
                for _, subimage in pairs(imagelist) do
                    display.DefineComposite(subimage .. "_framed", subimage, "floor_frame")
                    table.insert(framedlist, subimage .. "_framed")
                end
                display.DefineRandModel(basename .. "_framed", getn(framedlist), framedlist)
            end
        end
    end
end


-----------------------------------------
-- Special Floors
-----------------------------------------

-- gradients
do
    DefSubimages("fl-gradient2", {w=6, h=4, modelname="fl-gradient"})
    DefAlias("fl_slope", "fl-gradient21")
    DefAlias("fl_slope_pw", "fl-gradient4")
    DefAlias("fl_slope_ps", "fl-gradient1")
    DefAlias("fl_slope_pe", "fl-gradient3")
    DefAlias("fl_slope_pn", "fl-gradient2")
    DefAlias("fl_slope_inw", "fl-gradient12")
    DefAlias("fl_slope_isw", "fl-gradient11")
    DefAlias("fl_slope_ise", "fl-gradient9")
    DefAlias("fl_slope_ine", "fl-gradient10")
    DefAlias("fl_slope_onw", "fl-gradient8")
    DefAlias("fl_slope_osw", "fl-gradient6")
    DefAlias("fl_slope_ose", "fl-gradient5")
    DefAlias("fl_slope_one", "fl-gradient7")
    DefAlias("fl_slope_tw", "fl-gradient21")
    DefAlias("fl_slope_ts", "fl-gradient21")
    DefAlias("fl_slope_te", "fl-gradient21")
    DefAlias("fl_slope_tn", "fl-gradient21")
    DefAlias("fl_slope_twl", "fl-gradient2")
    DefAlias("fl_slope_tsl", "fl-gradient4")
    DefAlias("fl_slope_tel", "fl-gradient1")
    DefAlias("fl_slope_tnl", "fl-gradient3")
    DefAlias("fl_slope_twr", "fl-gradient1")
    DefAlias("fl_slope_tsr", "fl-gradient3")
    DefAlias("fl_slope_ter", "fl-gradient2")
    DefAlias("fl_slope_tnr", "fl-gradient4")
end

-- leaves --
do
    DefSubimages("fl_lawn", {w=10, h=2, modelname="fl_lawnx"})
    DefRandFloor("fl_lawn", {"fl_lawnx1", "fl_lawnx2", "fl_lawnx3", "fl_lawnx4"})
    DefAlias("fl_lawn_a", "fl_lawn")
    DefRandFloor("fl_lawn_b", {"fl_lawnx5", "fl_lawnx6", "fl_lawnx7", "fl_lawnx8"})
    DefAlias("fl_lawn_c1", "fl_lawnx9")
    DefAlias("fl_lawn_c2", "fl_lawnx10")
    DefAlias("fl_lawn_c3", "fl_lawnx11")
    DefAlias("fl_lawn_c4", "fl_lawnx12")
    DefAlias("fl_lawn_d1", "fl_lawnx13")
    DefAlias("fl_lawn_d2", "fl_lawnx14")
    DefAlias("fl_lawn_d3", "fl_lawnx15")
    DefAlias("fl_lawn_d4", "fl_lawnx16")
    DefAlias("fl_lawn_e1", "fl_lawnx17")
    DefAlias("fl_lawn_e2", "fl_lawnx18")
    DefAlias("fl_lawn_e3", "fl_lawnx19")
    DefAlias("fl_lawn_e4", "fl_lawnx20")
end

-- wooden floor --
do
    DefSubimages("fl-wood", {w=1, h=4, modelname="fl-woodx"})
    DefRandFloor("fl-wood", {"fl-woodx1", "fl-woodx2", "fl-woodx3", "fl-woodx4"})
    DefRandFloor("fl-wood1", {"fl-woodx3", "fl-woodx4"})
    DefRandFloor("fl-wood2", {"fl-woodx1", "fl-woodx2"})

    DefAlias("fl_wood", "fl-wood")
    DefAlias("fl_wood1", "fl-wood1")
    DefAlias("fl_wood2", "fl-wood2")
    DefAlias("fl_wood_framed", "fl-stwood")
    DefAlias("fl_wood_framed1", "fl-stwood1")
    DefAlias("fl_wood_framed2", "fl-stwood2")
end

-- yinyang floor --
do
    DefSubimages("fl_yinyang_yin", {w=1, h=2, modelname="fl_yinyang_yinx"})
    DefSubimages("fl_yinyang_yang", {w=1, h=2, modelname="fl_yinyang_yangx"})
    DefAlias("fl_yinyang_yin", "fl_yinyang_yinx1")
    DefAlias("fl_yinyang_yin_invisible", "fl_yinyang_yinx2")
    DefAlias("fl_yinyang_yang", "fl_yinyang_yangx1")
    DefAlias("fl_yinyang_yang_invisible", "fl_yinyang_yangx2")
end

--------------------------
-- Simple floor aliases --
--------------------------
do
    DefAlias("fl_fake_abyss", "fl_abyss")
    DefAlias("fl_inverse", "fl_inverse_white")
    DefAlias("fl_metal_1", "fl_metal1")
    DefAlias("fl_metal_2", "fl_metal2")
    DefAlias("fl_metal_3", "fl_metal3")
    DefAlias("fl_metal_4", "fl_metal4")
    DefAlias("fl_metal_5", "fl_metal5")
    DefAlias("fl_metal_6", "fl_metal6")
    DefAlias("fl_metal_7", "fl-normal")
    DefAlias("fl_metal_7n", "fl-normal")
    DefAlias("fl_metal_1_framed", "fl_metal1_framed")
    DefAlias("fl_metal_2_framed", "fl_metal2_framed")
    DefAlias("fl_metal_3_framed", "fl_metal3_framed")
    DefAlias("fl_metal_4_framed", "fl_metal4_framed")
    DefAlias("fl_metal_5_framed", "fl_metal5_framed")
    DefAlias("fl_metal_6_framed", "fl_metal6_framed")
    DefAlias("fl_metal_7_framed", "fl-normal_framed")
    DefAlias("fl-space-force", "fl_space")
    DefAlias("fl_samba_h", "fl_samba1")
    DefAlias("fl_samba_v", "fl_samba2")
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
        local images = DefSubimages(basemodel.."_heating", {h=8});
        local frames = BuildFrames(images, 240)
        DefAnim(basemodel.."_heating", frames);
    end

    heating_animation("fl_ice")
    heating_animation("fl_water")
    heating_animation("fl_swamp")
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
        "it_bag",
        "it_banana",
        "it_bomb_black",
        "it_bomb_white",
        "it-blocker",
        "it_bottle_idle",
        "it_bottle_broken",
        "it_brush",
        "it_cherry",
        "it_cross",
        "it_document",
        "it_drop",
        "it_dynamite",
        "it_extralife",
        "it_flag_black",
        "it_flag_white",
        "it_floppy",
        "it_glasses",
        "it_glasses_broken",
        "it_hammer",
        "it_key",
        "it_landmine",
        "it_magicwand",
        "it_odometer",
        "it_pencil",
        "it_pin",
        "it_ring",
        "it_rubberband",
        "it_spade",
        "it_spoon",
        "it_spring_keep",
        "it_spring_drop",
        "it_squashed",
        "it_surprise",
        "it_sword",
        "it_umbrella",
        "it_weight",
        "it_wrench"
    }

    DefImages(itemlist)

    DefImage("it_brake", {filename="st-brake"})
    DefImage("it_coin_s", {filename="it-coin1"})
    DefImage("it_coin_m", {filename="it-coin2"})
    DefImage("it_coin_l", {filename="it-coin4"})
    DefImage("it_meditation_hollow", {filename="it_meditation_hollow"})
    DefImage("it_meditation_hill", {filename="it_meditation_hill"})
    DefImage("it_meditation_dent", {filename="it_meditation_dent"})
    DefImage("it_meditation_bump", {filename="it_meditation_bump"})
    DefImage("it_meditation_volcano", {filename="it_meditation_volcano"})
    DefImage("it_meditation_caldera", {filename="it_meditation_caldera"})
end

-------------------------
-- Simple item aliases --
-------------------------

do
    DefAlias("it-key_a", "it-key")
    DefAlias("it-key_b", "it-key")
    DefAlias("it-key_c", "it-key")
end

--------------------------------------------------------------
-- Multiple-Image-Items, non animated, e.g. it-burnable-oil --
--------------------------------------------------------------

-- it-extinguisher --
do
    DefTiles("it_extinguisher", {"it_extinguisher_full", "it_extinguisher_medium", "it_extinguisher_empty"})
end

-- it-dummy
do
    DefTiles("it_dummy", {"it_dummy", "it_dummy_egg"})
end

-- Oil --
do
    DefSubimages("it_burnable_oil", {h=4})
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
    DefTiles("it_yinyang", {"it_yinyang", "it_yangyin"})
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
    local images = DefSubimages("it_bomb_black_burning", {h=9})
    local frames = BuildFrames(images, 100)
    DefAnim("it_bomb_black_burning", frames)
    
    local frames = BuildFrames({"it_bomb_black_burning8", "it_bomb_black_burning9"},120)
    DefAnim("it_bomb_black_exploding", frames)
end

-- Burning white bomb --
do
    local images = DefSubimages("it_bomb_white_burning", {h=9})
    local frames = BuildFrames(images, 100)
    DefAnim("it_bomb_white_burning", frames
    )
    local frames = BuildFrames({"it_bomb_white_burning8", "it_bomb_white_burning9"},120)
    DefAnim("it_bomb_white_exploding", frames)
end

-- Burning dynamite --
do
    local images = DefSubimages("it_dynamite_burning", {h=15})
    local frames = BuildFrames(images, 100)
    DefAnim("it_dynamite_burning", frames)
end

-- Burning Floor --
do
    local images = DefSubimages("it_burnable_ignite", {h=8})
    local frames = BuildFrames(images, 100)
    DefAnim("it_burnable_ignited", frames)

    local images = DefSubimages("it_burnable_burning", {h=8})
    local frames = BuildFrames(images, 100)
    DefAnim("it_burnable_burning", frames)

    -- Simple fire related items:
    DefImages({"it_burnable_ash", "it_burnable_fireproof"})
    DefAlias("it_burnable_invisible", "invisible")
end

-- it-coffee --
do
    local images = DefSubimages("it_coffee", {h=4})
    local frames = BuildFrames(images,150)
    DefAnim("it_coffee", frames, true)
end

-- Cracks --
do
    local images = DefSubimages("it_crack_abyss", {h=8})
    local frames = BuildFrames(images,50)
    DefAnim("it_debris", frames)

    local frames = BuildFrames({"it_crack_abyss4", "it_crack_abyss5", "it_crack_abyss6", "it_crack_abyss7", "it_crack_abyss8"},120)
    DefAnim("it_crack_abyss_anim", frames)
    
    local images = DefSubimages("it_crack_water", {h=8})
    local frames = BuildFrames(images,50)
    DefAnim("it_debris_water", frames)

    local frames = BuildFrames({"it_crack_water4", "it_crack_water5", "it_crack_water6", "it_crack_water7", "it_crack_water8"},120)
    DefAnim("it_crack_water_anim", frames)

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
    DefAlias("it_explosion", "expl")
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
    local images = {"it_puller_n", "it_puller_e", "it_puller_s", "it_puller_w"}
    DefTiles("it_puller", images)
    local frames = BuildFrames(images, 100)
    DefAnim("it_puller_active", RepeatAnim(frames, 4), false)
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
    DefTiles("it_sensor_exit", {"it_sensor_exit","it_sensor_exit1"})
    local exitframes = BuildFrames({"it_sensor_exit","it_sensor_exit1"}, 300)
    DefAnim("it_sensor_exit_hit", RepeatAnim(exitframes, 3), false)
end

-- it-shogun --
do
    NewAnim("it_shogun_s", {img="it-shogun-small", h=3, speed=160, pingpong=1, loop=1})
    NewAnim("it_shogun_m", {img="it-shogun-med",   h=3, speed=160, pingpong=1, loop=1})
    NewAnim("it_shogun_l", {img="it-shogun-big",   h=3, speed=160, pingpong=1, loop=1})
end

-- it-springboard --
do
    local images = {"it_springboard1", "it_springboard2"}
    DefTiles ("it_springboard", images)
    DefAlias ("it_springboard", images[1])
    DefAnim("it_springboard_anim", BuildFrames(ReverseFrames(images),120))
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
    DefImage("sh_grate_cross")
    DefImage("sh_grate_framed")
    DefImage("sh_portal_horse")
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
    DefStone("st_beads", "sh-glass")
    DefStone("st-black1")
    DefStone("st-black2")
    DefStone("st-black3")
    DefStone("st-black4", "sh-white4")
    DefStone("st_jamb_black")
    DefStone("st_plop_slate")
    DefStone("st_bluegray", "sh-round")
    DefStone("st-brake", "sh-brake")
    DefStone("st_brownie", "sh-round")
    DefStone("st-bug")
    DefStone("st_pinkbumps")
    DefStone("st_camouflage")
    DefStone("st_dummy")
    DefImage("st-easymode")
    DefStone("st-flhay")
    DefStone("st_flat_fire")
    DefStone("st-floppy0", "sh-round", {filename="st-floppy1"})
    DefStone("st-floppy1", "sh-round", {filename="st-floppy2"})
    DefStone("st-flrock")
    DefStone("st_rawglass", "sh-glass");
    DefStone("st_lightglass", "sh-glass")
    DefStone("st_darkglass", "sh-glass")
    DefStone("st_redfiber", "sh-glass")
    DefStone("st_grate_cross", "sh_grate_cross")
    DefStone("st_grate_framed", "sh_grate_framed")
    DefStone("st_portal_horse", "sh_portal_horse")
    DefStone("st_greenbrown", "sh-round")
    DefStone("st-key0", "sh-round", {filename="st-key1"})
    DefStone("st-key1", "sh-round", {filename="st-key2"})
    DefStone("st_redbrown", "sh-round")
    DefStone("st_metal")
    DefStone("st_redrock")
    DefStone("st_granite", "sh-round")
    DefStone("st_darkgray")
    DefStone("st_plaster")
    DefStone("st_flat")
    DefStone("st_flat_cracked")
    DefStone("st_purplemarble")
    DefStone("st_tigris")
    DefStone("st_purplegray")
    DefStone("st_greengray")
    DefStone("st_concrete", "sh-round")
    DefStone("st_ancient", "sh-round")
    DefStone("st_brownmarble")
    DefStone("st-rubberband")
    DefStone("st_brownpyramide")
    DefStone("st_redmarble")
    DefStone("st_surprise")
    DefStone("st-white1")
    DefStone("st-white2")
    DefStone("st-white3")
    DefStone("st-white4", "sh-white4")
    DefStone("st_jamb_white")
    DefStone("st_woven")
    DefStone("st_yellow")
    DefStone("st_blur_straight")
    DefStone("st_blur_cross")
    DefStone("st_yinyang_inactive", "sh-round")
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
    DefSolidStone("st_magic_brick", "st-bigbrickx1")
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
    DefSolidStoneWithAnim ("st_break_oxydc", 10, 50)
    DefSolidStoneWithAnim ("st_break_boulder", 10, 50)
    DefRoundStoneWithAnim ("st_break_white", 10, 50)
    DefRoundStoneWithAnim ("st_break_black", 10, 50)
    DefRoundStoneWithAnim ("st_break_plain", 10, 50)
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
        --local img2 = DefSubimages("st-chess"..colour.."-capture", {h=5});
        local f1 = BuildFrames(img1, 120)
        --local f2 = BuildFrames(img2, 40)
        local f2 = f1
        local f3 = BuildFrames(img1, 500)
        DefAnim("st_chess"..colour.."-disappearing", f1);
        DefAnim("st_chess"..colour.."-appearing", ReverseFrames(f1))
        DefAnim("st_chess"..colour.."-capture", f2);
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
    DefTiles("st-disco", {"st_disco0","st_disco1","st_disco2"})
end

-- st-death --
do
    DefRoundStoneWithAnim ("st-death", 3, 140)
end

-- st_yinyang --
do
    DefRoundStoneWithAnim("st_yinyang", 6, 140)
end

-- st-dynamite, st-bombs, st-whitebombs --
do
    function make_bombstone(name)
        local images = DefSubimages(name, {h=7})
        DefRoundStone(name, name.."1")
        DefAnim(name.."_breaking", BuildFrames(images,50))
    end

    make_bombstone("st_dispenser_bombblack")
    make_bombstone("st_dispenser_dynamite")
    make_bombstone("st_dispenser_bombwhite")
end

-- st-flash --
do
    local images = DefSubimages("st_flash", {h=1})
    DefSolidStone("st_flash", "st_flash1")
end

-- st-fourswitch --
do
   DefSubimages("st-fourswitch",{h=4})
   DefShModel("st-fourswitch-n", "st-fourswitch1", "sh-round")
   DefShModel("st-fourswitch-e", "st-fourswitch2", "sh-round")
   DefShModel("st-fourswitch-s", "st-fourswitch3", "sh-round")
   DefShModel("st-fourswitch-w", "st-fourswitch4", "sh-round")
end

-- st_ice --
do
    DefAlias("st_ice", "st_rawglass")
    DefAlias("st_ice_breaking", "it_debris_water")
--    local images = DefSubimages("st_ice_break", {h=3})
--    local frames = BuildFrames(images,50)
--    DefAnim("st_ice_break-anim", frames)
--    DefShModel("st_ice_breaking", "st_ice_break-anim", "sh-glass")
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
    DefStone("st-laserswitch0", nil, {filename="st_oxydb"})
    DefAnimImages("laserswitch-blink",
        RepeatAnim(PingPong(BuildFrames(FrameNames("st_fakeoxyd_blink", 1,4),50))), {loop=1})
    DefSolidStone("st-laserswitch1", "laserswitch-blink")
end

-- st-lightpassenger --
do
    local img = DefSubimages("st-lightpassenger", {h=7})
    DefShModel("st-lightpassenger", img[1], "sh-glass")
    DefImage("st-lightpassenger_offx", {filename = "st-lightpassenger_off"})
    DefShModel("st-lightpassenger_off", "st-lightpassenger_offx", "sh-glass")
    DefAlias("st-lightpassenger_hidden", "st_darkglass")
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

-- st_dongle :-) --
do
    local img = DefSubimages("st_dongle", {h=4, modelname="st_dongle-fg"})
    local nlist = {}
    for i=1, table.getn(img) do
        nlist[i] = "st_dongle"..i
        DefRoundStone(nlist[i], img[i])
    end
    display.DefineRandModel("st_dongle", table.getn(nlist), nlist)
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
    DefSubimages("st_puzzle", {modelname="st_puzzlex",w=16,h=4,startindex=0})
    -- Full puzzels
    for i=0,31  do DefSolidStone("st_puzzle"..i, "st_puzzlex"..i) end
    -- Hollow puzzels
    for i=32,63  do DefShModel("st_puzzle"..i, "st_puzzlex"..i, "sh-puzzle1") end
end

-- st_flat-break --
do
    local images = DefSubimages("st_flat-break", {h=6})
    DefAnim("flat_break-anim",BuildFrames(images,50))
    DefSolidStone("st_flat_breaking", "flat_break-anim")
    DefAlias("st_plaster_breaking", "st_flat_breaking")
end

-- st-rock3-falling --
do
    local images = DefSubimages("st_flat_falling", {h=4})
    DefAnim("st_flat_falling", BuildFrames(images, 100))
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
   local images = DefSubimages("st-stoneimpulse", {h=6})
   DefRoundStone("st-stoneimpulse", images[1])
   DefRoundStone("st-stoneimpulse-steady", images[6])
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

do
    local images = DefSubimages("st_stoneimpulse_break", {h=8})
    DefAnim("st_stoneimpulse_breaking", BuildFrames(images,50))
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
DefAlias("st-turnstile-e", "st_puzzle1")
DefAlias("st-turnstile-w", "st_puzzle4")
DefAlias("st-turnstile-s", "st_puzzle8")
DefAlias("st-turnstile-n", "st_puzzle2")

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
    local colorspots = FrameNames("st_oxyd_color", 1, num_colors)
    AddFrameNames(colorspots, "st_oxyd_color", 96, 97)
    local blink_ovls = FrameNames("st_oxyd_blink", 1, 5)
    local pseudospots = {}
    pseudospots[-3] = FrameNames("st_oxyd_pseudo_3", 1, 4)
    pseudospots[-4] = FrameNames("st_oxyd_pseudo_4", 1, 8)

-- Define "fading in" and "fading out" animations for oxyd stones.
-- These two animations are combined with the stone images to
-- produce the opening and closing animations for oxyd stones.
    local baseimg = {
        a="st_oxyda_open",
        b="st_oxydb_open",
        c="st_oxydc_open",
        d="st_oxydd_open",
    }
    local shadow = {
       a="sh-round",
       b="sh-round",
       c="sh-solid",
       d="sh-solid",
    }
    local fopening = {
        a = BuildFrames(DefSubimages("st_oxyda_opening", {h=9}), 60),
        b = BuildFrames(DefSubimages("st_oxydb_opening", {h=14}), 40),
        c = BuildFrames(DefSubimages("st_oxydc_opening", {h=5}), 70),
        d = BuildFrames(DefSubimages("st_oxydd_opening", {h=5}), 70),
    }
    local fclosing = {
        a = ReverseFrames(fopening["a"]),
        b = ReverseFrames(fopening["b"]),
        c = ReverseFrames(fopening["c"]),
        d = ReverseFrames(fopening["d"]),
    }

    function mkopenclose(flavor, color)
        local n = "st_oxyd" .. flavor .. color
        local fadein = "oxyd"..flavor.."_fadein"
        local fadeout= "oxyd"..flavor.."_fadeout"
        local spotcolor = color

        if (color >= 0) then
            spotcolor = color + 1 -- oxyd color 0..num_colors-1, file 1.., frames 1..
        else
            spotcolor = 100 + color -- pseudo colors
        end

        DefOverlay(n.."_base", {baseimg[flavor], colorspots[spotcolor]})
        display.DefineComposite(n.."_opening_fg", n.."_base", fadein)
        display.DefineComposite(n.."_closing_fg", n.."_base", fadeout)
        DefShModel (n.."_opening", n.."_opening_fg", shadow[flavor])
        DefShModel (n.."_closing", n.."_closing_fg", shadow[flavor])
    end

    function mkblink(flavor, color)
        local n = "st_oxyd"..flavor..color.."_blink"
        local img={baseimg[flavor],colorspots[color+1], "st_oxyd_questmark"}
        DefOverlay(n..1, img)
        DefOverlay(n..2, {baseimg[flavor], colorspots[color+1]})
        DefAnim(n.."_anim", BuildFrames({n..1,n..2}, 500), 1)
        DefShModel(n, n.."_anim", shadow[flavor])
    end

    function mkopened(flavor, color)
        local n = "st_oxyd" .. flavor .. color .. "_open"
        local names = {}

        for i=1, table.getn(blink_ovls) do
            local images={baseimg[flavor],colorspots[color+1],blink_ovls[i]}
            names[i] = n .. format("_%04d", i)
            DefOverlay(names[i], images)
        end

        -- compose these images into an animation
        frames = PingPong(BuildFrames(names, 100))
        DefAnim(n.."_anim", frames, true)

        -- and finally add a shadow to make the model complete
        DefShModel(n, n.."_anim", shadow[flavor])
    end

    function mkpseudo(flavor, color)
        local n = "st_oxyd" .. flavor .. "_pseudo" .. color
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
        DefAnim(n.."_anim", frames, false)

        -- and finally add a shadow to make the model complete
        DefShModel(n, n.."_anim", shadow[flavor])
    end

    function mkoxyd(flavor)
        DefStone("st_oxyd"..flavor, shadow[flavor])
        DefShModel("st_fake_oxyd"..flavor, "st_oxyd"..flavor, shadow[flavor])
--        DefSolidStone("st_fake_oxyd"..flavor, "st_oxyd"..flavor)
        img = DefImage("st_oxyd"..flavor.."_open")
        DefShModel("st_fake_oxyd"..flavor.."_open", img, shadow[flavor])

        local fadein = "oxyd"..flavor.."_fadein"
        local fadeout= "oxyd"..flavor.."_fadeout"
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

    -- flavors 'e'
    for color = 0, num_colors - 1 do
        DefOverlay("st_oxyde"..color.."_peep", {"st_oxydb_open", colorspots[color+1], "st_oxyde"})
        DefShModel("st_oxyde"..color, "st_oxyde"..color.."_peep", "sh-round")
        DefAlias("st_oxyde"..color.."_opening", "st_oxydb"..color.."_opening")
        DefAlias("st_oxyde"..color.."_closing", "st_oxydb"..color.."_closing")
        DefAlias("st_oxyde"..color.."_blink", "st_oxydb"..color.."_blink")
        DefAlias("st_oxyde"..color.."_open", "st_oxydb"..color.."_open")
    end
    DefAlias("st_oxyde", "st_oxydb")
    DefAlias("st_fake_oxyde", "st_oxydb")
    DefAlias("st_fake_oxyde_open", "st_fake_oxydb_open")
    DefAlias("st_oxyde-3_opening", "st_oxydb-3_opening")
    DefAlias("st_oxyde_pseudo-3", "st_oxydb_pseudo-3")
    DefAlias("st_oxyde-3_closing", "st_oxydb-3_closing")
    DefOverlay("st_oxyde-3_peep", {"st_oxydb_open", colorspots[97], "st_oxyde"})
    DefShModel("st_oxyde-3", "st_oxyde-3_peep", "sh-round")
    DefAlias("st_oxyde-4_opening", "st_oxydb-4_opening")
    DefAlias("st_oxyde_pseudo-4", "st_oxydb_pseudo-4")
    DefAlias("st_oxyde-4_closing", "st_oxydb-4_closing")
    DefOverlay("st_oxyde-4_peep", {"st_oxydb_open", colorspots[96], "st_oxyde"})
    DefShModel("st_oxyde-4", "st_oxyde-4_peep", "sh-round")
end

-- st-coffee --
do
    DefAlias("st_magic_oxydc", "st_oxydc")
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
    DefAlias("st-door_a-open", "st_grate_cross")
    DefAlias("st-door_a-closed", "st_oxyda")
    local f = BuildFrames({"st-door_a-closed", "st-door_a-open"},60)
    DefAnim("st-door_a-opening", f)
    DefAnim("st-door_a-closing", ReverseFrames(f))
end

-- st-door_b --
do
    DefAlias("st-door_b-open", "invisible")
    DefAlias("st-door_b-closed", "st_flat")
    local img=DefSubimages("st-doorb", {modelname="doorb", h=8})
    local f = BuildFrames(img,60)
    DefAnim("st-door_b-opening", f)
    DefAnim("st-door_b-closing", ReverseFrames(f))
end

-- st-door_c --
do
    DefAlias("st-door_c-open", "st_grate_framed")
    DefAlias("st-door_c-closed", "st_flat")
    frames=BuildFrames({"st-door_c-closed","st-door_c-open"},60)
    DefAnim("st-door_c-opening", frames)
    DefAnim("st-door_c-closing", ReverseFrames(frames))
end

-- st-fakeoxyda --
do
    DefAlias("st_magic_oxyda", "st_oxyda")
end

-- st_quake --
do
    DefStone("st_quake", "sh-round", {filename="st_oxydb"})
    DefAnimImages("quaking",
        RepeatAnim(PingPong(BuildFrames(FrameNames("st_fakeoxyd_blink", 1,4),50))))
    DefRoundStone("st_quaking", "quaking")

    namelist = DefSubimages("st_quake_break",{h=6})
    DefAnim("quakebreak_anim", BuildFrames(namelist,50))
    DefRoundStone("st_quakebreak_anim", "quakebreak_anim")
end

-- st-spitter --
do
    DefAlias ("st_spitter", "st-timeswitch")
    DefAlias ("st_spitter_loading", "st_quaking")
    DefAlias ("st_spitter_spitting", "st_quaking")
end

---------------------------
-- Thief stone and floor --
---------------------------
do
    -- Base images

    local stonebase = "st_bluegray"
    local floorbase = "fl_bluegray"
    local thiefovl = DefSubimages("thief_template", {h = 7})
    local captureovl = DefSubimages("thief_capture_template", {h = 12})
    local drunkenovl = DefSubimages("thief_drunken_template", {h = 2})

    -- Creating st-thief

    local names = {}
    for j = 1, table.getn(thiefovl) do
        names[j] = "st_thief"..format("_%04d", j)
        display.DefineComposite(names[j], stonebase, thiefovl[j])
    end

    local f1 = BuildFrames(names, 80)
    DefAnim("pre_st_thief_emerge", f1)
    DefAnim("pre_st_thief_retreat", ReverseFrames(f1))
    DefRoundStone("st_thief", stonebase)
    DefRoundStone("st_thief_emerge", "pre_st_thief_emerge")
    DefRoundStone("st_thief_retreat", "pre_st_thief_retreat")

    names = {}
    for j = 1, table.getn(drunkenovl) do
        names[j] = "st_thief_drunken"..format("%04d", j)
        display.DefineComposite(names[j], stonebase, drunkenovl[j])
    end
    
    local f5 = BuildFrames(names, 3000)
    DefAnim("pre_st_thief_drunken", f5, true)
    DefRoundStone("st_thief_drunken", "pre_st_thief_drunken")

    -- Creating st_thief_capture
    --
    --   Note that this is done without template, as the whole
    --   stone has to disappear (e.g. via shrinking)

    local img2 = DefSubimages("st_thief_capture", {h = 12})
    local f2 = BuildFrames(img2, 80)
    DefAnim("st_thief_capture", f2)

    -- Creating fl-thief

    local floornames = {}
    local floorcapture = {}
    local floordrunken = {}
    local floorbases = DefSubimages(floorbase, {h=4, modelname="fl_thief_base"})
    for k = 1,4 do
        floornames[k] = {}
        for j = 1, table.getn(thiefovl) do
            floornames[k][j] = "fl_thief"..k..format("_%04d", j)
            display.DefineComposite(floornames[k][j], floorbases[k], thiefovl[j])
        end
        floorcapture[k] = {}
        for j = 1, table.getn(captureovl) do
            floorcapture[k][j] = "fl_thief"..k.."_capture"..format("_%04d", j)
            display.DefineComposite(floorcapture[k][j], floorbases[k], captureovl[j])
        end
        floordrunken[k] = {}
        for j = 1, table.getn(drunkenovl) do
            floordrunken[k][j] = "fl-thief"..k.."-drunken"..format("_%04d", j)
            display.DefineComposite(floordrunken[k][j], floorbases[k], drunkenovl[j])
        end
        local f3 = BuildFrames(floornames[k], 80)
        local f4 = BuildFrames(floorcapture[k], 80)
        local f6 = BuildFrames(floordrunken[k], 3000)
        DefAlias("fl_thief"..k, floorbases[k])
        DefAnim("fl_thief"..k.."_emerge", f3)
        DefAnim("fl_thief"..k.."_retreat", ReverseFrames(f3))
        DefAnim("fl_thief"..k.."_capture", f4)
        DefAnim("fl_thief"..k.."_drunken", f6, true)
    end
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
    DefAlias("st_break_bug", "st-bug")
    DefAlias("st_break_bug-anim", "st_flat_breaking")
    DefAlias("st-explosion", "expl")
    DefAlias("st-glass_move", "st-glass");
    DefAlias("st_break_laser", "st_flat")
    DefAlias("st_break_laser-anim", "st_flat_breaking")
    DefAlias("st_blur_magic", "st_blur_cross")
end

-- Invisible stones --
do
    DefAlias("st-actorimpulse_invisible", "invisible")
    DefAlias("st-death_invisible", "invisible")
    DefAlias("st_invisible", "invisible")
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
