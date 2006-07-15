--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

stone="st-rock2"

create_world( levelw, levelh)
draw_border( stone)
draw_stones( stone, { 4,4}, {0,1}, 5)
draw_stones( stone, {15,4}, {0,1}, 5)
draw_stones( stone, { 6,4}, {1,0}, 8)
draw_stones( stone, { 6,8}, {1,0}, 8)
draw_stones( stone, { 5,5}, {1,0},10)
draw_stones( stone, { 5,6}, {1,0},10)
draw_stones( stone, { 5,7}, {1,0},10)


draw_floor("fl-gradient", { 2,11}, {1,0}, 16, {type=14})
draw_floor("fl-gradient", { 4, 3}, {1,0}, 12, {type=16})
draw_floor("fl-gradient", { 2, 1}, {1,0}, 16, {type=15})
draw_floor("fl-gradient", { 4, 9}, {1,0}, 12, {type=13})
draw_floor("fl-gradient", { 3,10}, {1,0}, 15, {type=24})
draw_floor("fl-gradient", { 2, 2}, {1,0}, 15, {type=23})

draw_floor("fl-gradient", { 1, 2}, {0,1},  9, {type=17})
draw_floor("fl-gradient", {16, 4}, {0,1},  5, {type=19})
draw_floor("fl-gradient", { 3, 4}, {0,1},  5, {type=18})
draw_floor("fl-gradient", {18, 2}, {0,1},  9, {type=20})
draw_floor("fl-gradient", { 2, 3}, {0,1},  8, {type=22})
draw_floor("fl-gradient", {17, 2}, {0,1},  8, {type=21})

set_floor("fl-gradient", 1, 1, {type=5})
set_floor("fl-gradient",18, 1, {type=6})
set_floor("fl-gradient",18,11, {type=8})
set_floor("fl-gradient", 1,11, {type=7})
set_floor("fl-gradient", 3, 3, {type=12})
set_floor("fl-gradient",16, 3, {type=10})
set_floor("fl-gradient",16, 9, {type=9})
set_floor("fl-gradient", 3, 9, {type=11})

oxyd( 0, 3)
oxyd( 0, 9)
oxyd(19, 3)
oxyd(19, 9)
oxyd( 3, 0)
oxyd(16, 0)
oxyd( 3,12)
oxyd(16,12)
oxyd( 5, 4)
oxyd(14, 4)
oxyd( 5, 8)
oxyd(14, 8)

oxyd_shuffle()


set_actor("ac-blackball", 11, 11)











