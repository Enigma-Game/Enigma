-- 
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 58
levelh = 13

create_world(levelw, levelh)
draw_border("st-rock4")
fill_floor("fl-samba")

oxyd_default_flavor = "c"

oxyd ( 1,11)
oxyd ( 2, 5)
oxyd ( 9, 1)
oxyd (15, 1)
oxyd (42,11)
oxyd (48,11)
oxyd (55, 7)
oxyd (56, 1)

oxyd_shuffle()

fill_floor("fl-abyss", 23, 4, 12, 5)
fill_floor("fl-abyss", 20, 6,  3, 1)
fill_floor("fl-abyss", 35, 6,  3, 1)

draw_stones( "st-rock4", { 1,10}, {1,0}, 5)
draw_stones( "st-rock4", { 5, 9}, {1,0}, 5)
draw_stones( "st-rock4", { 9, 8}, {1,0}, 6)
draw_stones( "st-rock4", {14, 7}, {1,0},10)
draw_stones( "st-rock4", {23, 8}, {1,0}, 4)
draw_stones( "st-rock4", {26, 9}, {1,0}, 6)
draw_stones( "st-rock4", {31, 8}, {1,0}, 4)
draw_stones( "st-rock4", {34, 7}, {1,0}, 5)
draw_stones( "st-rock4", {19, 5}, {1,0}, 5)
draw_stones( "st-rock4", {23, 4}, {1,0}, 4)
draw_stones( "st-rock4", {26, 3}, {1,0}, 6)
draw_stones( "st-rock4", {31, 4}, {1,0}, 4)
draw_stones( "st-rock4", {34, 5}, {1,0},10)
draw_stones( "st-rock4", {43, 4}, {1,0}, 6)
draw_stones( "st-rock4", {48, 3}, {1,0}, 5)
draw_stones( "st-rock4", {52, 2}, {1,0}, 5)
draw_stones( "st-rock4", {19, 6}, {1,0}, 1)
draw_stones( "st-rock4", {38, 6}, {1,0}, 1)

set_item("it-yinyang", 25, 9)
set_item("it-yinyang", 32, 3)

local ac1=set_actor("ac-blackball", 25.5, 9.5)
local ac2=set_actor("ac-whiteball", 32.5, 3.5)

AddRubberBand(ac1, ac2, 25, 1)
