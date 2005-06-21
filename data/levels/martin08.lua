--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)
draw_border("st-invisible")

fill_floor("fl-water", 0,0,levelw,levelh)
fill_floor("fl-sand", 6, 3,levelw-12,levelh-6)
fill_floor("fl-leaves", 7, 4, levelw-14,levelh-8)
fill_floor("fl-sand", 7, 4, 1, 1)
fill_floor("fl-sand", 12, 4, 1, 1)
fill_floor("fl-sand", 7, 8, 1, 1)
fill_floor("fl-sand", 12, 8, 1, 1)
fill_floor("fl-water", 6, 3, 1, 1)
fill_floor("fl-water", 13, 3, 1, 1)
fill_floor("fl-water", 6, 9, 1, 1)
fill_floor("fl-water", 13, 9, 1, 1)

set_stone("st-shogun-s", 8,6)

set_attrib(laser( 1, 5, FALSE, NORTH), "name", "laser1")
set_attrib(laser(18, 5, FALSE, NORTH), "name", "laser2")
set_attrib(laser( 1, 7, FALSE, SOUTH), "name", "laser3")
set_attrib(laser(18, 7, FALSE, SOUTH), "name", "laser4")
set_attrib(laser( 8, 1, FALSE,  WEST), "name", "laser5")
set_attrib(laser(11, 1, FALSE,  EAST), "name", "laser6")
set_attrib(laser( 8,11, FALSE,  WEST), "name", "laser7")
set_attrib(laser(11,11, FALSE,  EAST), "name", "laser8")

shogundot1(  8, 5, {target="laser5", action="onoff"})
shogundot1(  9, 5, {target="laser2", action="onoff"})
shogundot1( 10, 5, {target="laser8", action="onoff"})
shogundot1( 11, 5, {target="laser6", action="onoff"})
shogundot1(  8, 7, {target="laser7", action="onoff"})
shogundot1(  9, 7, {target="laser3", action="onoff"})
shogundot1( 10, 7, {target="laser1", action="onoff"})
shogundot1( 11, 7, {target="laser4", action="onoff"})

oxyd(  1, 3)
oxyd( 18, 3)
oxyd(  1, 9)
oxyd( 18, 9)
oxyd(  6, 1)
oxyd( 13, 1)
oxyd(  6, 11)
oxyd( 13, 11)

oxyd_shuffle()

set_actor("ac-blackball", 7, 5)
