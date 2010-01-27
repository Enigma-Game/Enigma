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

-- ac_marble --
do
    local images,frames,shadows
    
    -- Normal black marble
    images = SpriteImages("ac_marble_black", 2, 0.5, 0.31)
    shadows = SpriteImage("sh_marble", 0.4, 0.29)
    DefShModel("ac_marble_black", "ac_marble_black1", "sh_marble")
    DefShModel("ac_marble_black-shine", "ac_marble_black2", "sh_marble")
    
    -- Normal white marble
    -- Use shadow from black marble
    images = SpriteImages("ac_marble_white", 2, 0.5, 0.31)
    DefShModel("ac_marble_white", "ac_marble_white1", "sh_marble")
    DefShModel("ac_marble_white-shine", "ac_marble_white2", "sh_marble")
    
    -- Falling black marble
    images = SpriteImages("ac_marble_black_fall", 10)
    shadows = SpriteImages("sh_marble_fall", 10, 0.4)
    frames = {}
    for i=1,table.getn(images) do
        DefShModel("ac_marble_black-sink"..(i-1), images[i], shadows[i])
        frames[i] = "ac_marble_black-sink"..(i-1)
    end
    DefAnim("ac_marble_black-fall", ComposeFrames(frames,{70,65,60,55,50,50,50,50,50,50,50}))
    DefAlias("ac_marble_black-fallen", "invisible")
    
    -- Appearing / disappearing black marble
    -- use the images from falling black marble
    DefAnim("ac_marble_black-appear", ReverseFrames(BuildFrames(frames, 25)))
    DefAnim("ac_marble_black-disappear", BuildFrames(frames, 25))

    -- Falling white marble
    -- Use shadow from black marble
    images = SpriteImages("ac_marble_white_fall", 10)
    frames = {}
    for i=1,table.getn(images) do
        DefShModel("ac_marble_white-sink"..(i-1), images[i], shadows[i])
        frames[i] = "ac_marble_white-sink"..(i-1)
    end
    DefAnim("ac_marble_white-fall", ComposeFrames(frames,{70,65,60,55,50,50,50,50,50,50,50}))
    DefAlias("ac_marble_white-fallen", "invisible")
    
    -- Appearing / disappearing white marble
    -- use the images from falling white marble
    DefAnim("ac_marble_white-appear", ReverseFrames(BuildFrames(frames, 25)))
    DefAnim("ac_marble_white-disappear", BuildFrames(frames, 25))

    -- Jumping black marble
    images  = SpriteImages("ac_marble_black_jump", 4)
    shadows = SpriteImages("sh_marble_jump", 4, 0.4)
    frames  = {}
    for i=1,4 do
        DefShModel("bb-jump"..i, images[i], shadows[i])
        table.insert(frames, "bb-jump"..i)
    end
    DefAnim("ac_marble_black-jump", PingPong(BuildFrames(frames, 70)))

    -- Jumping white marble
    -- Use shadow from black marble
    images  = SpriteImages("ac_marble_white_jump", 4)
    frames  = {}
    for i=1,4 do
        DefShModel("wb-jump"..i, images[i], shadows[i])
        table.insert(frames, "wb-jump"..i)
    end
    DefAnim("ac_marble_white-jump", PingPong(BuildFrames(frames, 70)))

    -- Sinking black marble
    shadows = SpriteImages("sh_marble_sink", 7, 0.4)
    images = SpriteImages("ac_marble_black_sink", 7)
    for i=1,table.getn(images) do
        DefShModel("ac_marble_black-sink"..(i-1), images[i], shadows[i])
    end
    DefAlias("ac_marble_black-sunk", "invisible")
    
    -- Sinking white marble
    -- Use shadow from black marble
    images = SpriteImages("ac_marble_white_sink", 7)
    for i=1,table.getn(images) do
        DefShModel("ac_marble_white-sink"..(i-1), images[i], shadows[i])
    end
    DefAlias("ac_marble_white-sunk", "invisible")

    -- Shattering black marble
    Sprite({name="ac_marble_black_shatter", nimages=5, framelen=60})
    DefAlias("ac_marble_black-shatter", "ac_marble_black_shatter")
    DefAlias("ac_marble_black-shattered", "ac_marble_black_shatter5")

    -- Shattering white marble
    Sprite({name="ac_marble_white_shatter", nimages=5, framelen=60})
    DefAlias("ac_marble_white-shatter", "ac_marble_white_shatter")
    DefAlias("ac_marble_white-shattered", "ac_marble_white_shatter5")
end

-- ac_pearl --
do
    local images, frames, shadows
    
    -- Normal black pearl
    SpriteImage("ac_pearl_black", 0.5, 0.43)
    SpriteImage("sh_pearl", 0.4, 0.41)
    DefShModel("ac_pearl_black", "ac_pearl_black", "sh_pearl")
    DefAlias("ac_pearl_black-shine", "ac_pearl_black")
    
    -- Normal white pearl
    -- Use shadow from black pearl
    SpriteImage("ac_pearl_white", 0.5, 0.43)
    DefShModel("ac_pearl_white", "ac_pearl_white", "sh_pearl")
    DefAlias("ac_pearl_white-shine", "ac_pearl_white")
    
    -- Falling black pearl
    images = SpriteImages("ac_pearl_black_fall", 5, 0.5, 0.43)
    shadows = SpriteImages("sh_pearl_fall", 5, 0.4, 0.43)
    table.insert(images, "invisible")
    table.insert(shadows, "invisible")
    frames = {}
    for i=1,table.getn(images) do
        DefShModel("ac_pearl_black-fall"..(i-1), images[i], shadows[i])
        frames[i] = "ac_pearl_black-fall"..(i-1)
    end
    DefAnim("ac_pearl_black-fall", ComposeFrames(frames,{70,65,60,55,50,30}))
    DefAlias("ac_pearl_black-fallen", "invisible")
    
    -- Appearing / disappearing black pearl
    -- use the images from falling black pearl
    DefAnim("ac_pearl_black-appear", ReverseFrames(BuildFrames(frames, 25)))
    DefAnim("ac_pearl_black-disappear", BuildFrames(frames, 25))
    
    -- Falling white pearl
    -- Use shadows from falling black pearl
    images = SpriteImages("ac_pearl_white_fall", 5, 0.5, 0.43)
    table.insert(images, "invisible")
    frames = {}
    for i=1,table.getn(images) do
        DefShModel("ac_pearl_white-fall"..(i-1), images[i], shadows[i])
        frames[i] = "ac_pearl_white-fall"..(i-1)
    end
    DefAnim("ac_pearl_white-fall", ComposeFrames(frames,{70,65,60,55,50,30}))
    DefAlias("ac_pearl_white-fallen", "invisible")
    
    -- Appearing / disappearing white pearl
    -- use the images from falling white pearl
    DefAnim("ac_pearl_white-appear", ReverseFrames(BuildFrames(frames, 25)))
    DefAnim("ac_pearl_white-disappear", BuildFrames(frames, 25))
    
    -- Jumping black pearl
    images  = SpriteImages("ac_pearl_black_jump", 4)
    shadows = SpriteImages("sh_pearl_jump", 4, 0.4)
    frames  = {}
    for i=1,4 do
        DefShModel("sb-jump"..i, images[i], shadows[i])
        table.insert(frames, "sb-jump"..i)
    end
    DefAnim("ac_pearl_black-jump", PingPong(BuildFrames(frames, 70)))

    -- Jumping white pearl
    -- Use shadow from black pearl
    images  = SpriteImages("ac_pearl_white_jump", 4)
    frames  = {}
    for i=1,4 do
        DefShModel("sb-jump"..i, images[i], shadows[i])
        table.insert(frames, "sb-jump"..i)
    end
    DefAnim("ac_pearl_white-jump", PingPong(BuildFrames(frames, 70)))

    -- Sinking black pearl
    -- TODO: extra sink animation for pearls?
    DefAlias("ac_pearl_black-sink0", "ac_pearl_black_fall1")
    DefAlias("ac_pearl_black-sink1", "ac_pearl_black_fall2")
    DefAlias("ac_pearl_black-sink2", "ac_pearl_black_fall3")
    DefAlias("ac_pearl_black-sink3", "ac_pearl_black_fall3")
    DefAlias("ac_pearl_black-sink4", "ac_pearl_black_fall4")
    DefAlias("ac_pearl_black-sink5", "ac_pearl_black_fall4")
    DefAlias("ac_pearl_black-sink6", "ac_pearl_black_fall5")
    DefAlias("ac_pearl_black-sunk", "invisible")

    -- Sinking white pearl
    DefAlias("ac_pearl_white-sink0", "ac_pearl_white_fall1")
    DefAlias("ac_pearl_white-sink1", "ac_pearl_white_fall2")
    DefAlias("ac_pearl_white-sink2", "ac_pearl_white_fall3")
    DefAlias("ac_pearl_white-sink3", "ac_pearl_white_fall3")
    DefAlias("ac_pearl_white-sink4", "ac_pearl_white_fall4")
    DefAlias("ac_pearl_white-sink5", "ac_pearl_white_fall4")
    DefAlias("ac_pearl_white-sink6", "ac_pearl_white_fall5")
    DefAlias("ac_pearl_white-sunk", "invisible")

    -- Shattering black pearl
    Sprite({name="ac_pearl_black_shatter", nimages=5, framelen=60})
    DefAlias("ac_pearl_black-shatter", "ac_pearl_black_shatter")
    DefAlias("ac_pearl_black-shattered", "ac_pearl_black_shatter5")

    -- Shattering white pearl
    Sprite({name="ac_pearl_white_shatter", nimages=5, framelen=60})
    DefAlias("ac_pearl_white-shatter", "ac_pearl_white_shatter")
    DefAlias("ac_pearl_white-shattered", "ac_pearl_white_shatter5")     
