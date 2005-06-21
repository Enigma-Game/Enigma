--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 13)
oxyd_default_flavor = "c"

fill_floor( "fl-abyss")

fill_floor( "fl-metal", 0, 2, 7, 9)
fill_floor( "fl-metal",13, 2, 7, 9)

fill_floor( "fl-gradient", 7, 4, 6, 1, {type=1})
fill_floor( "fl-metal", 7, 6, 6, 1)
fill_floor( "fl-gradient", 7, 8, 6, 1, {type=2})

draw_stones( "st-woven", { 0,2}, {0,1}, 9)
draw_stones( "st-woven", {19,2}, {0,1}, 9)

draw_stones( "st-oneway-w", { 6,2}, {0,1}, 5)
draw_stones( "st-oneway-e", { 6,7}, {0,1}, 4)
draw_stones( "st-oneway-w", {13,2}, {0,1}, 4)
draw_stones( "st-oneway-e", {13,7}, {0,1}, 4)

set_stone( "st-woven", 13, 6)

oxyd( 0, 4)
oxyd( 0, 6)
oxyd( 0, 8)
oxyd(19, 4)
oxyd(19, 6)
oxyd(19, 8)

oxyd_shuffle()

document( 5, 6, "You need to be fast!")

set_actor("ac-blackball", 10, 6.5)
