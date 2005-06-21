--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

create_world( 20, 13)
oxyd_default_flavor = "d"

draw_border("st-rock2")
fill_floor("fl-metal")

oxyd( 5, 3)
oxyd(14, 3)
oxyd( 5, 9)
oxyd(14, 9)

oxyd_shuffle()

set_actor("ac-blackball", 10,6.5)
