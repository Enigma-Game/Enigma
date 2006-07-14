--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 25

create_world(levelw, levelh)
draw_border("st-marble")
fill_floor("fl-bluegreen")

enigma.AddConstantForce(15,0)

--oxyd nicht veraendern--

oxyd(16,21)
oxyd(16,17)
oxyd(16,23)
oxyd(16,15)

--oxyd nicht veraendern--

fill_floor("fl-inverse", 3,4, 3,7)
fill_floor("fl-inverse", 2,5, 1,5)
fill_floor("fl-inverse", 6,5, 1,5)
fill_floor("fl-inverse", 11,5, 6,1)
fill_floor("fl-inverse", 11,9, 6,1)

draw_stones("st-marble", {2,2}, {1,0},16)
draw_stones("st-marble", {10,4}, {1,0},9)
draw_stones("st-marble", {10,8}, {1,0},9)
draw_stones("st-marble", {8,6}, {1,0},10)
draw_stones("st-marble", {8,10}, {1,0},10)
draw_stones("st-marble", {10,12}, {1,0},9)
draw_stones("st-marble", {2,12}, {1,0},7)
draw_stones("st-marble", {2,13}, {0,1},6)
draw_stones("st-marble", {2,20}, {0,1},4)
draw_stones("st-marble", {3,14}, {1,0},15)
draw_stones("st-marble", {17,15}, {0,1},4)
draw_stones("st-marble", {17,20}, {0,1},4)
draw_stones("st-marble", {4,16}, {1,0},11)
draw_stones("st-marble", {4,18}, {1,0},11)
draw_stones("st-marble", {4,20}, {1,0},11)
draw_stones("st-marble", {4,22}, {1,0},11)
set_stones("st-marble", {{16,16},{16,18},{16,20},{16,22}})
set_stones("st-marble", {{8,4},{8,8}})

set_stones("st-death", {{8,3},{8,5},{8,7},{8,9},{8,11}})
set_stones("st-death", {{19,1},{19,3},{19,5},{19,7},{19,9},{19,11}})

oneway(9,4, enigma.SOUTH)
oneway(9,8, enigma.SOUTH)
oneway(9,12, enigma.SOUTH)
oneway(18,2, enigma.SOUTH)
oneway(18,6, enigma.SOUTH)
oneway(18,10, enigma.SOUTH)
oneway(2,19, enigma.WEST)
oneway(3,16, enigma.SOUTH)
oneway(3,18, enigma.SOUTH)
oneway(3,20, enigma.NORTH)
oneway(3,22, enigma.NORTH)
oneway(4,15, enigma.WEST)
oneway(4,17, enigma.WEST)
oneway(4,19, enigma.WEST)
oneway(4,21, enigma.WEST)
oneway(4,23, enigma.WEST)
oneway(17,19, enigma.WEST)
oneway(15,16, enigma.NORTH)
oneway(15,18, enigma.NORTH)
oneway(15,20, enigma.SOUTH)
oneway(15,22, enigma.SOUTH)

set_actor("ac-blackball", 4.5,1.5)
















