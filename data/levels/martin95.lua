--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh+1)

fill_floor("fl-hay")
fill_floor("fl-wood", 0, 4, levelw-4, levelh-8)

draw_border("st-marble", 0,0, levelw, levelh)
draw_stones("st-marble", {0,13}, {1,0}, levelw)
draw_border("st-bombs", 1,1, levelw-2, levelh-2)

fill_items( "it-burnable", 3, 3, levelw-6, levelh-6)

set_item("it-dynamite", 10,3)

set_item("it-puller-n", 5, 10)
set_item("it-extinguisher", 5,12, {load=10})
set_stone("st-marble_move", 5, 12)

oxyd( 0, 6)
oxyd(19, 6)
oxyd( 6, 0)
oxyd( 6,12)
oxyd(13, 0)
oxyd(13,12)

oxyd_shuffle()


set_actor("ac-blackball", 9.5,5.5)