end

-- ac-killerball --
do
    DefAlias("ac_killer_black", "ac_pearl_black")
    DefAlias("ac_killer_white", "ac_pearl_white")
end

----------------------------
-- not ball-shaped actors --
----------------------------

-- ac-bug --
do
    SpriteImage("ac_bug",0.5,0.44)
    DefShModel("ac_bug", "ac_bug", "sh_pearl")
end

-- ac-cannonball --
do
    DefAlias("ac_cannonball", "ac_marble_black-jump")
end

-- ac-horse --
do
    SpriteImage("ac_horse",0.5,0.18)
    SpriteImage("sh_horse", 0.4)
    DefShModel("ac_horse", "ac_horse", "sh_horse")
end

-- ac-rotor --
do
    local fg = SpriteImages("ac_rotor",9)
    local bg = SpriteImages("sh_rotor", 9, 0.4)

    frames = {}
    for i=1,9 do
        DefShModel("ac-rotor"..i, fg[i], bg[i])
        table.insert(frames, "ac-rotor"..i)
    end
    DefAnim("ac_rotor", BuildFrames(frames, 30), true)
end

-- ac-top --
do
    local fg = SpriteImages("ac_top", 9, 0.5, 0.3)
    local bg = SpriteImage("sh_top", 0.4, 0.3)
    
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
        {"fl_metal", 7},
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
        {"fl_wood_framed", 2},
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
    DefSubimages("fl_slope", {w=6, h=4, modelname="fl_slope"})
    DefAlias("fl_slope", "fl_slope21")
    DefAlias("fl_slope_pw", "fl_slope4")
    DefAlias("fl_slope_ps", "fl_slope1")
    DefAlias("fl_slope_pe", "fl_slope3")
    DefAlias("fl_slope_pn", "fl_slope2")
    DefAlias("fl_slope_inw", "fl_slope12")
    DefAlias("fl_slope_isw", "fl_slope11")
    DefAlias("fl_slope_ise", "fl_slope9")
    DefAlias("fl_slope_ine", "fl_slope10")
    DefAlias("fl_slope_onw", "fl_slope8")
    DefAlias("fl_slope_osw", "fl_slope6")
    DefAlias("fl_slope_ose", "fl_slope5")
    DefAlias("fl_slope_one", "fl_slope7")
    DefAlias("fl_slope_tw", "fl_slope21")
    DefAlias("fl_slope_ts", "fl_slope21")
    DefAlias("fl_slope_te", "fl_slope21")
    DefAlias("fl_slope_tn", "fl_slope21")
    DefAlias("fl_slope_twl", "fl_slope2")
    DefAlias("fl_slope_tsl", "fl_slope4")
    DefAlias("fl_slope_tel", "fl_slope1")
    DefAlias("fl_slope_tnl", "fl_slope3")
    DefAlias("fl_slope_twr", "fl_slope1")
    DefAlias("fl_slope_tsr", "fl_slope3")
    DefAlias("fl_slope_ter", "fl_slope2")
    DefAlias("fl_slope_tnr", "fl_slope4")
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
    DefSubimages("fl_wood", {w=1, h=4, modelname="fl-woodx"})
    DefRandFloor("fl_wood", {"fl-woodx1", "fl-woodx2", "fl-woodx3", "fl-woodx4"})
    DefRandFloor("fl_wood1", {"fl-woodx3", "fl-woodx4"})
    DefRandFloor("fl_wood2", {"fl-woodx1", "fl-woodx2"})
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
    DefAlias("fl_metal_7", "fl_metal7")
    DefAlias("fl_metal_7n", "fl_metal7")
    DefAlias("fl_metal_1_framed", "fl_metal1_framed")
    DefAlias("fl_metal_2_framed", "fl_metal2_framed")
    DefAlias("fl_metal_3_framed", "fl_metal3_framed")
    DefAlias("fl_metal_4_framed", "fl_metal4_framed")
    DefAlias("fl_metal_5_framed", "fl_metal5_framed")
    DefAlias("fl_metal_6_framed", "fl_metal6_framed")
    DefAlias("fl_metal_7_framed", "fl_metal7_framed")
    DefAlias("fl-space-force", "fl_space")
    DefAlias("fl_samba_h", "fl_samba1")
    DefAlias("fl_samba_v", "fl_samba2")
end

----------------------
-- Floor animations --
----------------------

-- bridges --
do
    DefImage("fl_bridge_gc_open")
    DefImage("fl_bridge_gc_closed")
    local namelist=DefSubimages("fl_bridge_gc", {h=10})
    local frames = BuildFrames(namelist,70)
    DefAnim("fl_bridge_gc_opening", ReverseFrames(frames))
    DefAnim("fl_bridge_gc_closing", frames)
end

do
    local bridge_o = DefImage("fl_bridge_bw_open")
    local bridge_c = DefImage("fl_bridge_bw_closed")

    for key,suff in ipairs({"bw", "bs", "be", "bn"}) do
        DefAlias("fl_bridge_"..suff.."_open", "fl_bridge_bw_open")
        DefAlias("fl_bridge_"..suff.."_closed", "fl_bridge_bw_closed")

        local namelist=DefSubimages("fl_bridge_"..suff, {h=7})
        table.insert(namelist, 1, bridge_c)
        table.insert(namelist, bridge_o)
        local frames = BuildFrames(namelist,70)
        DefAnim("fl_bridge_"..suff.."_opening", frames)
        DefAnim("fl_bridge_"..suff.."_closing", ReverseFrames(frames))
    end
end


-- scales
do
    local scales = DefSubimages("fl_scales", {h=2})
    
    display.DefineComposite("fl_scales_brick_released", "fl_brick", scales[1])
    display.DefineComposite("fl_scales_brick_pressed", "fl_brick", scales[2])

    display.DefineComposite("fl_scales_bridgewood_released", "fl_bridge_bw_closed", scales[1])
    display.DefineComposite("fl_scales_bridgewood_pressed", "fl_bridge_bw_closed", scales[2])

    display.DefineComposite("fl_scales_concrete_released", "fl_concrete", scales[1])
    display.DefineComposite("fl_scales_concrete_pressed", "fl_concrete", scales[2])

    display.DefineComposite("fl_scales_darkgray_released", "fl_darkgray", scales[1])
    display.DefineComposite("fl_scales_darkgray_pressed", "fl_darkgray", scales[2])

    display.DefineComposite("fl_scales_gray_released", "fl_gray", scales[1])
    display.DefineComposite("fl_scales_gray_pressed", "fl_gray", scales[2])

    display.DefineComposite("fl_scales_platinum_released", "fl_platinum", scales[1])
    display.DefineComposite("fl_scales_platinum_pressed", "fl_platinum", scales[2])
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
        "it_bottle_idle",
        "it_bottle_broken",
        "it_brush",
        "it_cherry",
        "it_coin_s",
        "it_coin_m",
        "it_coin_l",
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

    DefImage("it_brake", {filename="st_brake"})
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

