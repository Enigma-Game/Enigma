--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)

oxyd_default_flavor = "a"

draw_border("st-rock1")
fill_floor("fl-bluegreen")

fill_stones("st-rock1", 9,1, 10,11)
fill_stones("st-rock1", 1,8, 9,4)
fill_stones("st-rock1", 1,5, 2,3)
fill_stones("st-rock1", 8,3, 1,6)

set_stone("st-rock1", 3, 3)
set_stone("st-rock1", 5, 3)
set_stone("st-rock1", 6, 3)
set_stone("st-rock1", 6, 2)
set_stone("st-rock1", 6, 5)
set_stone("st-rock1", 4, 5)
set_stone("st-rock1", 3, 5)

set_stone("st-wood",3,2)
set_stone("st-wood",2,4)
set_stone("st-wood",4,3)
set_stone("st-wood",4,6)

set_item("it-trigger", 1,1, {action="openclose", target="door1"})
set_item("it-trigger", 1,2, {action="openclose", target="door2"})
set_item("it-trigger", 2,1, {action="openclose", target="door3"})
set_item("it-trigger", 2,2, {action="openclose", target="door4"})

set_stone("st-door_a", 9,2, {type="v", name="door1"})
set_stone("st-door_a", 9,6, {type="v", name="door2"})
set_stone("st-door_a",17,2, {type="v", name="door3"})
set_stone("st-door_a",17,6, {type="v", name="door4"})

fill_stones("st-rock1_hole", 10, 2, 7,1)
fill_stones("st-rock1_hole", 17, 3, 1,3)
fill_stones("st-rock1_hole", 10, 6, 7,1)
fill_stones("st-rock1_hole",  9, 8, 5,1)
fill_stones("st-rock1_hole", 10,10, 7,1)
set_stone("st-rock1_hole", 9,7)
set_stone("st-rock1_hole",13,9)

oxyd( 9,10)
oxyd(17,10)

oxyd_shuffle()

set_actor("ac-blackball", 5.5, 6.5)
