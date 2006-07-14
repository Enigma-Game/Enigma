--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 57, 36)
oxyd_default_flavor = "c"

fill_floor( "fl-abyss")

draw_border( "st-death")

fill_floor( "fl-wood", 21, 14, 8, 5)
fill_floor( "fl-brick", 23, 20, 3, 8)
fill_floor( "fl-wood", 28, 26, 5, 7)
fill_floor( "fl-brick", 34, 21, 6, 7)
fill_floor( "fl-metal", 41, 29, 5, 4)
fill_floor( "fl-wood", 47, 18, 6, 9)
fill_floor( "fl-normal", 44, 8, 5, 8)
fill_floor( "fl-plank", 35, 4, 8, 7)
fill_floor( "fl-wood", 24, 4, 8, 7)
fill_floor( "fl-brick", 12, 4, 9, 7)
fill_floor( "fl-wood", 6, 12, 6, 7)
fill_floor( "fl-wood", 13, 17, 3, 3)
fill_floor( "fl-plank", 13, 22, 3, 3)
fill_floor( "fl-wood", 11, 25, 2, 2)
fill_floor( "fl-metal", 4, 23, 6, 4)
fill_floor( "fl-plank", 8, 29, 5, 4)
fill_floor( "fl-wood", 15, 30, 12, 2)

oxyd(13, 5)
oxyd( 4,26)
oxyd(29,16)
oxyd(30,33)
oxyd(42, 4)
oxyd(52,26)

oxyd_shuffle()

set_item( "it-spring2", 25, 17)
set_item( "it-spring1", 24, 25)

set_actor("ac-blackball", 26, 17)

display.SetFollowMode(display.FOLLOW_SCREENSCROLLING)
















