--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 39
levelh = 37

create_world(levelw, levelh)

draw_border("st-greenbrown")

fill_floor("fl-leaves", 19,0, level_width/2+1, level_height)
fill_floor("fl-space", 0,0, level_width/2, level_height)

fill_floor("fl-gray", 1,1, 1,1)
fill_floor("fl-gray", 1,35, 3,1)
fill_floor("fl-gray", 2,34, 1,1)
fill_floor("fl-gray", 6,1, 1,1)
fill_floor("fl-gray", 8,1, 11,1)
fill_floor("fl-gray", 17,2, 2,2)
fill_floor("fl-gray", 14,35, 1,1)
fill_floor("fl-gray", 17,34, 1,2)
fill_floor("fl-gray", 10,24, 1,1)
fill_floor("fl-gray", 10,25, 2,1)
fill_floor("fl-gray", 13,25, 1,1)
fill_floor("fl-gray", 14,24, 3,1)
fill_floor("fl-gray", 16,25, 3,1)
fill_floor("fl-gray", 18,24, 1,1)
fill_floor("fl-gray", 10,28, 1,2)
fill_floor("fl-gray", 14,28, 3,1)
fill_floor("fl-gray", 18,28, 1,3)
fill_floor("fl-gray", 11,29, 1,2)
fill_floor("fl-gray", 17,29, 1,4)
fill_floor("fl-gray", 18,32, 1,2)
fill_floor("fl-gray", 13,29, 1,1)
fill_floor("fl-gray", 14,30, 2,1)
fill_floor("fl-gray", 16,31, 1,1)
fill_floor("fl-gray", 14,32, 2,1)
fill_floor("fl-gray", 15,33, 2,1)
fill_floor("fl-gray", 13,31, 1,3)
fill_floor("fl-gray", 12,30, 1,2)
fill_floor("fl-gray", 16,29, 1,1)
fill_floor("fl-gray", 12,34, 1,1)
fill_floor("fl-gray", 8,30, 2,1)
fill_floor("fl-gray", 8,31, 1,1)
fill_floor("fl-gray", 10,31, 1,1)
fill_floor("fl-gray", 9,32, 1,1)
fill_floor("fl-gray", 11,32, 1,1)
fill_floor("fl-gray", 10,33, 2,1)

draw_stones("st-greenbrown", {4,2}, {0,1}, 34)
draw_stones("st-greenbrown", {7,1}, {0,1}, 34)
draw_stones("st-greenbrown", {19,1}, {0,1}, 1)
draw_stones("st-greenbrown", {19,3}, {0,1}, 33)
draw_stones("st-greenbrown", {8,2}, {1,0}, 3)
draw_stones("st-greenbrown", {13,2}, {1,0}, 4)
draw_stones("st-greenbrown", {16,4}, {1,0}, 3)
draw_stones("st-greenbrown", {10,3}, {0,1}, 2)
draw_stones("st-greenbrown", {13,3}, {0,1}, 2)
draw_stones("st-greenbrown", {20,12}, {1,0}, 17)
draw_stones("st-greenbrown", {20,24}, {1,0}, 2)
draw_stones("st-greenbrown", {23,24}, {1,0}, 14)
draw_stones("st-greenbrown", {25,13}, {0,1}, 11)

set_stone("st-greenbrown", 16,3)
set_stone("st-greenbrown", 37,10)
set_stones("st-greenbrown", {{30,16},{30,17},{29,17},{30,19},{30,20},{29,19},
                            {32,16},{32,17},{33,17},{32,19},{32,20},{33,19}})

set_stones("st-grate1", {{31,16},{31,17},{31,20},{29,18},{32,18},{33,18}})

set_stones("st-brownie", {{33,26},{33,27},{33,28},{33,29},{33,25},{31,26},
                         {31,27},{31,28},{31,29},{31,25}})

draw_stones("st-greenbrown", {35,25}, {0,1}, 1)
draw_stones("st-greenbrown", {35,29}, {0,1}, 1)
draw_stones("st-greenbrown", {29,30}, {1,0}, 7)
draw_stones("st-greenbrown", {29,27}, {0,1}, 1)
draw_stones("st-greenbrown", {29,29}, {0,1}, 1)
draw_stones("st-greenbrown", {21,35}, {0,1}, 1)

shogundot3(31,19, {target="door", action="openclose"})
set_stone("st-shogun-sml",30,18)
set_stone("st-door", 37, 24 , {name="door", type="h"})

fill_floor("fl-inverse", 19,0, level_width/2, level_height/3)

fill_floor("fl-abyss", 21,2, 16,9)
fill_floor("fl-abyss", 21,20, 3,3)
fill_floor("fl-abyss", 36,25, 1,2)
fill_floor("fl-abyss", 23,35, 14,1)
fill_floor("fl-abyss", 23,33, 14,1)
fill_floor("fl-abyss", 36,29, 2,2)
fill_floor("fl-space", 26,34, 8,1)
fill_floor("fl-leaves", 28,34, 1,1)
fill_floor("fl-leaves", 31,34, 1,1)


fill_floor("fl-abyss", 26,13, 2,11)
fill_floor("fl-abyss", 28,13, 9,2)
fill_floor("fl-abyss", 28,22, 9,2)

draw_stones("st-greenbrown", {23,25}, {1,0},7)
draw_stones("st-greenbrown", {23,27}, {1,0},6)
draw_stones("st-greenbrown", {20,32}, {1,0},16)
draw_stones("st-glass", {21, 25}, {0,1},7)
set_stones("st-glass", {{35,26},{35,28},{29,28},{22,32}})

set_attrib(laser(38,26, FALSE, WEST), "name", "laser")
set_stone("st-switch", 22,35, {action="onoff", target="laser"})

mirrorp(20,26,0,0,4)
mirrorp(20,28,0,0,2)
mirrorp(37,28,0,0,3)
mirrorp(37,31,0,0,3)
mirror3(22,31,0,1,4)

oxyd(37,9)
oxyd(22,14)
oxyd(22,18)
oxyd(22,21)
fakeoxyd(22,16)
fakeoxyd(8,1)

oneway(19,2, enigma.WEST)
fill_floor("fl-leaves", 20,2, 1,1)
set_item("it-magicwand",9,3)

document(3,35, "Warp 9... Energy...")

set_actor("ac-blackball", 2.5,35.5)
