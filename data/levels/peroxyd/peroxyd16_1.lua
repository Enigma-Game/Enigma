--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "c"       -- Default flavor for oxyd stones.

draw_border("st-rock2")

fill_floor("fl-gray")

oxyd (6,0)
oxyd (7,12)
oxyd (11,12)
oxyd (10,0)
oxyd (13,0)
oxyd (19,7)

set_attrib(laser(0,6, FALSE, EAST), "name", "laser")
set_stone("st-switch", 0,7, {action="onoff", target="laser"})

draw_stones("st-glass", {1,1},{1,0},18)
draw_stones("st-glass", {1,11},{1,0},18)
draw_stones("st-glass", {1,2},{0,1},5)
draw_stones("st-glass", {1,8},{0,1},3)
draw_stones("st-glass", {18,2},{0,1},9)

mirror3(9,5,0,0,4)
mirror3(10,4,0,0,4)
mirror3(11,4,0,0,4)
mirror3(12,4,0,0,4)
mirror3(10,6,0,0,4)
mirror3(11,6,0,0,4)
mirror3(11,7,0,0,2)
mirror3(7,8,0,1,2)
mirror3(8,7,0,0,1)
mirror3(9,6,0,0,3)
mirrorp(9,4,0,0,4)
mirrorp(8,5,0,0,1)
mirrorp(13,5,0,0,4)
mirrorp(12,7,0,0,4)
mirrorp(8,6,0,0,2)
mirrorp(8,8,0,0,4)
mirrorp(6,8,0,0,1)

set_actor("ac-blackball", 3,7)











