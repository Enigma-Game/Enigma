--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 13)
oxyd_default_flavor = "b"

draw_checkerboard_floor( "fl-normal", "fl-metal", 0, 0, 20, 13)
draw_border("st-fakeoxyd")

draw_stones("st-fakeoxyd", { 2,1}, {0,1}, 5)
draw_stones("st-fakeoxyd", {17,1}, {0,1}, 5)
draw_stones("st-fakeoxyd", { 2,7}, {0,1}, 5)
draw_stones("st-fakeoxyd", {17,7}, {0,1}, 5)

draw_stones("st-fakeoxyd", {15,1}, {0,1}, 2)

set_stone( "st-fakeoxyd", 5, 6)
set_stone( "st-fakeoxyd",14, 6)

doorv( 2,6, {name="door1"})
doorv(17,6, {name="door2"})

set_stone( "st-laserswitch",  5, 0, {action="openclose", target="door2"})
set_stone( "st-laserswitch", 14,12, {action="openclose", target="door1"})

laser( 5,12, 1, NORTH)
laser(14, 0, 1, SOUTH)

mirrorp( 5, 5, 0, 0, 1)
mirrorp( 5, 7, 0, 0, 1)
mirror3( 9, 5, 0, 0, 1)
mirror3( 9, 6, 0, 0, 4)
mirror3( 9, 7, 0, 0, 3)
mirror3(10, 5, 0, 0, 1)
mirror3(10, 6, 0, 0, 2)
mirror3(10, 7, 0, 0, 3)
mirrorp(14, 5, 0, 0, 1)
mirrorp(14, 7, 0, 0, 1)


oxyd( 0, 4)
oxyd( 0, 8)
oxyd(19, 4)
oxyd(19, 8)
oxyd( 9, 0)
oxyd(10, 0)
oxyd( 9,12)
oxyd(10,12)

oxyd_shuffle()

set_actor("ac-blackball", 16.5, 1.5)
