--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 25

create_world(levelw, levelh)

fill_floor("fl-tigris")
draw_border("st-marble", 0,0, levelw, levelh)

fill_items( "it-burnable", 1, 1, levelw-2, levelh-2)
set_item("it-burnable_ignited", 10,3)

draw_items("it-extinguisher", {7,12}, {1,0},6)

draw_stones("st-wood", {1,12}, {1,0},levelw-2)

set_item("it-blackbomb", 1, 6)
set_item("it-blackbomb", 1,18)
set_item("it-blackbomb",18, 6)
set_item("it-blackbomb",18,18)

oxyd( 0, 6)
oxyd( 0,18)
oxyd(19, 6)
oxyd(19,18)
oxyd( 6, 0)
oxyd( 6,12)
oxyd( 6,24)
oxyd(13, 0)
oxyd(13,12)
oxyd(13,24)

oxyd_shuffle()


set_actor("ac-blackball", 7.5,6.5)
