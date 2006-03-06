--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)
draw_border("st-brownie")
fill_floor("fl-sahara")
fill_floor("fl-water", 16,1,3,11)

set_item("it-spring1", 1,1)

oxyd( 17, 2)
oxyd( 17, 3)
oxyd( 17, 5)
oxyd( 17, 7)
oxyd( 17, 9)
oxyd( 17,10)

oxyd_shuffle()

set_actor("ac-blackball", 2.3, 2.3)















