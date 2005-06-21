--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-rock1")

oxyd(18, 2)
oxyd(18,10)
oxyd(14, 1)
oxyd(14,11)

oxyd_shuffle()

fill_floor("fl-sahara", 0, 0, 10,level_height)
fill_floor("fl-water",10, 0, 10,level_height)
fill_floor("fl-samba", 15, 3, 3, 1)
fill_floor("fl-samba", 15, 9, 3, 1)
fill_floor("fl-samba", 17, 6, 2, 1)
fill_floor("fl-samba", 18, 3, 1, 2)
fill_floor("fl-samba", 18, 8, 1, 2)
fill_floor("fl-samba", 13, 1, 1, 1)
fill_floor("fl-samba", 14, 2, 1, 1)
fill_floor("fl-samba", 13,11, 1, 1)
fill_floor("fl-samba", 14,10, 1, 1)

draw_stones("st-block",  { 8, 2}, {0,1}, 9)
draw_stones("st-grate1", { 1, 1}, {1,0},13)
draw_stones("st-grate1", { 1,11}, {1,0},13)
draw_stones("st-grate1", {18, 3}, {0,1}, 7)
draw_stones("st-grate1", {15, 1}, {1,0}, 4)
draw_stones("st-grate1", {15,11}, {1,0}, 4)
draw_stones("st-grate1", { 1, 2}, {0,1}, 9)
draw_stones("st-grate1", { 9, 2}, {0,1}, 4)
draw_stones("st-grate1", { 9, 7}, {0,1}, 4)
draw_stones("st-rock1",  { 3, 4}, {0,1}, 5)
set_stones("st-rock1", {{4,4},{4,8}})
set_stones("st-block", {{4,6}})

puzzle( 3, 3,PUZ_0100)
puzzle( 4, 3,PUZ_0101)
puzzle( 5, 3,PUZ_0101)
puzzle( 6, 3,PUZ_0001)
puzzle( 3, 9,PUZ_0100)
puzzle( 4, 9,PUZ_0101)
puzzle( 5, 9,PUZ_0101)
puzzle( 6, 9,PUZ_0001)

set_item("it-seed", 1, 1)
set_item("it-seed", 1,11)

set_item("it-spring1",17, 6)

set_actor("ac-blackball", 5.5,6.5 )