--------------------------------------------------------------
-- Multiple-Image-Items, non animated, e.g. it-burnable-oil --
--------------------------------------------------------------

-- it_extinguisher --
do
    DefTiles("it_extinguisher", {"it_extinguisher_full", "it_extinguisher_medium", "it_extinguisher_empty"})
end

-- it_dummy
do
    DefTiles("it_dummy", {"it_dummy", "it_dummy_egg"})
end

-- it_key --
do
    DefTiles("it_key", {"it_key", "it_key_1", "it_key_2", "it_key_3", "it_key_4", "it_key_5", "it_key_6", "it_key_7", "it_key_8"})
end

-- Oil --
do
    DefSubimages("it_burnable_oil", {h=4})
end

-- Laserbeams --
do
    DefTiles("it_laser", {"it_laser_h", "it_laser_v", "it_laser_hv"})
end

-- it_pipe --
do
    DefTiles("it_pipe", {"it_pipe_e", "it_pipe_s", "it_pipe_es", "it_pipe_sw", "it_pipe_ew",
                         "it_pipe_w", "it_pipe_n", "it_pipe_ne", "it_pipe_nw", "it_pipe_ns"})
end

-- it-trigger --
do
    DefTiles("it_trigger", {"it_trigger_up","it_trigger_down"})
end

-- it_yinyang --
do
    DefTiles("it_yinyang", {"it_yinyang", "it_yangyin"})
end

-- it_strip --
do
    DefSubimages("it_strip", {modelname="it_strip", w=4, h=4, startindex=0})
end

----------------------------------------
-- Animated Items and Item Animations --
----------------------------------------

-- Burning black bomb --
do
    local images = DefSubimages("it_bomb_black_burning", {h=9})
    local frames = BuildFrames(images, 100)
    DefAnim("it_bomb_black_burning", frames)
end

-- Burning white bomb --
do
    local images = DefSubimages("it_bomb_white_burning", {h=9})
    local frames = BuildFrames(images, 100)
    DefAnim("it_bomb_white_burning", frames)
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

-- it_coffee --
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

-- it_death --
do
    local images = DefSubimages("it_death", {h=4})
    DefAlias("it_death", images[1])
    DefAnim("it_death_anim", BuildFrames(images, 100))
end

-- Explosion --
do
    DefAnimImages("it_explosion", {{"it_explosion", 50}})
end

-- it_magnet --
do
    DefImage("it_magnet_off")
    local images = DefSubimages("it_magnet_on", {h=5})
    local frames = BuildFrames(images, 100)
    DefAnim("it_magnet_on", frames, 1)
end

-- it_puller --
do
    local images = {"it_puller_n", "it_puller_e", "it_puller_s", "it_puller_w"}
    DefTiles("it_puller", images)
    local frames = BuildFrames(images, 100)
    DefAnim("it_puller_active", RepeatAnim(frames, 4), false)
end

-- it_seed --
do
    local images = DefSubimages("it_seed", {h=5})
    DefAlias("it_seed", "it_seed1")
    local frames = {
        "it_seed1", "it_seed2", "it_seed1", "it_seed3", "it_seed1", "it_seed2",
        "it_seed1", "it_seed4", "it_seed5", "it_seed4", "it_seed1",
        "it_seed4", "it_seed5",
    }
    DefAnim("it_seed_growing", BuildFrames(frames, 120))
end

-- it_sensor --
do
    DefTiles("it_sensor", {"it_sensor","it_sensor1"})
    local frames = BuildFrames({"it_sensor","it_sensor1"}, 300)
    DefAnim("it_sensor_hit", RepeatAnim(frames, 3), false)
    DefTiles("it_sensor_exit", {"it_sensor_exit","it_sensor_exit1"})
    local exitframes = BuildFrames({"it_sensor_exit","it_sensor_exit1"}, 300)
    DefAnim("it_sensor_exit_hit", RepeatAnim(exitframes, 3), false)
end

-- it_shogun --
do
    NewAnim("it_shogun_s", {img="it_shogun_s", h=3, speed=160, pingpong=1, loop=1})
    NewAnim("it_shogun_m", {img="it_shogun_m",   h=3, speed=160, pingpong=1, loop=1})
    NewAnim("it_shogun_l", {img="it_shogun_l",   h=3, speed=160, pingpong=1, loop=1})
end

-- it_springboard --
do
    local images = {"it_springboard1", "it_springboard2"}
    DefTiles ("it_springboard", images)
    DefAlias ("it_springboard", images[1])
    DefAnim("it_springboard_anim", BuildFrames(ReverseFrames(images),120))
end

-- it_vortex --
do
    local images = DefSubimages("it_vortex", {h=4})
    DefAlias("it_vortex_open", images[1])
    DefAlias("it_vortex_closed", images[4])

    DefAnim("it_vortex_opening", ReverseFrames(BuildFrames(images, 100)))
    DefAnim("it_vortex_closing", BuildFrames(images, 100))
end

-- it_wormhole --
do
    local images = DefSubimages("it_wormhole", {h=2})
    local frames = BuildFrames(images, 100)
    DefAnim("it_wormhole", frames, true)
    DefAlias("it_wormhole_off", "it_wormhole1")
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
    DefImage("sh_solid")
    DefImage("sh_round")
    DefImage("sh_rounded")
    DefImage("sh_grate_cross")
    DefImage("sh_grate_framed")
    DefImage("sh_portal_horse")
    DefImage("sh_rawglass")
    DefImage("sh_rawglass_quad")
    DefImage("sh_passage_frame")
    DefImage("sh_passage_slash")
    DefImage("sh_passage_cross")
    DefImage("sh_puzzle_hollow")
    DefImage("sh_brake")
    DefImage("sh_floating")
    DefImage("sh_jamb")
    DefImage("sh_beads")
    -- Note: Window shadows are defined where needed.
end

-------------------
-- simple stones --
-------------------
do
    DefStone("st_beads", "sh_beads")
    DefStone("st_passage_black_square", "sh_round")
    DefStone("st_passage_black_slash", "sh_passage_slash")
    DefStone("st_passage_black_cross", "sh_passage_cross")
    DefStone("st_passage_black_frame", "sh_passage_frame")
    DefStone("st_jamb_black", "sh_jamb")
    DefStone("st_plop_slate")
    DefStone("st_bluegray", "sh_round")
    DefStone("st_brake", "sh_brake")
    DefStone("st_brownie", "sh_round")
    DefStone("st_break_bug")
    DefStone("st_pinkbumps")
    DefStone("st_camouflage")
    DefStone("st_dummy")
    DefImage("st-easymode")
    DefStone("st_floppy_off", "sh_round")
    DefStone("st_floppy_on", "sh_round")
    DefStone("st_box_rock", "sh_round")
    DefStone("st_rawglass", "sh_rawglass")
    DefStone("st_rawglass_quad", "sh_rawglass_quad")
    DefStone("st_lightglass", "sh_rawglass")
    DefStone("st_darkglass", "sh_rawglass")
    DefStone("st_redfiber", "sh_rawglass")
    DefStone("st_grate_cross", "sh_grate_cross")
    DefStone("st_grate_framed", "sh_grate_framed")
    DefStone("st_portal_horse", "sh_portal_horse")
    DefStone("st_redbrown", "sh_round")
    DefStone("st_metal")
    DefStone("st_redrock", "sh_rounded")
    DefStone("st_granite", "sh_round")
    DefStone("st_darkgray", "sh_rounded")
    DefStone("st_plaster")
    DefStone("st_purplemarble")
    DefStone("st_tigris")
    DefStone("st_purplegray")
    DefStone("st_greengray")
    DefStone("st_concrete", "sh_round")
    DefStone("st_ancient", "sh_round")
    DefStone("st_brownmarble")
    DefStone("st_rubberband", "sh_round")
    DefStone("st_brownpyramide")
    DefStone("st_redmarble")
    DefStone("st_surprise", "sh_round")
    DefStone("st_passage_white_square", "sh_round")
    DefStone("st_passage_white_slash", "sh_passage_slash")
    DefStone("st_passage_white_cross", "sh_passage_cross")
    DefStone("st_passage_white_frame", "sh_passage_frame")
    DefStone("st_jamb_white", "sh_jamb")
    DefStone("st_woven")
    DefStone("st_yellow")
    DefStone("st_blur_straight", "sh_round")
    DefStone("st_blur_cross", "sh_round")
    DefStone("st_yinyang_inactive", "sh_round")
