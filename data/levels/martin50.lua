--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 13)
oxyd_default_flavor = "d"

draw_border("st-marble")
draw_stones("st-marble", { 9,1}, {0,1}, 11)
draw_stones("st-marble", {10,1}, {0,1}, 11)

fill_floor("fl-dunes", 0,0, 10,13)
fill_floor("fl-marble", 10,0,10,13)


set_item( "it-magnet", 2, 2, {name="magnet1"})
set_item( "it-trigger", 12, 2, {action="onoff", target="magnet1"})

set_item( "it-magnet", 7, 2, {name="magnet2"})
set_item( "it-trigger", 17, 2, {action="onoff", target="magnet2"})

set_item( "it-magnet", 5, 6, {name="magnet3"})
set_item( "it-trigger", 15, 6, {action="onoff", target="magnet3"})

set_item( "it-magnet", 2, 10, {name="magnet4"})
set_item( "it-trigger", 12, 10, {action="onoff", target="magnet4"})

set_item( "it-magnet", 7, 10, {name="magnet5"})
set_item( "it-trigger", 17, 10, {action="onoff", target="magnet5"})


oxyd( 0, 1)
oxyd( 4, 0)
oxyd( 8, 0)
oxyd( 0,11)
oxyd( 1,12)
oxyd( 8,12)

oxyd_shuffle()

set_actor("ac-blackball", 14, 2)
set_actor("ac-whiteball", 5.5, 6.5)
