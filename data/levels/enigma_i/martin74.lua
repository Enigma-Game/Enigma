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
fill_floor("fl-water", 15,4,3,3)
fill_floor("fl-water", 8,1,1,11)

set_item("it-spring2", 3,8)
set_item("it-spring1", 18,9)

oxyd(16,5)
oxyd( 3,9)

oxyd_shuffle()

set_actor("ac-blackball", 2.3, 2.3)















