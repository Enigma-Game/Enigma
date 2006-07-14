--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 21
levelh = 14

create_world( levelw, levelh)
draw_border("st-glass", 0, 0, 20, 13)
fill_floor("fl-himalaya")

draw_border("st-glass", 2, 3, 9, 7)

oneway( 2,6, WEST)
oneway(10,6, EAST)

set_attrib( laser( 20, 2, 1, WEST), "name", "laser1")
set_attrib( laser( 20,10, 1, WEST), "name", "laser2")
set_attrib( laser(  1,13, 1, NORTH), "name", "laser3")
set_attrib( laser( 12,13, 1, NORTH), "name", "laser4")
set_attrib( laser( 13,13, 1, NORTH), "name", "laser5")
set_attrib( laser( 14,13, 1, NORTH), "name", "laser6")
set_attrib( laser( 15,13, 1, NORTH), "name", "laser7")
set_attrib( laser( 16,13, 1, NORTH), "name", "laser8")
set_attrib( laser( 17,13, 1, NORTH), "name", "laser9")

shogundot1( 4,6, {target="laser1", action="onoff"})
shogundot1( 5,5, {target="laser2", action="onoff"})
shogundot1( 5,7, {target="laser3", action="onoff"})
shogundot1( 6,4, {target="laser4", action="onoff"})
shogundot1( 6,6, {target="laser5", action="onoff"})
shogundot1( 6,8, {target="laser6", action="onoff"})
shogundot1( 7,5, {target="laser7", action="onoff"})
shogundot1( 7,7, {target="laser8", action="onoff"})
shogundot1( 8,6, {target="laser9", action="onoff"})

set_stone("st-shogun-s",4,5)
set_stone("st-shogun-s",4,7)
set_stone("st-shogun-s",5,6)
set_stone("st-shogun-s",6,4)
set_stone("st-shogun-s",6,5)
set_stone("st-shogun-s",6,7)
set_stone("st-shogun-s",6,8)
set_stone("st-shogun-s",7,6)
set_stone("st-shogun-s",8,7)

oxyd( 0, 1)
oxyd( 0,11)
oxyd(18, 0)
oxyd(19, 1)
oxyd(18,12)
oxyd(19,11)

oxyd_shuffle()

set_actor("ac-blackball", 9.5, 5.5)
















