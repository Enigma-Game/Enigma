--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-invisible")
fill_floor("fl-water", 0, 0, levelw, levelh)
fill_floor("fl-bluegray", 1,1, level_width-2,level_height-2)

oxyd(1,1)
oxyd(18,11)
oxyd(18,1)
oxyd(5,5)
oxyd(1,11)
oxyd(14,6)
oxyd_shuffle()

draw_stones("st-invisible", {4,4}, {1,0},3)
draw_stones("st-invisible", {10,4}, {1,0},4)
draw_stones("st-invisible", {6,7}, {1,0},4)
draw_stones("st-invisible", {12,7}, {1,0},6)
draw_stones("st-invisible", {3,10}, {1,0},5)
draw_stones("st-invisible", {10,10}, {1,0},2)
draw_stones("st-invisible", {2,1}, {0,1},4)
draw_stones("st-invisible", {6,5}, {0,1},1)
draw_stones("st-invisible", {8,1}, {0,1},5)
draw_stones("st-invisible", {13,5}, {0,1},2)
draw_stones("st-invisible", {16,1}, {0,1},5)
draw_stones("st-invisible", {17,8}, {0,1},4)
draw_stones("st-invisible", {3,6}, {0,1},4)
draw_stones("st-invisible", {9,8}, {0,1},4)
draw_stones("st-invisible", {13,9}, {0,1},3)
draw_stones("st-invisible", {15,9}, {0,1},3)

set_actor("ac-blackball", 16.5,11.5)
















