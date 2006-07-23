--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)

draw_border("st-metal")

oxyd (1,1)
oxyd (4,4)
oxyd (10,5)
oxyd (10,11)
oxyd (16,1)
oxyd (9,1)
oxyd (18,11)
oxyd (18,1)
oxyd_shuffle()

fill_floor("fl-bluegreen", 0,0, levelw,levelh)

set_actor("ac-blackball", 10,8.5)

draw_stones("st-metal", {3,3}, {0,1},7)
draw_stones("st-metal", {6,5}, {0,1},5)
draw_stones("st-metal", {14,1}, {0,1},5)
draw_stones("st-metal", {4,3}, {1,0},4)
draw_stones("st-metal", {17,1}, {0,1},4)
draw_stones("st-metal", {17,6}, {0,1},2)
draw_stones("st-metal", {9,10}, {1,0},3)
draw_stones("st-metal", {8,1}, {0,1},2)
draw_stones("st-metal", {4,9}, {1,0},2)

set_stone("st-swap", 11,3)
set_stone("st-swap", 14,7)
set_stone("st-swap", 15,9)
set_stone("st-swap", 2,10)

oneway(6,4, enigma.EAST)
oneway(7,2, enigma.NORTH)
oneway(2,3, enigma.SOUTH)
oneway(14,6, enigma.WEST)
oneway(17,5, enigma.WEST)
oneway(16,7, enigma.NORTH)

--document(11,5, "Steinchen wechsel dich...")

set_stones("st-metal", {{3,11},{9,11},{14,11},{13,3},{18,7},{14,8},
                        {15,10},{14,11},{13,10},{12,9},{1,3}})

set_stones("st-grate1", {{7,1},{8,3},{9,3},{10,3},{12,3}})
set_stones("st-grate1", {{4,8},{5,8},{2,9},{2,11}})
set_stones("st-grate1", {{12,10},{13,9},{14,9},{14,10}})
set_stones("st-grate1", {{15,7},{15,8},{18,6}})












