--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 13)
oxyd_default_flavor = "d"

draw_border("st-brownie")
fill_floor("fl-metal")

oxyd( 6,11)
oxyd( 7,11)
oxyd( 8,11)
oxyd( 9,11)
oxyd(10,11)
oxyd(11,11)
oxyd(12,11)
oxyd(13,11)

oxyd_shuffle()

document( 18, 11, "Easy?")

set_actor("ac-blackball", 10, 1.5)
