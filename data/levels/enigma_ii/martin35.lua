--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

stone="st-brick"

create_world( levelw, levelh)
fill_floor( "fl-abyss", 0, 0, levelw, levelh)

draw_border( stone)
draw_stones( stone, { 2, 2}, {0,1}, 9)
draw_stones( stone, {17, 2}, {0,1}, 9)
draw_stones( stone, { 4, 2}, {1,0},12)
draw_stones( stone, { 4,10}, {1,0},12)

draw_floor("fl-gradient", { 2, 1}, {1,0},  8, {type=3})
draw_floor("fl-gradient", {10, 1}, {1,0},  8, {type=4})
draw_floor("fl-gradient", { 2,11}, {1,0},  8, {type=3})
draw_floor("fl-gradient", {10,11}, {1,0},  8, {type=4})
draw_floor("fl-gradient", { 1, 2}, {0,1}, 15, {type=1})
draw_floor("fl-gradient", { 1, 6}, {0,1}, 15, {type=2})
draw_floor("fl-gradient", {18, 2}, {0,1}, 5, {type=1})
draw_floor("fl-gradient", {18, 6}, {0,1}, 5, {type=2})

set_floor("fl-gradient", 1, 1, {type=9})
set_floor("fl-gradient",18, 1, {type=11})
set_floor("fl-gradient",18,11, {type=12})
set_floor("fl-gradient", 1,11, {type=10})

oxyd( 0, 3)
oxyd( 0, 9)
oxyd(19, 3)
oxyd(19, 9)
oxyd( 3, 0)
oxyd(16, 0)
oxyd( 3,12)
oxyd(16,12)
oxyd( 2, 3)
oxyd( 2, 9)
oxyd(17, 3)
oxyd(17, 9)
oxyd( 3, 2)
oxyd( 3,10)
oxyd(16, 2)
oxyd(16,10)

oxyd_shuffle()


set_actor("ac-blackball", 1.5, 10)















