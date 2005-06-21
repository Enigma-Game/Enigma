--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 25

create_world(levelw, levelh)
draw_border("st-marble")
fill_floor("fl-leaves")
fill_floor("fl-abyss", 0,0, level_width,10)

oxyd( 4, 16)
oxyd( 16,22)

fakeoxyd(17,14)
fakeoxyd(2,21)

set_floor("fl-abyss", 13,16)
fill_floor("fl-abyss", 4,10, 3,2)
fill_floor("fl-abyss", 8,10, 8,1)

puzzle(8,13,PUZ_0110)
puzzle(9,13,PUZ_0101)
puzzle(10,13,PUZ_0101)
puzzle(11,13,PUZ_0101)
puzzle(12,13,PUZ_0001)
puzzle(6,16,PUZ_0100)
puzzle(7,16,PUZ_0101)
puzzle(8,16,PUZ_1111)
puzzle(9,16,PUZ_0001)
puzzle(4,19,PUZ_1000)
puzzle(13,14,PUZ_1001)
puzzle(13,15,PUZ_1010)
puzzle(13,17,PUZ_1010)
puzzle(13,18,PUZ_0100)
puzzle(12,19,PUZ_1100)
puzzle(12,18,PUZ_0001)
puzzle(11,18,PUZ_0110)
puzzle(11,19,PUZ_1010)

set_stone("st-door", 17,19, {name="door1", type="h"})
set_item("it-trigger", 17,16, {action="openclose", target="door1"})

set_item("it-spring1",7,11)

set_stones("st-invisible", {{7,9}})
draw_stones("st-marble", {8,15}, {1,0},5)
draw_stones("st-marble", {8,17}, {1,0},5)
draw_stones("st-marble", {14,19}, {0,1},5)
draw_stones("st-marble", {15,19}, {1,0},2)
set_stones("st-marble", {{14,15},{14,17},{18,19},{1,13}})
set_stones("st-grate1", {{13,13},{14,13},{14,14},{14,18},{6,15}})
draw_stones("st-marble", {3,12}, {1,0},4)
draw_stones("st-marble", {8,12}, {1,0},11)
draw_stones("st-marble_hole", {1,12}, {1,0},2)
set_stones("st-marble_move", {{7,12},{2,13},{7,15},{3,13}})
draw_stones("st-marble", {1,23}, {1,0},3)
draw_stones("st-grate1", {4,18}, {1,1},4)

set_actor("ac-blackball", 15.5, 16.5)