end

-- st_key --
do
    local images_on =  DefSubimages("st_key_on", {modelname="st_key_on", w=1, h=9})
    local images_off = DefSubimages("st_key_off", {modelname="st_key_off", w=1, h=9})
    DefRoundStone("st_key_on", images_on[1])
    DefRoundStone("st_key_off", images_off[1])
    for i=1,8 do
        DefRoundStone("st_key_"..i.."_on", images_on[i+1])
        DefRoundStone("st_key_"..i.."_off", images_off[i+1])
    end
end

---------------------
-- Animated stones --
---------------------

-- st_actorimpulse --
do
    namelist = DefSubimages("st_actorimpulse", {h=3})
    shnamelist = DefSubimages("sh_actorimpulse", {h=3,imgw=ShadowSize,imgh=ShadowSize})
    
    DefAnim("st-ai-fg", PingPong(BuildFrames(namelist, 30)))
    DefAnim("st-ai-sh", PingPong(BuildFrames(shnamelist, 30)))
    DefShModel("st_actorimpulse_anim", "st-ai-fg", "st-ai-sh")

    DefShModel("st_actorimpulse", namelist[1], shnamelist[1])
end

-- st_bluesand --
do
    DefSubimages("st_bluesand", {modelname="st_bluesandx",w=4,h=4})
    for i=0,15 do DefSolidStone("st_bluesand"..i, "st_bluesandx"..(i+1)) end
end

-- st_brick --
do
    DefSubimages("st_brick", {modelname="st_brickx",w=4,h=4})
    for i=0,15 do DefSolidStone("st_brick"..i, "st_brickx"..(i+1)) end
end

-- st_panel --
do
    DefSubimages("st_panel", {modelname="st_panelx",w=4,h=4})
    for i=0,15 do DefSolidStone("st_panel"..i, "st_panelx"..(i+1)) end
end

-- st_boulder --
do
    local img=DefSubimages("st_boulder", {w=4,h=3})
    local imgfall=DefSubimages("st_boulder_fall", {w=4,h=3})

    function def_boulder(orient, start)
        local animname="st_boulder"..orient.."a"
        local frames={img[start], img[start+1], img[start+2]}
        DefAnim(animname, BuildFrames(frames, 120), false)
        DefShModel("st_boulder_"..orient, animname, "sh_round")

        animname="st_boulder_"..orient.."_fall_anim"
        frames={imgfall[start],imgfall[start+1],imgfall[start+2]}
        DefAnim(animname, BuildFrames(frames, 120), false)
    end

    def_boulder("n",1)
    def_boulder("e",4)
    def_boulder("s",7)
    def_boulder("w",10)
end

-- st-break_*, breakable stones --
do
    DefSolidStoneWithAnim ("st_break_oxydc", 10, 50)
    DefSolidStoneWithAnim ("st_break_boulder", 10, 50)
    DefRoundStoneWithAnim ("st_break_white", 10, 50)
    DefRoundStoneWithAnim ("st_break_black", 10, 50)
    DefRoundStoneWithAnim ("st_break_plain", 10, 50)
end

-- st_chage_plus, st_charge_zero, st_charge_minus --
do
    function def_chargestone(basename)
        local images = DefSubimages(basename, {h=2})
        DefShModel(basename, images[1], "sh_rounded")
        DefAnim(basename.."-animfg", {{images[2], 140}})
        DefShModel(basename.."_anim", basename.."-animfg", "sh_rounded")
    end

    def_chargestone("st_charge_plus")
    def_chargestone("st_charge_minus")
    def_chargestone("st_charge_zero")
end

-- st_chess_black, st_chess_white --
do
    function make_chess(colour)
        local img1 = DefSubimages("st_chess"..colour, {h=5});
        --local img2 = DefSubimages("st_chess"..colour.."-capture", {h=5});
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

-- st_coinslot --
do
    DefStone("st_coinslot")
    local images = DefSubimages("st_coinslot_insert", {h=18})
    DefAnim("st_coinslot_anim", BuildFrames(images, 20))
    DefSolidStone("st_coinslot_insert", "st_coinslot_anim")
    DefSolidStone("st_coinslot_active", "st_coinslot_insert18")
end

-- st_disco --
do
    DefTiles("st_disco", {"st_disco0","st_disco1","st_disco2"})
end

-- st_death --
do
    DefRoundStoneWithAnim ("st_death", 3, 140)
    DefRoundStoneWithAnim ("st_death_light", 3, 140)
end

-- st_yinyang --
do
    DefRoundStoneWithAnim("st_yinyang", 6, 140)
end

-- st_dispenser --
do
    function make_dispenser(name)
        local images = DefSubimages(name, {h=7})
        DefRoundStone(name, name.."1")
        DefAnim(name.."_breaking", BuildFrames(images,50))
    end

    make_dispenser("st_dispenser_bombblack")
    make_dispenser("st_dispenser_dynamite")
    make_dispenser("st_dispenser_bombwhite")
    make_dispenser("st_dispenser_extralife")
end

-- st-flash --
do
    local images = DefSubimages("st_flash", {h=6})
    DefSolidStone("st_flash", "st_flash1")
    table.remove(images,1)
    local frames = BuildFrames(images,50)
    DefAnim("st_flash_anim", frames)
    DefSolidStone("st_flash-anim", "st_flash_anim")
end

-- st_fourswitch --
do
    DefSubimages("st_fourswitch",{h=4})
    DefShModel("st_fourswitch-n", "st_fourswitch1", "sh_round")
    DefShModel("st_fourswitch-e", "st_fourswitch2", "sh_round")
    DefShModel("st_fourswitch-s", "st_fourswitch3", "sh_round")
    DefShModel("st_fourswitch-w", "st_fourswitch4", "sh_round")
end

-- st_ice --
do
    DefStone("st_ice", "sh_rawglass")

    local images = DefSubimages("st_ice_break", {h=5})
    local frames = BuildFrames(images,50)
    DefAnim("st_ice_break-anim", frames)
    DefShModel("st_ice_breaking", "st_ice_break-anim", "sh_rawglass")

    images = DefSubimages("st_ice_melting", {h=10})
    frames = BuildFrames(images,200)
    DefAnim("st_ice_melt-anim", frames)
    DefShModel("st_ice_melting", "st_ice_melt-anim", "sh_rawglass")
end

-- st_knight --
do
    local images = DefSubimages("st_knight", {modelname="knight", h=5})
    for i=1, table.getn(images) do
        DefShModel("st_knight"..i, images[i], "sh_round")
    end
end

