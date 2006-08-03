--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

stone="st-thief"
create_world( levelw, levelh)
draw_border( stone)
fill_floor( "fl-brick", 0, 0, levelw, levelh)

draw_stones( stone, { 1,3}, {2,0}, 4)
draw_stones( stone, {12,3}, {2,0}, 4)
draw_stones( stone, { 1,9}, {2,0}, 4)
draw_stones( stone, {12,9}, {2,0}, 4)
draw_stones( stone, { 4,1}, {0,1}, 3)
draw_stones( stone, { 8,1}, {0,1}, 3)
draw_stones( stone, {11,1}, {0,1}, 3)
draw_stones( stone, {15,1}, {0,1}, 3)
draw_stones( stone, { 4,9}, {0,1}, 3)
draw_stones( stone, { 8,9}, {0,1}, 3)
draw_stones( stone, {11,9}, {0,1}, 3)
draw_stones( stone, {15,9}, {0,1}, 3)
draw_stones( stone, { 2,6}, {1,0},16)

doorh(2,3,{name="door1"})
set_stone("st-floppy", 4, 4, {action="openclose", target="door1"})
doorh(6,3,{name="door2"})
set_stone("st-floppy", 8, 4, {action="openclose", target="door2"})
doorh(2,9,{name="door3"})
set_stone("st-floppy", 4, 8, {action="openclose", target="door3"})
doorh(6,9,{name="door4"})
set_stone("st-floppy", 8, 8, {action="openclose", target="door4"})
doorh(13,3,{name="door5"})
set_stone("st-floppy", 11, 4, {action="openclose", target="door5"})
doorh(17,3,{name="door6"})
set_stone("st-floppy", 15, 4, {action="openclose", target="door6"})
doorh(13,9,{name="door7"})
set_stone("st-floppy", 11, 8, {action="openclose", target="door7"})
doorh(17,9,{name="door8"})
set_stone("st-floppy", 15, 8, {action="openclose", target="door8"})


set_item("it-floppy", 10, 11)
document( 9, 4, "Warning ... ... thieves!")

oxyd( 2, 0)
oxyd( 6, 0)
oxyd(13, 0)
oxyd(17, 0)
oxyd( 2,12)
oxyd( 6,12)
oxyd(13,12)
oxyd(17,12)
oxyd_shuffle()

set_actor("ac-blackball", 10, 2.5)

















