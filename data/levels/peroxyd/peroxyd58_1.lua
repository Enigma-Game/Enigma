--
-- A level for Enigma
--
-- Copyright (c) 2002 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 25

create_world(levelw, levelh)
draw_border("st-marble")
fill_floor("fl-marble")

oxyd( 4, 13)
oxyd( 1, 23)
oxyd( 6, 20)
oxyd( 18, 7)

hammer(2,11)

fill_floor("fl-water", 4,10, 1,1)
fill_floor("fl-water", 14,10, 1,1)
fill_floor("fl-water", 8,10, 1,2)
fill_floor("fl-water", 9,8, 1,4)
fill_floor("fl-water", 10,10, 1,2)

puzzle(2,11,PUZ_0001)
puzzle(2,13,PUZ_0110)
puzzle(1,15,PUZ_1010)
puzzle(2,15,PUZ_1010)
puzzle(1,17,PUZ_1010)
puzzle(2,17,PUZ_1010)
puzzle(1,19,PUZ_1010)
puzzle(2,19,PUZ_1010)
puzzle(1,21,PUZ_1010)
puzzle(2,21,PUZ_1010)
puzzle(2,23,PUZ_1000)
puzzle(9,5,PUZ_0010)
puzzle(9,6,PUZ_1000)
puzzle(8,7,PUZ_0100)
puzzle(9,7,PUZ_0101)
puzzle(10,7,PUZ_0001)
puzzle(4,9,PUZ_0100)
puzzle(5,9,PUZ_0101)
puzzle(6,9,PUZ_0001)
puzzle(12,9,PUZ_0100)
puzzle(13,9,PUZ_0101)
puzzle(14,9,PUZ_0001)
puzzle(11,14,PUZ_0100)
puzzle(12,14,PUZ_0011)
puzzle(12,15,PUZ_1010)
puzzle(12,16,PUZ_1010)
puzzle(12,17,PUZ_1010)
puzzle(12,18,PUZ_1010)
puzzle(12,19,PUZ_1010)
puzzle(12,20,PUZ_1000)

draw_stones("st-marble", {3,5}, {0,1},12)
draw_stones("st-marble", {4,2}, {0,1},7)
draw_stones("st-marble", {5,6}, {0,1},3)
draw_stones("st-marble", {6,6}, {1,0},2)
draw_stones("st-marble", {6,1}, {1,0},13)
draw_stones("st-marble", {16,2}, {1,0},3)
draw_stones("st-marble", {6,3}, {1,0},13)
draw_stones("st-marble", {6,6}, {1,0},2)
draw_stones("st-marble", {11,6}, {1,0},8)
draw_stones("st-grate1", {5,2}, {1,0},14)
draw_stones("st-grate1", {3,1}, {1,0},3)
draw_stones("st-grate1", {3,2}, {0,1},3)
draw_stones("st-marble", {13,7}, {1,0},4)
draw_stones("st-marble", {13,8}, {1,0},4)
draw_stones("st-marble", {15,9}, {0,1},4)
draw_stones("st-marble", {16,9}, {0,1},13)
draw_stones("st-marble", {4,12}, {1,0},5)
draw_stones("st-marble", {10,12}, {1,0},5)
draw_stones("st-marble", {5,16}, {1,0},7)
draw_stones("st-marble", {13,16}, {1,0},3)
draw_stones("st-marble", {3,18}, {0,1},4)
draw_stones("st-marble", {4,18}, {0,1},4)
draw_stones("st-marble", {5,17}, {0,1},5)
draw_stones("st-marble", {6,21}, {1,0},6)
draw_stones("st-marble", {13,21}, {1,0},3)
set_stones("st-marble", {{8,9},{10,9}})
set_stones("st-wood", {{6,10},{12,10}})
set_stones("st-stone_break", {{12,13}})
set_stone("st-oneway-s", 4,16)

set_actor("ac-blackball", 9.5, 4.5)











