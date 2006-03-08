-- Per.Oxyd MP #6, a level for Enigma
-- Copyright (C) 2006 Raoul
-- Licensed under GPL v2.0 or above 
-- Per.Oxyd Clone

-- GENERAL --
Require("levels/lib/libpuzzle.lua")
levelw = 39
levelh = 21
create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
display.SetFollowMode(display.FOLLOW_SCROLLING)

-- DESIGN --
boden="fl-concrete"
wand="st-metal"

-- FLOOR --
fill_floor(boden, 0,0,levelw,levelh)

-- STONES --
--walls
draw_border(wand)
draw_stones(wand,{19,1},{0,1},5)
draw_stones(wand,{19,15},{0,1},5)

draw_stones(wand,{1,10},{1,0},12)
draw_stones(wand,{26,10},{1,0},12)


draw_stones(wand,{15,6},{1,0},9)
draw_stones(wand,{15,14},{1,0},9)

draw_stones(wand,{13,8},{0,1},5)
draw_stones(wand,{25,8},{0,1},5)

set_stone(wand,13,6)
set_stone(wand,13,14)
set_stone(wand,25,6)
set_stone(wand,25,14)

draw_stones(wand,{16,10},{1,0},7)
set_stone(wand,19,9)
set_stone(wand,19,11)

--colorstones:
fill_stones("st-black1",1,1,3,3)
fill_stones("st-white1",1,4,3,3)
fill_stones("st-black1",1,7,3,3)
fill_stones("st-white1",1,11,3,3)
fill_stones("st-black1",1,14,3,3)
fill_stones("st-white1",1,17,3,3)

fill_stones("st-white1",35,1,3,3)
fill_stones("st-black1",35,4,3,3)
fill_stones("st-white1",35,7,3,3)
fill_stones("st-black1",35,11,3,3)
fill_stones("st-white1",35,14,3,3)
fill_stones("st-black1",35,17,3,3)

--puzzle
matr={{1,1,1,1,1,1,1,1,1,1},{1,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,1},{1,1,1,1,1,1,1,1,1,1}}

matrix2places(matr,10,6,14,8)
which_piece(matr,10,6)
draw_direct(anzteile,locs,teile,"")

-- ACTORS --
set_actor("ac-blackball", 11.5,5.5, {player=0})
set_actor("ac-whiteball", 11.5,15.5, {player=1})
set_item("it-yinyang", 11,5)
set_item("it-yinyang", 11,15)

-- OXYD --
oxyd(2,2)
oxyd(2,5)
oxyd(2,8)
oxyd(2,12)
oxyd(2,15)
oxyd(2,18)
oxyd(36,2)
oxyd(36,5)
oxyd(36,8)
oxyd(36,12)
oxyd(36,15)
oxyd(36,18)
oxyd_default_flavor = "a"
oxyd_shuffle()









