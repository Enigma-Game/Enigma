--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 39, 13)
oxyd_default_flavor = "c"

fill_floor( "fl-abyss")

fill_floor( "fl-samba", 3, 3, 16, 1)
fill_floor( "fl-samba", 18, 4, 1, 8)
fill_floor( "fl-samba", 13, 11, 5, 1)
fill_floor( "fl-samba", 13, 6, 1, 5)
fill_floor( "fl-samba", 6, 6, 7, 1)
fill_floor( "fl-samba", 6, 7, 1, 4)
fill_floor( "fl-samba", 1, 11, 10, 1)
fill_floor( "fl-samba", 1, 1, 1, 10)
fill_floor( "fl-samba", 2, 1, 8, 1)
set_floor( "fl-samba", 4, 2)

draw_items( "it-tinyhill", {5,3}, {2,0}, 7)
draw_items( "it-hollow", {1, 1}, {0,1}, 10)
draw_items( "it-hollow", {18, 3}, {0,1},9)
draw_items( "it-tinyhollow", {4, 1}, {0,1},3)
set_item( "it-hill", 7, 1)
set_item( "it-hill", 6, 7)
set_item( "it-tinyhollow", 5, 11)
set_item( "it-hollow", 13, 6)

oxyd(10, 1)
oxyd( 3, 4)
oxyd(11,11)
oxyd(10, 5)
oxyd(17, 4)
oxyd(17, 8)

oxyd_shuffle()

set_actor("ac-blackball", 6.5, 3.5)















