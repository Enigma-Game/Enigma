-- Oxyd 2 Player Nr 33, a Oxyd-Clone for Enigma
-- Copyright (C) 2005,2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 20
levelh = 13
create_world(levelw, levelh)
enigma.ConserveLevel = FALSE

Require("levels/lib/libpuzzle.lua")

--DESIGN--
boden="fl-rock"
wand="st-rock1"

-- FLOOR --
fill_floor("fl-abyss", 0,0,20,13)

fill_floor(boden,0,1,4,11)
fill_floor(boden,4,2,3,9)
fill_floor(boden,7,3,3,3)
fill_floor(boden,8,6,3,4)
fill_floor(boden,11,7,2,3)
fill_floor(boden,13,2,3,9)
fill_floor(boden,16,1,4,11)

-- STONES --
draw_stones(wand,{0,1},{1,0},4)
draw_stones(wand,{3,2},{1,0},4)
draw_stones(wand,{6,3},{1,0},8)
draw_stones(wand,{13,2},{1,0},4)
draw_stones(wand,{16,1},{1,0},4)

draw_stones(wand,{0,11},{1,0},4)
draw_stones(wand,{3,10},{1,0},4)
draw_stones(wand,{6,5},{1,0},3)
draw_stones(wand,{8,9},{1,0},6)
draw_stones(wand,{13,10},{1,0},4)
draw_stones(wand,{16,11},{1,0},4)

draw_stones(wand,{0,3},{0,1},7)
draw_stones(wand,{3,4},{0,1},5)
draw_stones(wand,{6,6},{0,1},4)
draw_stones(wand,{8,6},{0,1},3)
draw_stones(wand,{10,6},{0,1},1)
draw_stones(wand,{13,4},{0,1},3)
draw_stones(wand,{16,4},{0,1},5)
draw_stones(wand,{19,3},{0,1},7)

draw_stones(wand,{10,5},{1,0},3)
draw_stones(wand,{10,7},{1,0},4)

set_stone("st-oneway_black", 3,3, {orientation=enigma.WEST})
set_stone("st-oneway_black", 3,9, {orientation=enigma.WEST})
set_stone("st-oneway_white", 16,3, {orientation=enigma.EAST})
set_stone("st-oneway_white", 16,9, {orientation=enigma.EAST})

-- ACTORS --
set_item("it-yinyang", 15,6)
set_item("it-yinyang", 5,6)
set_actor("ac-blackball", 15.5, 6.5, {player=0})
set_actor("ac-whiteball",  5.5, 6.5, {player=1})

-- OXYD --
oxyd(0,2)
oxyd(0,10)
oxyd(19,2)
oxyd(19,10)
oxyd_default_flavor = "a"
oxyd_shuffle()

puzzle({{1,1,1,1},{1,0,0,0},{1,0,0,0},{1,0,0,0}},9,4,"red")