-- st_laser --
do
    local images = DefSubimages("st_laser", {w=4,h=9})
    local imagesn = {}
    local imagese = {}
    local imagess = {}
    local imagesw = {}
    for i=0,8 do
        imagesn[i] = images[1+i]
        imagese[i] = images[10+i]
        imagess[i] = images[19+i]
        imagesw[i] = images[28+i]
        -- slicing like in python would be nice here :-)
    end

    -- deactivated laser
    DefShModel("st-laser-n", imagesn[1], "sh_rounded")
    DefShModel("st-laser-e", imagese[1], "sh_rounded")
    DefShModel("st-laser-s", imagess[1], "sh_rounded")
    DefShModel("st-laser-w", imagesw[1], "sh_rounded")
    
    -- activated laser
    DefAnim("st-laseron-anim-n", BuildFrames(imagesn,100), true)
    DefShModel("st-laseron-n", "st-laseron-anim-n", "sh_rounded")
    DefAnim("st-laseron-anim-e", BuildFrames(imagese,100), true)                                                                                       
    DefShModel("st-laseron-e", "st-laseron-anim-e", "sh_rounded")
    DefAnim("st-laseron-anim-s", BuildFrames(imagess,100), true)                                                                                       
    DefShModel("st-laseron-s", "st-laseron-anim-s", "sh_rounded")
    DefAnim("st-laseron-anim-w", BuildFrames(imagesw,100), true)                                                                                       
    DefShModel("st-laseron-w", "st-laseron-anim-w", "sh_rounded")
end

-- st_lightpassenger --
do
    local img = DefSubimages("st_lightpassenger", {h=7})
    DefShModel("st_lightpassenger", img[1], "sh_rawglass")
    DefImage("st_lightpassenger_offx", {filename = "st_lightpassenger_off"})
    DefShModel("st_lightpassenger_off", "st_lightpassenger_offx", "sh_rawglass")
    DefAlias("st_lightpassenger_hidden", "st_darkglass")
    -- Blink animation
    local frames = {img[2], img[3], img[4], img[5], img[6], img[7]}
    DefAnim("st_lightpassenger_blink1", PingPong(BuildFrames(frames, 75)), true)
    DefShModel("st_lightpassenger_blink", "st_lightpassenger_blink1", "sh_rawglass")
    -- Break animation
    img = DefSubimages("st_lightpassenger_break", {h=7})
    local anim_v, anim_h, anim_hv = {}, {}, {}
    for j = 1, 7 do
        anim_v[j] = "st_lightpassenger_break_v"..j
        display.DefineComposite(anim_v[j], "it_laser_v", img[j])
        anim_h[j] = "st_lightpassenger_break_h"..j
        display.DefineComposite(anim_h[j], "it_laser_h", img[j])
        anim_hv[j] = "st_lightpassenger_break_hv"..j
        display.DefineComposite(anim_hv[j], "it_laser_hv", img[j])
    end
    DefAnim("st_lightpassenger_break_v", BuildFrames(anim_v, 50), false)
    DefAnim("st_lightpassenger_break_h", BuildFrames(anim_h, 50), false)
    DefAnim("st_lightpassenger_break_hv", BuildFrames(anim_hv, 50), false)
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

-- st_mail --
do
    local images = DefSubimages("st_mail", {h=4})
    DefSolidStone("st_mail_n", images[1])
    DefSolidStone("st_mail_e", images[2])
    DefSolidStone("st_mail_w", images[3])
    DefSolidStone("st_mail_s", images[4])
end

-- st_oneway, st_oneway_black, st_oneway_white --
do
    local model_names = {
        "st_oneway_e", "st_oneway_n", "st_oneway_s", "st_oneway_w",
        "st_oneway_black_e", "st_oneway_black_n", "st_oneway_black_s", "st_oneway_black_w",
        "st_oneway_white_e", "st_oneway_white_n", "st_oneway_white_s", "st_oneway_white_w"
    }
    DefSubimages("st_oneway", {modelname="st_onewayx",w=3,h=4})
    for i=1,12 do DefFloatingStone(model_names[i], "st_onewayx"..i) end
end

-- st_pull, st_swap --
do
    DefSubimages("st_pull",{modelname="st-pull-fg",h=5})
    DefSubimages("st_swap",{modelname="st-swap-fg",h=5})
    DefSubimages("sh_pull",{h=4, imgw=ShadowSize, imgh=ShadowSize})

    DefShModel("st_pull",   "st-pull-fg1", "sh_rawglass")
    DefShModel("st_pull_n", "st-pull-fg2", "sh_pull1")
    DefShModel("st_pull_s", "st-pull-fg3", "sh_pull2")
    DefShModel("st_pull_e", "st-pull-fg4", "sh_pull3")
    DefShModel("st_pull_w", "st-pull-fg5", "sh_pull4")

    DefShModel("st_swap",   "st-swap-fg1", "sh_round")
    DefShModel("st_swap_n", "st-swap-fg2", "sh_pull1")
    DefShModel("st_swap_s", "st-swap-fg3", "sh_pull2")
    DefShModel("st_swap_e", "st-swap-fg4", "sh_pull3")
    DefShModel("st_swap_w", "st-swap-fg5", "sh_pull4")
end

-- st-puzzle* --
do
    DefSubimages("st_puzzle_blue", {modelname="st_puzzlex",w=4,h=4,startindex=0})
    DefSubimages("st_puzzle_yellow", {modelname="st_puzzlex",w=4,h=4,startindex=16})
    DefSubimages("st_puzzle_blue_hollow", {modelname="st_puzzlex",w=4,h=4,startindex=32})
    DefSubimages("st_puzzle_yellow_hollow", {modelname="st_puzzlex",w=4,h=4,startindex=48})
    -- Full puzzels
    for i=0,31  do DefSolidStone("st_puzzle"..i, "st_puzzlex"..i) end
    -- Hollow puzzels
    for i=32,63  do DefShModel("st_puzzle"..i, "st_puzzlex"..i, "sh_puzzle_hollow") end
end

-- st_flat --
do
    local images = DefSubimages("st_flat", {h=4})
    -- flat stone
    DefShModel("st_flat", images[1], "sh_solid")
    -- flat stone fall anim
    DefAnim("st_flat_falling", BuildFrames(images, 100))
    -- st_flat_break
    local images = DefSubimages("st_flat_break", {h=6})
    DefAnim("flat_break-anim", BuildFrames(images, 50))
    DefSolidStone("st_flat_breaking", "flat_break-anim")
    DefAlias("st_plaster_breaking", "st_flat_breaking")
    -- cracked flat
    DefStone("st_flat_cracked")
    -- burnable flat
    DefStone("st_flat_fire")
end

-- st_rotator --
do
    local images = DefSubimages("st_rotator_ccw", {h=8})
    DefAnim("st_rotator_ccw_anim", BuildFrames(images, 70), false)
    DefRoundStone("st_rotator_ccw", "st_rotator_ccw_anim")
    local images = DefSubimages("st_rotator_cw", {h=8})
    DefAnim("st_rotator_cw_anim", BuildFrames(images, 70), false)
    DefRoundStone("st_rotator_cw", "st_rotator_cw_anim")
end

-- st_scissors --
do
    local images = DefSubimages("st_scissors", {h=2})
    DefRoundStone("st_scissors", images[1])
    DefAnim("st_scissors_snip_anim", {{images[2], 130}})
    DefShModel("st_scissors_snip", "st_scissors_snip_anim", "sh_round")
end

-- st-shogun* --
do
    DefImages{"sh_shogun_s","sh_shogun_m", "sh_shogun_l"}
    DefSubimages("st_shogun", {modelname="st-shogun-fg",h=7})

    DefShModel("st_shogun1", "st-shogun-fg1", "sh_shogun_s")
    DefShModel("st_shogun2", "st-shogun-fg2", "sh_shogun_m")
    DefShModel("st_shogun3", "st-shogun-fg3", "sh_shogun_s")
    DefShModel("st_shogun4", "st-shogun-fg4", "sh_shogun_l")
    DefShModel("st_shogun5", "st-shogun-fg5", "sh_shogun_s")
    DefShModel("st_shogun6", "st-shogun-fg6", "sh_shogun_m")
    DefShModel("st_shogun7", "st-shogun-fg7", "sh_shogun_s")
end

