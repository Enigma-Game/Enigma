--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

create_world( 20, 13)
enigma.Brittleness = 0.8

fill_floor( "fl-abyss", 0, 0, 20, 13)
fill_floor( "fl-wood", 1, 1, 18, 11)
fill_floor( "fl-abyss", 3, 3, 14, 7)
fill_floor( "fl-wood", 4, 4, 1, 6)
fill_floor( "fl-wood", 15, 3, 1, 6)
fill_floor( "fl-wood", 6, 3, 1, 3)
fill_floor( "fl-wood", 13, 7, 1, 3)
fill_floor( "fl-wood", 6, 6, 8, 1)

set_item ("it-crack0", 1, 6)
set_item ("it-crack0", 2, 7)
set_item ("it-crack0", 18, 6)
set_item ("it-crack0", 17, 8)


oxyd_default_flavor = "d"

oxyd( 4, 4)
oxyd( 8, 5)
oxyd(11, 5)
oxyd( 8, 7)
oxyd(11, 7)
oxyd(15, 8)

oxyd_shuffle()

set_actor("ac-blackball", 10, 6.5)










