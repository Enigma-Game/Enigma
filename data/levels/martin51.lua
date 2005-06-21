--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 13)
oxyd_default_flavor = "b"

fill_floor("fl-rough")
fill_floor("fl-rough_slow", 14, 5, 3, 3)

stone = "st-rock2"

draw_border( stone)
draw_stones( stone, { 1,5}, {1,0}, 4)
draw_stones( stone, { 1,7}, {1,0}, 4)
draw_stones( stone, { 5,1}, {0,1}, 5)
draw_stones( stone, { 5,7}, {0,1}, 5)
draw_stones( stone, { 7,1}, {0,1}, 5)
draw_stones( stone, { 7,7}, {0,1}, 5)
draw_stones( stone, { 8,5}, {1,0}, 7)
draw_stones( stone, { 8,7}, {1,0}, 7)
draw_stones( stone, {14,4}, {1,0}, 3)
draw_stones( stone, {14,8}, {1,0}, 3)
draw_stones( stone, {17,5}, {0,1}, 3)
draw_stones( stone, {16,5}, {0,2}, 2)
set_stone( stone, 13,6)

doorv( 2, 6, {name="door1"})
doorh( 6, 2, {name="door2"})
doorh( 6,10, {name="door3"})
doorv(10, 6, {name="door4"})
set_floor( "fl-bridge", 6, 6, {name="bridge"})

set_item( "it-trigger", 14,6, {action="openclose", target="door1"})
set_item( "it-trigger", 15,5, {action="openclose", target="door2"})
set_item( "it-trigger", 15,6, {action="openclose", target="bridge"})
set_item( "it-trigger", 15,7, {action="openclose", target="door3"})
set_item( "it-trigger", 16,6, {action="openclose", target="door4"})

oxyd( 6, 0)
oxyd( 6,12)
oxyd( 0, 6)
oxyd(12, 6)

oxyd_shuffle()

set_actor("ac-blackball", 9.2, 6.5, {essential=1})
set_actor("ac-whiteball-small", 15.5, 6.5, {player=0})
