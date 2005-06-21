--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.
 
create_world(20, 13)
draw_border("st-actorimpulse", 0, 0, 13, 13)
fill_floor("fl-space")
draw_border("st-glass3", 15,4,5,5)
fill_floor("fl-trigger", 16,5,3,3)

set_item( "it-trigger", 16, 6, {action="onoff", target="magnet3"})
set_item( "it-trigger", 18, 6, {action="onoff", target="magnet4"})
set_item( "it-trigger", 17, 5, {action="onoff", target="magnet1"})
set_item( "it-trigger", 17, 7, {action="onoff", target="magnet2"})

SetDefaultAttribs("it-magnet", {strength=1.0})
set_item( "it-magnet",  6, 1, {name="magnet1"})
set_item( "it-magnet",  6,11, {name="magnet2"})
set_item( "it-magnet",  1, 6, {name="magnet3"})
set_item( "it-magnet", 11, 6, {name="magnet4"})

set_stone("st-death", 0, 6)
set_stone("st-death",12, 6)
set_stone("st-death", 6, 0)
set_stone("st-death", 6,12)

oxyd_default_flavor = "d"

oxyd( 1, 1)
oxyd( 1,11)
oxyd(11, 1)
oxyd(11,11)

oxyd_shuffle()

set_actor("ac-blackball", 17.5,6.5)

set_actor("ac-whiteball", 6.5,6.5, {player=0})
