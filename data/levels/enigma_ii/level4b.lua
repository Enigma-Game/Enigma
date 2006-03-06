--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)
draw_border( "st-wood")
fill_floor("fl-dunes")

oxyd( 1, 2)
oxyd( 18, 2)
oxyd( 1, 10)
oxyd(18, 10)
oxyd_shuffle()

puzzle(5,3,PUZ_1111)
puzzle(6,3,PUZ_1111)
puzzle(7,3,PUZ_1111)
puzzle(8,3,PUZ_1111)
puzzle(9,3,PUZ_1111)
puzzle(10,3,PUZ_1111)
puzzle(11,3,PUZ_1111)
puzzle(12,3,PUZ_1111)
puzzle(13,3,PUZ_1111)
puzzle(14,3,PUZ_1111)
puzzle(5,4,PUZ_1111)
puzzle(6,4,PUZ_1111)
puzzle(7,4,PUZ_1111)
puzzle(8,4,PUZ_1111)
puzzle(9,4,PUZ_1111)
puzzle(10,4,PUZ_1111)
puzzle(11,4,PUZ_1111)
puzzle(12,4,PUZ_1111)
puzzle(13,4,PUZ_1111)
puzzle(14,4,PUZ_1111)
puzzle(5,5,PUZ_1111)
puzzle(6,5,PUZ_1111)
puzzle(7,5,PUZ_0000)
puzzle(8,5,PUZ_1111)
puzzle(9,5,PUZ_0000)
puzzle(10,5,PUZ_1111)
puzzle(11,5,PUZ_0000)
puzzle(12,5,PUZ_1111)
puzzle(13,5,PUZ_1111)
puzzle(14,5,PUZ_1111)
puzzle(5,6,PUZ_1111)
puzzle(6,6,PUZ_1111)
puzzle(7,6,PUZ_1111)
puzzle(8,6,PUZ_1111)
puzzle(9,6,PUZ_1111)
puzzle(10,6,PUZ_0000)
puzzle(11,6,PUZ_1111)
puzzle(12,6,PUZ_1111)
puzzle(13,6,PUZ_1111)
puzzle(14,6,PUZ_1111)
puzzle(5,7,PUZ_1111)
puzzle(6,7,PUZ_1111)
puzzle(7,7,PUZ_1111)
puzzle(8,7,PUZ_1111)
puzzle(9,7,PUZ_1111)
puzzle(10,7,PUZ_1111)
puzzle(11,7,PUZ_1111)
puzzle(12,7,PUZ_0000)
puzzle(13,7,PUZ_1111)
puzzle(14,7,PUZ_1111)
puzzle(5,8,PUZ_1111)
puzzle(6,8,PUZ_1111)
puzzle(7,8,PUZ_1111)
puzzle(8,8,PUZ_0000)
puzzle(9,8,PUZ_1111)
puzzle(10,8,PUZ_1111)
puzzle(11,8,PUZ_1111)
puzzle(12,8,PUZ_1111)
puzzle(13,8,PUZ_1111)
puzzle(14,8,PUZ_1111)
puzzle(5,9,PUZ_1111)
puzzle(6,9,PUZ_1111)
puzzle(7,9,PUZ_1111)
puzzle(8,9,PUZ_1111)
puzzle(9,9,PUZ_1111)
puzzle(10,9,PUZ_1111)
puzzle(11,9,PUZ_1111)
puzzle(12,9,PUZ_1111)
puzzle(13,9,PUZ_1111)
puzzle(14,9,PUZ_1111)

draw_stones("st-wood", {8,2}, {1,0},4)
draw_stones("st-wood", {8,10}, {1,0},4)
draw_stones("st-wood", {1,5}, {0,1},3)
draw_stones("st-wood", {18,5}, {0,1},3)
set_stones("st-wood", {{3,4},{3,6},{3,8},{16,4},{16,6},{16,8}})
draw_stones("st-invisible", {8,11}, {1,0},4)
draw_stones("st-invisible", {2,10}, {1,0},3)
draw_stones("st-invisible", {2,2}, {0,1},8)
draw_stones("st-invisible", {17,2}, {0,1},9)
draw_stones("st-grate1", {5,2}, {1,0},3)
draw_stones("st-grate1", {12,2}, {1,0},3)
draw_stones("st-grate1", {5,10}, {1,0},3)
draw_stones("st-grate1", {12,10}, {1,0},3)
draw_stones("st-grate1", {4,3}, {0,1},7)
draw_stones("st-grate1", {15,3}, {0,1},7)
draw_stones("st-grate1", {2,1}, {1,0},6)
draw_stones("st-grate1", {12,1}, {1,0},6)
draw_stones("st-grate1", {2,11}, {1,0},6)
draw_stones("st-grate1", {12,11}, {1,0},6)

set_item("it-magicwand",3,6)

document(3,8, "")

set_actor("ac-blackball", 15.5, 9.5)
