-- st_stoneimpulse --
--
-- Note: It's important that the duration of the closing animation
-- (anim2) is longer than the opening animation (anim1). Otherwise
-- impulse stones do not work properly!
do
    local images = DefSubimages("st_stoneimpulse", {h=6})
    DefRoundStone("st_stoneimpulse", images[1])
    DefRoundStone("st_stoneimpulse_steady", images[6])
    local frames = {}
    for i=1,4 do table.insert(frames, images[i]) end
    DefAnim("stoneimpulse_anim1", BuildFrames(frames, 55))
    DefRoundStone("st_stoneimpulse_anim1", "stoneimpulse_anim1")
    table.insert(frames, images[4]) -- add 1 frame to make closing anim longer!
    DefAnim("stoneimpulse_anim2", ReverseFrames(BuildFrames(frames, 55)))
    DefRoundStone("st_stoneimpulse_anim2", "stoneimpulse_anim2")
end

-- st_stoneimpulse_hollow --
do
    local images = DefSubimages("st_stoneimpulse_hollow", {h=4})
    local shadows = DefSubimages("sh_stoneimpulse_hollow", {h=4, imgw=ShadowSize, imgh=ShadowSize})

    local framesfg = {}
    local framesbg = {}
    for i=1,4 do
        table.insert(framesfg, images[i])
        table.insert(framesbg, shadows[i])
    end

    DefShModel("st_stoneimpulse_hollow", framesfg[1], framesbg[1])

    DefAnim("stoneimpulse-hollow-anim1-fg", BuildFrames(framesfg, 55))
    DefAnim("stoneimpulse-hollow-anim1-bg", BuildFrames(framesbg, 55))
    DefShModel("st_stoneimpulse_hollow_anim1", "stoneimpulse-hollow-anim1-fg", "stoneimpulse-hollow-anim1-bg")

    table.insert(framesfg, images[4]) -- add 1 frame to make closing anim longer!
    table.insert(framesbg, shadows[4]) -- add 1 frame to make closing anim longer!

    DefAnim("stoneimpulse-hollow-anim2-fg", ReverseFrames(BuildFrames(framesfg, 55)))
    DefAnim("stoneimpulse-hollow-anim2-bg", ReverseFrames(BuildFrames(framesbg, 55)))
    DefShModel("st_stoneimpulse_hollow_anim2", "stoneimpulse-hollow-anim2-fg", "stoneimpulse-hollow-anim2-bg")
end

do
    local images = DefSubimages("st_stoneimpulse_break", {h=8})
    DefAnim("st_stoneimpulse_breaking", BuildFrames(images,50))
end

-- st_switch, st_switch_black, st_switch_white --
do
    function mkswitch(modelname, basename)
        local n = DefSubimages(modelname, {h=3, modelname=basename.."-fg"})
        local f = BuildFrames(n, 60)
        DefRoundStone(modelname.."_off", n[1])
        DefRoundStone(modelname.."_on", n[3])
        DefAnim(basename.."_turnon", f)
        DefAnim(basename.."_turnoff", ReverseFrames(f))
        DefRoundStone(modelname.."_turnon", basename.."_turnon")
        DefRoundStone(modelname.."_turnoff", basename.."_turnoff")
    end
    mkswitch("st_switch", "switch")
    mkswitch("st_switch_black", "switch_black")
    mkswitch("st_switch_white", "switch_white")
end

-- st_timer --
do
    local images = DefSubimages("st_timer", {h=5})
    local timerframes = {"st_timer3", "st_timer2", "st_timer1", "st_timer2", "st_timer3", "st_timer4", "st_timer5", "st_timer4"}
    DefAnim("timer_anim", BuildFrames(timerframes, 120), 1)
    DefRoundStone("st_timer", "timer_anim")
    DefRoundStone("st_timeroff", "st_timer3")
end

-- st_monoflop, st_laserswitch --
do
    local images = DefSubimages("st_monoflop", {h=2})
    DefRoundStone("st_monoflop", "st_monoflop1")
    DefAnim("st_monoflop_anim", BuildFrames (images, 180), 1)
    DefShModel("st_monoflop_anim", "st_monoflop_anim", "sh_round")
end

-- laserswitch --
do
    DefStone("st_laserswitch0", "sh_round", {filename="st_oxydb"})
    local images = DefSubimages("st_quake", {h=4})
    local frames = BuildFrames(RepeatAnim(PingPong(images)), 50 )
    DefAnim("laserswitch-blink", frames, true)
    DefShModel("st_laserswitch1", "laserswitch-blink", "sh_round")
end

-- st_turnstile --
do
    local images = DefSubimages("st_turnstile", {h=2})
    DefAnim("turnstile_anim", ReverseFrames(BuildFrames(images, 30)))
    DefSolidStone("st_turnstile", images[1])
    DefSolidStone("st_turnstile_anim", "turnstile_anim")
end

-- st-turnstle, arms --
DefAlias("st_turnstile_e", "st_puzzle1")
DefAlias("st_turnstile_w", "st_puzzle4")
DefAlias("st_turnstile_s", "st_puzzle8")
DefAlias("st_turnstile_n", "st_puzzle2")

-- st-turnstile-green --
do
    local images = DefSubimages("st_turnstile_green", {h=2})
    DefAnim("turnstile_green_anim", ReverseFrames(BuildFrames(images, 30)))
    DefSolidStone("st_turnstile_green", images[1])
    DefSolidStone("st_turnstile_green_anim", "turnstile_green_anim")
end

