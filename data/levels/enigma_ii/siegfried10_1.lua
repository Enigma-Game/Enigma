--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-glass")
fill_floor("fl-leaves")

oxyd(13,5)
oxyd(15,7)
oxyd(15,5)
oxyd(13,7)

fill_floor("fl-water", 1,1, 1,1)
fill_floor("fl-water", 2,2, 9,1)
fill_floor("fl-water", 1,6, 1,5)
fill_floor("fl-water", 1,11, 17,1)
fill_floor("fl-water", 11,1, 7,1)
fill_floor("fl-water", 18,1, 1,11)
fill_floor("fl-water", 11,6, 1,5)
fill_floor("fl-water", 3,6, 1,1)
fill_floor("fl-water", 3,9, 1,1)
fill_floor("fl-water", 5,8, 1,1)
fill_floor("fl-water", 7,6, 1,1)
fill_floor("fl-water", 7,8, 1,1)
fill_floor("fl-water", 9,7, 1,1)
fill_floor("fl-water", 9,9, 1,1)

set_stones("st-glass", {{12,3},{12,9},{14,3},{14,9},{16,3},{16,9}})
set_stones("st-glass", {{2,1},{4,1},{6,1},{8,1},{10,1}})
set_stones("st-glass", {{2,4},{4,4},{6,4},{8,4},{10,4}})

draw_stones("st-glass", {12,4}, {0,1},5)
draw_stones("st-glass", {14,4}, {0,1},5)
draw_stones("st-glass", {16,4}, {0,1},5)

set_item("it-trigger", 5,1, {action="openclose", target="door1"})
set_stone("st-door", 13, 3 , {name="door1", type="h"})

set_item("it-trigger", 3,1, {action="openclose", target="door2"})
set_stone("st-door", 15, 3 , {name="door2", type="h"})

set_item("it-trigger", 9,1, {action="openclose", target="door3"})
set_stone("st-door", 15, 9 , {name="door3", type="h"})

set_item("it-trigger", 7,1, {action="openclose", target="door4"})
set_stone("st-door", 13, 9 , {name="door4", type="h"})

puzzle(3,7,PUZ_0010)
puzzle(9,6,PUZ_1010)
puzzle(9,7,PUZ_1010)
puzzle(9,8,PUZ_1010)
puzzle(6,9,PUZ_1001)
puzzle(5,5,PUZ_0101)
puzzle(4,5,PUZ_0100)

set_actor("ac-blackball" ,2.5, 6.5)
















