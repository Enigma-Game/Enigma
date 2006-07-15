--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 37

create_world( levelw, levelh)
draw_border("st-greenbrown")
fill_floor("fl-dunes", 0,0,levelw,5)
fill_floor("fl-abyss", 0,5,levelw,5)
fill_floor("fl-dunes", 0,10,levelw,levelh-10)

oxyd( 4,3)
oxyd( 8,3)
oxyd(11,3)
oxyd(15,3)
oxyd_shuffle()


mirrorp( 10,34,TRUE,FALSE,2)

draw_stones( "st-grate1", {1,33},{1,0},levelw-2)
draw_stones( "st-grate1", {1,35},{1,0},levelw-2)
set_stone( "st-grate1", 1, 34)
set_stone( "st-grate1", levelw-2,34)

set_attrib( laser( 0,34, FALSE, EAST), "name", "laser")
set_stone("st-switch", 0,30, {action="onoff", target="laser"})

set_actor("ac-blackball", 10, 30)











