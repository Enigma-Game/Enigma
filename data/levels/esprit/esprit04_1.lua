--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 13)
oxyd_default_flavor = "b"

fill_floor( "fl-abyss")

draw_floor("fl-gradient", {1,5}, {1,0}, 9, {type=3})
draw_floor("fl-gradient", {10,5}, {1,0}, 9, {type=4})
draw_floor("fl-gradient", {1,6}, {1,0}, 9, {type=3})
draw_floor("fl-gradient", {10,6}, {1,0}, 9, {type=4})
draw_floor("fl-gradient", {1,7}, {1,0}, 9, {type=3})
draw_floor("fl-gradient", {10,7}, {1,0}, 9, {type=4})

draw_border("st-fakeoxyd", 0, 5, level_width, 3)

oxyd( 2, 5)
oxyd( 2, 7)
oxyd( 7, 5)
oxyd( 7, 7)
oxyd(12, 5)
oxyd(12, 7)
oxyd(17, 5)
oxyd(17, 7)
oxyd_shuffle()

set_actor("ac-blackball", 10, 6.5)