-- growing stones, st_box, st_flhay, greenbrown seed, st-volcano, st-blocker --
do
    local shadows=DefSubimages("sh_round_growing", {h=3,imgw=ShadowSize,imgh=ShadowSize})

    -- Wooden (box) stones --
    do
        DefSubimages("st_box_wood", {modelname="st_box_wood_fg", h=2})
        DefShModel("st_box_wood1", "st_box_wood_fg1", "sh_round")
        DefShModel("st_box_wood2", "st_box_wood_fg2", "sh_round")

        local images=DefSubimages("st_box_wood_growing", {h=3})
        DefAnim("st_box_wood_growing_fg", BuildFrames(images, 130))
        DefAnim("st_box_wood_growing_bg", BuildFrames(shadows, 130))
        DefShModel("st_box_wood_growing", "st_box_wood_growing_fg", "st_box_wood_growing_bg")
    end

    -- Greenbrown stone --
    do
        local images = DefSubimages("st_greenbrown", {h=4})
        -- stone
        DefShModel("st_greenbrown", images[1], "sh_round")
        -- grow anim
        table.remove(images, 1)
        DefAnim("st_greenbrown_growing_fg", ReverseFrames(BuildFrames(images, 130)))
        DefAnim("st_greenbrown_growing_bg", BuildFrames(shadows, 130))
        DefShModel("st_greenbrown_growing", "st_greenbrown_growing_fg", "st_greenbrown_growing_bg")
    end

    -- st_box_hay --
    do
        local images = DefSubimages("st_box_hay", {h=4})
        -- stone
        DefShModel("st_box_hay", images[1], "sh_round")
        -- grow anim
        table.remove(images, 1)
        DefAnim("st_box_hay_growing_fg", ReverseFrames(BuildFrames(images, 130)))
        DefAnim("st_box_hay_growing_bg", BuildFrames(shadows, 130))
        DefShModel("st_box_hay_growing", "st_box_hay_growing_fg", "st_box_hay_growing_bg")
    end

    -- Volcano growing --
    do
        local images = DefSubimages("st_volcano_growing", {h=3})
        DefAnim("volcano-growing-fg", BuildFrames(images, 80))
        DefAnim("volcano-growing-bg", BuildFrames(shadows, 80))
        DefShModel("st_volcano_growing", "volcano-growing-fg", "volcano-growing-bg")
    end

    -- Blocker stone --
    do
        local images = DefSubimages("st_blocker", {h=4})
        -- blocker stone
        DefShModel("st_blocker", images[1], "sh_round")
        -- blocker item
        DefAlias("it_blocker", images[4])
        -- open/close animation
        frames = {}
        for i=4,2,-1 do
            table.insert(frames, "st_blocker"..i)
        end
        DefAnim("blocker_growing_fg", BuildFrames(frames, 60))
        DefAnim("blocker_growing_bg", BuildFrames(shadows, 60))
        DefAnim("blocker_shrinking_fg", ReverseFrames(BuildFrames(frames, 60)))
        DefAnim("blocker_shrinking_bg", ReverseFrames(BuildFrames(shadows, 60)))
        DefShModel("st_blocker_shrinking", "blocker_shrinking_fg", "blocker_shrinking_bg");
        DefShModel("st_blocker_growing", "blocker_growing_fg", "blocker_growing_bg");
    end
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

    -- Some images we will need later on
    local colordots = DefSubimages("st_oxyd_colordots", {w=4,h=4})
    local questmark = DefImage("st_oxyd_questmark")
    local blink_ovals = DefSubimages("st_oxyd_blink", {w=1,h=5})
    local quake_spot = DefSubimages("st_oxyd_quake", {w=1,h=4})
    local shuffle_spot = DefSubimages("st_oxyd_shuffle", {w=1,h=8})
    local oxyde = DefImage("st_oxyde")

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
        a="sh_round",
        b="sh_round",
        c="sh_solid",
        d="sh_solid",
    }
    local shadow_open = {
        a="sh_round",
        b="sh_round",
        c="sh_solid",
        d="sh_round",
    }
    local fopening = {
        a = BuildFrames(DefSubimages("st_oxyda_opening", {h=10}), 35),
        b = BuildFrames(DefSubimages("st_oxydb_opening", {h=14}), 25),
        c = BuildFrames(DefSubimages("st_oxydc_opening", {h=5}), 70),
        d = BuildFrames(DefSubimages("st_oxydd_opening", {h=10}), 35),
    }
    local fclosing = {
        a = ReverseFrames(fopening["a"]),
        b = ReverseFrames(fopening["b"]),
        c = ReverseFrames(fopening["c"]),
        d = ReverseFrames(fopening["d"]),
    }
    
    -- The open/close animation of oxyd stones
    function mkopenclose(flavor, color)
        local name = "st_oxyd" .. flavor .. color
        local fadein = "oxyd"..flavor.."_fadein"
        local fadeout= "oxyd"..flavor.."_fadeout"
        -- Magic numbers for pseudo oxyds which need model number "-3" and "-4"
        -- but are subimage #15 and #16
        if color < 0 then
            color = color +18
        end
        DefMultipleComposite(name.."_base", {baseimg[flavor], colordots[color+1]})
        DefMultipleComposite(name.."_opening_fg", {name.."_base", fadein})
        DefMultipleComposite(name.."_closing_fg", {name.."_base", fadeout})
        DefShModel (name.."_opening", name.."_opening_fg", shadow[flavor])
        DefShModel (name.."_closing", name.."_closing_fg", shadow[flavor])
    end
    
    -- The blinking question mark animation of single open oxyds
    function mkblink(flavor, color)
        local name = "st_oxyd"..flavor..color.."_blink"
        DefMultipleComposite(name..1, {baseimg[flavor], colordots[color+1], questmark})
        DefMultipleComposite(name..2, {baseimg[flavor], colordots[color+1]})
        DefAnim(name.."_anim", ComposeFrames({name..1,name..2}, {800, 1200}), true)
        DefShModel(name, name.."_anim", shadow_open[flavor])
    end
    
    -- The animation of pairwise open oxyds
    function mkopened(flavor, color)
        local name = "st_oxyd" .. flavor .. color .. "_open"
        local names = {}
        for i=1,5 do
            names[i] = name .. i
            DefMultipleComposite(names[i], {baseimg[flavor], colordots[color+1], blink_ovals[i]})
        end
        DefAnim(name.."_anim", PingPong(BuildFrames(names, 100)), true)
        DefShModel(name, name.."_anim", shadow_open[flavor])
    end
    
    -- The "open" state animation of the pseudo oxyds "quake" and "shuffle"
    function mkpseudo(flavor, color)
        local name = "st_oxyd" .. flavor .. "_pseudo" .. color
        local names = {}        
        if (color == -3) then
            for i=1,4 do
                names[i] = name .. i
                DefMultipleComposite(names[i], {baseimg[flavor], quake_spot[i]})
            end
            frames = RepeatAnim(PingPong(BuildFrames(names, 50)), 2)
        elseif (color == -4) then
            for i=1,8 do
                names[i] = name .. i
                DefMultipleComposite(names[i], {baseimg[flavor], shuffle_spot[i]})
            end    
            frames = RepeatAnim(BuildFrames(names, 100),2)
        end
        DefAnim(name.."_anim", frames, false)
        DefShModel(name, name.."_anim", shadow[flavor])
    end

    -- Now really make the oxyds, calling the functions defined above
    function mkoxyd(flavor)
        DefStone("st_oxyd"..flavor, shadow[flavor])
        DefShModel("st_fake_oxyd"..flavor, "st_oxyd"..flavor, shadow[flavor])
        local img = DefImage("st_oxyd"..flavor.."_open")
        DefShModel("st_fake_oxyd"..flavor.."_open", img, shadow[flavor])
        local fadein = "oxyd"..flavor.."_fadein"
        local fadeout= "oxyd"..flavor.."_fadeout"
        DefAnim(fadein, fopening[flavor])
        DefAnim(fadeout, fclosing[flavor])
        -- The regular oxyds are position 1 to 12 in the
        -- colordots image
        for color = 0, num_colors do
            mkopenclose(flavor, color)
            mkblink(flavor, color)
            mkopened(flavor, color)
        end
        -- Position 13, 14 are currently empty
        -- The shuffle and quake oxyds are number 15
        -- and 16 in colordots image.
        -- BUT they require modelnames with "-3" and "-4"
        mkopenclose(flavor, -3)
        mkpseudo(flavor, -3)
        mkopenclose(flavor, -4)
        mkpseudo(flavor, -4)
    end

    -- Make all the models and animations for the oxyd flavors a,b,c,d
    mkoxyd("a")
    mkoxyd("b")
    mkoxyd("c")
    mkoxyd("d")
    
    -- And now for flavor 'e'
    for color = 0, num_colors - 1 do
        DefMultipleComposite("st_oxyde"..color.."_peep", {"st_oxydb_open", colordots[color+1], oxyde})
        DefShModel("st_oxyde"..color, "st_oxyde"..color.."_peep", "sh_round")
        DefAlias("st_oxyde"..color.."_opening", "st_oxydb"..color.."_opening")
        DefAlias("st_oxyde"..color.."_closing", "st_oxydb"..color.."_closing")
        DefAlias("st_oxyde"..color.."_blink", "st_oxydb"..color.."_blink")
        DefAlias("st_oxyde"..color.."_open", "st_oxydb"..color.."_open")
    end
    DefMultipleComposite("st_oxyde-3_peep", {"st_oxydb_open", colordots[16], oxyde})
    DefShModel("st_oxyde-3", "st_oxyde-3_peep", "sh_round")
    DefMultipleComposite("st_oxyde-4_peep", {"st_oxydb_open", colordots[15], oxyde})
    DefShModel("st_oxyde-4", "st_oxyde-4_peep", "sh_round")
    DefAlias("st_oxyde", "st_oxydb")
    DefAlias("st_fake_oxyde", "st_oxydb")
    DefAlias("st_fake_oxyde_open", "st_fake_oxydb_open")
    DefAlias("st_oxyde-3_opening", "st_oxydb-3_opening")
    DefAlias("st_oxyde_pseudo-3", "st_oxydb_pseudo-3")
    DefAlias("st_oxyde-3_closing", "st_oxydb-3_closing")
    DefAlias("st_oxyde-4_opening", "st_oxydb-4_opening")
    DefAlias("st_oxyde_pseudo-4", "st_oxydb_pseudo-4")
    DefAlias("st_oxyde-4_closing", "st_oxydb-4_closing")
end

-- st_magic --
do
    DefAlias("st_magic_brick", "st_brick0")
    DefAlias("st_magic_oxyda", "st_oxyda")
    DefAlias("st_magic_oxydc", "st_oxydc")
end

