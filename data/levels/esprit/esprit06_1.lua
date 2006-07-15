--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 13)
oxyd_default_flavor = "a"

fill_floor( "fl-bluegray")
draw_border("st-wood_001")

oxyd( 2, 2)
oxyd( 4, 2)
oxyd( 6, 2)
oxyd(13, 2)
oxyd(15, 2)
oxyd(17, 2)
oxyd( 2,10)
oxyd( 4,10)
oxyd( 6,10)
oxyd(13,10)
oxyd(15,10)
oxyd(17,10)
oxyd_shuffle()

set_actor("ac-blackball", 10, 6.5)











