--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


levelw = 20
levelh = 13

create_world( levelw, levelh)
draw_border("st-death")
fill_floor("fl-space")
fill_floor("fl-tigris", 9,1,10,11)

draw_stones("st-death", {9,6}, {1,0}, 10)
set_stone("st-death", 7,6)

set_item("it-magnet", 8,6, {name="magnet"})
set_stone("st-switch", 13, 6, {action="onoff", target="magnet"})

oxyd( 17, 2)
oxyd( 17, 4)
oxyd( 17, 8)
oxyd( 17,10)

oxyd( 15, 2)
oxyd( 15, 4)
oxyd( 15, 8)
oxyd( 15,10)

oxyd_shuffle()

set_actor("ac-blackball", 16.5, 4.5)