-- st_door_d --
do
    local f,img,sh

    img=DefSubimages("st_door_d_ns", {h=7})
    sh=DefSubimages("sh_doorh", {h=7,imgw=ShadowSize,imgh=ShadowSize})
    DefShModel("st_door_d_ns_open", img[1], sh[1])
    DefShModel("st_door_d_ns_closed", img[7], sh[7])
    DefAnim("doorh-opening-fg", ReverseFrames(BuildFrames(img, 60)))
    DefAnim("doorh-opening-bg", ReverseFrames(BuildFrames(sh, 60)))
    DefShModel("st_door_d_ns_opening", "doorh-opening-fg", "doorh-opening-bg")
    DefAnim("doorh-closing-fg", BuildFrames(img, 60))
    DefAnim("doorh-closing-bg", BuildFrames(sh, 60))
    DefShModel("st_door_d_ns_closing", "doorh-closing-fg", "doorh-closing-bg")

    img=DefSubimages("st_door_d_ew", {w=7})
    sh=DefSubimages("sh_doorv", {w=7,imgw=ShadowSize,imgh=ShadowSize})
    DefShModel("st_door_d_ew_open", img[1], sh[1])
    DefShModel("st_door_d_ew_closed", img[7], sh[7])
    DefAnim("doorv-opening-fg", ReverseFrames(BuildFrames(img, 60)))
    DefAnim("doorv-opening-bg", ReverseFrames(BuildFrames(sh, 60)))
    DefShModel("st_door_d_ew_opening", "doorv-opening-fg", "doorv-opening-bg")
    DefAnim("doorv-closing-fg", BuildFrames(img, 60))
    DefAnim("doorv-closing-bg", BuildFrames(sh, 60))
    DefShModel("st_door_d_ew_closing", "doorv-closing-fg", "doorv-closing-bg")
end

-- st_door_a --
do
    DefAlias("st_door_a_open", "st_grate_cross")
    DefAlias("st_door_a_closed", "st_oxyda")
    local f = BuildFrames({"st_door_a_closed", "st_door_a_open"},60)
    DefAnim("st_door_a_opening", f)
    DefAnim("st_door_a_closing", ReverseFrames(f))
end

-- st_door_b --
do
    DefAlias("st_door_b_open", "invisible")
    DefAlias("st_door_b_closed", "st_flat")
    local img=DefSubimages("st_door_b", {modelname="doorb", h=8})
    local f = BuildFrames(img,60)
    DefAnim("st_door_b_opening", f)
    DefAnim("st_door_b_closing", ReverseFrames(f))
end

-- st_door_c --
do
    DefAlias("st_door_c_open", "st_grate_framed")
    DefAlias("st_door_c_closed", "st_flat")
    frames=BuildFrames({"st_door_c_closed","st_door_c_open"},60)
    DefAnim("st_door_c_opening", frames)
    DefAnim("st_door_c_closing", ReverseFrames(frames))
end

-- st_quake --
do
    DefStone("st_quake", "sh_round", {filename="st_oxydb"})
    local images = DefSubimages("st_quake", {h=4})
    local frames = BuildFrames(RepeatAnim(PingPong(images)), 50)
    DefAnim("quaking", frames)
    DefShModel("st_quaking", "quaking", "sh_round")
    local namelist = DefSubimages("st_quake_break",{h=6})
    DefAnim("quakebreak_anim", BuildFrames(namelist,50))
    DefRoundStone("st_quakebreak_anim", "quakebreak_anim")
end

-- st-spitter --
do
    local images = DefSubimages("st_spitter_idle", {h=6})
    local frames = BuildFrames(images,150)
    DefAnim("st_spitter_active", frames, true)
    DefShModel("st_spitter_active", "st_spitter_active", "sh_round")
    DefShModel("st_spitter_idle", "st_spitter_idle1",  "sh_round")

    local images = DefSubimages("st_spitter_spitting", {h=6})
    local frames = BuildFrames(images,175)
    DefAnim("st_spitter_spitting", frames)
    DefShModel("st_spitter_spitting", "st_spitter_spitting", "sh_round")

    local images = DefSubimages("st_spitter_loading", {h=6})
    local frames = BuildFrames(images,100)
    frames[6][2] = 50
    DefAnim("st_spitter_loading", frames)
    DefShModel("st_spitter_loading", "st_spitter_loading", "sh_round")

    local images = DefSubimages("st_spitter_breaking", {h=6})
    local frames = BuildFrames(images,100)
    DefAnim("st_spitter_breaking", frames)
    DefShModel("st_spitter_breaking", "st_spitter_breaking", "sh_round")

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
--
-- The models are stored in the file from top to bottom as north, east, south, west
-- in each column, therefore we need the mapping (i+1)%4+1
do
    local mst = DefSubimages("st_mirror_static", {w=5,h=4})
    local mmo = DefSubimages("st_mirror_movable", {w=5,h=4})
    local names_st = {"st-3mirror-so", "st-3mirror-st", "st-pmirror-so", "st-pmirror-st", "st-pmirror-sf"}
    local names_mo = {"st-3mirror-mo", "st-3mirror-mt", "st-pmirror-mo", "st-pmirror-mt", "st-pmirror-mf"}
    -- static mirrors
    for j=0,4 do
        for i=1,4 do
            DefShModel(names_st[j+1]..((i+1)%4+1), mst[j*4+i], "sh_rounded")
        end
    end
    -- movable mirrors
    for j=0,4 do
        for i=1,4 do
            DefShModel(names_mo[j+1]..((i+1)%4+1), mmo[j*4+i], "sh_rounded")
        end
    end
end

--------------------------
-- Simple stone aliases --
--------------------------
do
    DefAlias("st_break_bug-anim", "st_flat_breaking")
    DefAlias("st-glass_move", "st-glass");
    DefAlias("st_break_laser", "st_flat")
    DefAlias("st_break_laser-anim", "st_flat_breaking")
    DefAlias("st_blur_magic", "st_blur_cross")
end

-- Invisible stones --
do
    DefAlias("st-actorimpulse_invisible", "invisible")
    DefAlias("st_death_invisible", "invisible")
    DefAlias("st_invisible", "invisible")
end

-- Barrier stone --
do
    local items = {"it_bag", "it_banana", "it_bomb", "it_bomb_black", "it_bomb_white",
                   "it_bottle", "it_brake",
                   "it_brush", "it_cherry", "it_coffee", "it_coin", "it_coin_s", "it_coin_m",
                   "it_coin_l", "it_document", "it_drop", "it_dynamite", "it_extinguisher",
                   "it_extinguisher_empty", "it_extinguisher_full", "it_extinguisher_medium",
                   "it_extralife", "it_flag", "it_flag_black", "it_flag_white",
                   "it_floppy", "it_glasses", "it_glasses_broken", "it_hammer", "it_key",
                   "it_magicwand", "it_pencil", "it_pin", "it_pipe", "it_pipe_w", "it_pipe_w",
                   "it_pipe_sw", "it_pipe_e", "it_pipe_ew", "it_pipe_es",
                   "it_pipe_n", "it_pipe_nw", "it_pipe_ns", "it_pipe_ne",
                   "it_puller", "it_puller_w", "it_puller_s", "it_puller_e", "it_puller_n",
                   "it_ring", "it_rubberband", "it_seed", "it_seed_wood", "it_seed_hay",
                   "it_seed_greenbrown", "it_seed_volcano",
                   "it_spade", "it_spoon", "it_spring", "it_spring_keep", "it_spring_drop",
                   "it_surprise", "it_sword", "it_umbrella",
                   "it_wrench", "it_yinyang"}
    local stonebase = DefImage("st_barrier")
    for _, it in pairs(items) do
        display.DefineComposite("st_barrier_"..it.."_fg", stonebase, it)
        DefShModel("st_barrier_"..it, "st_barrier_"..it.."_fg", "sh_solid")
    end
    display.DefineComposite("st_barrier_all_fg", stonebase, "it_dummy")
    DefShModel("st_barrier_all", "st_barrier_all_fg", "sh_solid")
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
