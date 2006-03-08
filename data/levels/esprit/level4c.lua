--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 39
levelh = 13

create_world( levelw, levelh)
draw_border( "st-wood")
fill_floor("fl-hay", 0,0, levelw,levelh)
fill_floor("fl-water", 34,3, 1,1)
fill_floor("fl-water", 21,1, 17,2)
fill_floor("fl-water", 21,4, 16,7)

oxyd(1, 4)
oxyd(12, 1)
oxyd(1, 6)
oxyd(10, 7)
oxyd(10, 11)
oxyd(13, 10)
oxyd_shuffle()

fill_floor("fl-abyss", 1,1, 2,3)
fill_floor("fl-abyss", 4,2, 6,2)
fill_floor("fl-abyss", 9,4, 3,2)
fill_floor("fl-abyss", 13,1, 3,1)
fill_floor("fl-abyss", 1,5, 7,1)
fill_floor("fl-abyss", 5,6, 3,2)
fill_floor("fl-abyss", 5,9, 3,2)
fill_floor("fl-abyss", 9,7, 3,2)
fill_floor("fl-abyss", 10,3, 2,1)
fill_floor("fl-abyss", 13,3, 1,6)
fill_floor("fl-abyss", 15,3, 1,8)
fill_floor("fl-abyss", 1,7, 1,5)
fill_floor("fl-abyss", 17,2, 1,3)
fill_floor("fl-abyss", 17,6, 1,5)
fill_floor("fl-abyss", 3,7, 1,4)
fill_floor("fl-abyss", 11,10, 4,1)
abyss(2,6)
abyss(4,8)
abyss(11,1)
abyss(9,9)
abyss(9,11)

oneway(35,3, enigma.EAST)
draw_stones("st-wood", {19,1}, {0,1},11)
draw_stones("st-wood", {20,1}, {0,1},2)
draw_stones("st-wood", {20,4}, {0,1},7)
set_stones("st-wood", {{3,4},{2,8},{3,11},{6,8},{7,11},{9,6}})
set_stones("st-wood", {{11,2},{12,9},{14,8},{16,6},{18,10}})
set_stones("st-grate1", {{1,5},{3,9},{11,1},{9,9},{14,9}})
draw_stones("st-grate1", {12,3}, {1,1},2)
draw_stones("st-grate1", {12,5}, {1,1},2)
draw_stones("st-grate1", {17,2}, {0,1},3)

set_actor("ac-blackball", 8.5, 4.5)










