--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "a"
draw_border("st-rock1")
fill_floor("fl-rough")

oxyd( 11, 1)
oxyd( 18, 1)
oxyd( 11, 11)
oxyd( 18, 11)

fill_floor("fl-water", 10,1, 1 ,4)
fill_floor("fl-water", 10,8, 1 ,4)
fill_floor("fl-water", 10,5, 3 ,1)
fill_floor("fl-water", 10,7, 3 ,1)
fill_floor("fl-space", 10,6, 7 ,1)
fill_floor("fl-space", 17,4, 1 ,5)
draw_stones("st-grate2", {10,1}, {0,1},4)
draw_stones("st-grate2", {10,8}, {0,1},4)
draw_stones("st-thief", {18,4}, {0,1},5)
draw_stones("st-thief", {16,4}, {0,1},2)
draw_stones("st-thief", {16,7}, {0,1},2)
draw_stones("st-wood", {13,5}, {1,0},1)
draw_stones("st-wood", {13,7}, {1,0},1)
draw_stones("st-thief", {14,5}, {1,0},2)
draw_stones("st-thief", {14,7}, {1,0},2)
draw_stones("st-grate2", {11,4}, {1,0},5)
draw_stones("st-grate2", {11,8}, {1,0},5)
draw_stones("st-rock1", {2,2},{1,0},5)
draw_stones("st-rock1", {2,10},{1,0},5)

draw_stones("st-rock1", {1,5},{1,0},6)
draw_stones("st-rock1", {1,7},{1,0},6)

set_stone("st-oneway-n", 17,4)
set_stone("st-oneway-s", 17,8)
set_stones("st-bombs", {{1,4},{1,8}})

draw_stones("st-stone_break", {10,5}, {1,0},3)
draw_stones("st-stone_break", {10,7}, {1,0},3)

hammer(5,11)
dynamite(8,9)
dynamite(5,1)
set_item("it-blackbomb",1,3)
set_item("it-blackbomb",1,9)

doorh(1,2, {name="door1"})
set_stone("st-switch", 6,1, {action="openclose", target="door1"})
set_item("it-trigger", 4,3, {invisible=1, action="close", target="door1"})
set_item("it-trigger", 4,4, {invisible=1, action="close", target="door1"})

doorh(1,10, {name="door2"})
set_stone("st-switch", 6,11, {action="openclose", target="door2"})
set_item("it-trigger", 4,8, {invisible=1, action="close", target="door2"})
set_item("it-trigger", 4,9, {invisible=1, action="close", target="door2"})

set_stone("st-coinslot", 9,12, {action="onoff", target="laser"})
set_attrib(laser(9,0, FALSE, SOUTH), "name", "laser")
set_item("it-coin2", 3,6)
mirror3(7,6,TRUE,FALSE, 4)

set_actor("ac-blackball", 9.5, 4.5)
