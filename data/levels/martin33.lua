--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 39
levelh = 25

create_world( levelw, levelh)
fill_floor("fl-water", 0, 0, levelw, levelh)
fill_floor("fl-sand", 4,2,levelw-8,levelh-4)
fill_floor("fl-water", 8,4, levelw-16,levelh-8)

draw_stones("st-puzzle", { 5, 5}, {1,0}, 23, {connections=PUZ_0101})
draw_stones("st-puzzle", {28, 3}, {0,1}, 15, {connections=PUZ_1010})
draw_stones("st-puzzle", {10,18}, {1,0}, 23, {connections=PUZ_0101})
draw_stones("st-puzzle", { 9, 6}, {0,1}, 15, {connections=PUZ_1010})

puzzle( 4, 5, PUZ_0100)
puzzle( 9, 5, PUZ_0111)
puzzle(28, 2, PUZ_0010)
puzzle(28, 5, PUZ_1011)
puzzle(33,18, PUZ_0001)
puzzle(28,18, PUZ_1101)
puzzle( 9,21, PUZ_1000)
puzzle( 9,18, PUZ_1110)

oxyd( 8,4)
oxyd(19,4)
oxyd( levelw-9,4)
oxyd( levelw-9,12)
oxyd( 8,levelh-5)
oxyd( 8,12)
oxyd( levelw-9,levelh-5)
oxyd( 19,levelh-5)

oxyd_shuffle()

draw_stones("st-oneway-n", {levelw-8,16}, {1,0}, 4)
draw_stones("st-oneway-n", {levelw-8, 8}, {1,0}, 4)
draw_stones("st-oneway-s", {4,8}, {1,0}, 4)
draw_stones("st-oneway-s", {4,16}, {1,0}, 4)
draw_stones("st-oneway-w", {15,2}, {0,1}, 2)
draw_stones("st-oneway-w", {23,2}, {0,1}, 2)
draw_stones("st-oneway-e", {15,levelh-4}, {0,1}, 2)
draw_stones("st-oneway-e", {23,levelh-4}, {0,1}, 2)

set_actor("ac-blackball", 33, 21)
