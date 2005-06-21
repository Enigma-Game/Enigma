--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

create_world( 20, 13)
oxyd_default_flavor = "d"

draw_border("st-brownie")
fill_floor("fl-sand", 0,0, level_width,level_height)

oxyd( 3, 2)
oxyd(16, 2)
oxyd( 9, 6)
oxyd(10, 6)
oxyd( 3,10)
oxyd(16,10)

oxyd_shuffle()

set_item( "it-umbrella", 9, 2)
set_item( "it-brush", 10, 2)
set_actor("ac-blackball", 10, 3, {player=0})
