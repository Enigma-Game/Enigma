--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 39
levelh = 13

create_world( levelw, levelh)
fill_floor( "fl-marble", 0, 0, levelw, levelh)
draw_border( "st-invisible_magic")

draw_stones( "st-invisible_magic", {1,1}, {0,1},11)
draw_stones( "st-invisible_magic", {8,1}, {0,1}, 8)
draw_stones( "st-invisible_magic", {13,4}, {0,1}, 8)
draw_stones( "st-invisible_magic", {18,1}, {0,1}, 8)
draw_stones( "st-invisible_magic", {23,4}, {0,1}, 8)
draw_stones( "st-brick_magic", {28,1}, {0,1}, 8)
draw_stones( "st-brick_magic", {33,4}, {0,1}, 8)

set_attrib( laser( 38,2, FALSE, WEST), "name", "laser1")
set_stone("st-switch", 38,1, {action="onoff", target="laser1"})

set_attrib( laser( 38,5, FALSE, WEST), "name", "laser2")
set_stone("st-switch", 38,4, {action="onoff", target="laser2"})

set_attrib( laser( 38,7, FALSE, WEST), "name", "laser3")
set_stone("st-switch", 38,8, {action="onoff", target="laser3"})

set_attrib( laser( 38,10, FALSE, WEST), "name", "laser4")
set_stone("st-switch", 38,11, {action="onoff", target="laser4"})

oxyd( 0, 2)
oxyd( 0, 5)
oxyd( 0, 7)
oxyd( 0,10)

oxyd_shuffle()

set_actor("ac-blackball", 5.5, 4.5)
set_item("it-magicwand",5,6)















