--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 39, 13)
oxyd_default_flavor = "c"

fill_floor( "fl-normal")

draw_border("st-bluegray")
draw_stones("st-bluegray", {19,1}, {0,1}, 11)
doorv( 19, 6, {name="door1"})
set_stone("st-coinslot", 19, 5, {action="openclose", target="door1"})
set_stone("st-death", 19, 8)

set_item("it-coin1", 5, 2)
set_item("it-coin1", 8, 5)
set_item("it-coin1", 15, 4)

oxyd( 3, 3)
oxyd(24, 3)
oxyd( 6, 9)
oxyd(33, 8)

set_actor("ac-blackball", 10, 8.5)
