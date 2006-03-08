--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 13)
oxyd_default_flavor = "a"

stone = "st-bluegray"
floor = "fl-normal"

fill_floor( "fl-space")

fill_floor( floor, 9,6, 2,1)

set_floor( floor, 1, 1)
set_floor( floor,18, 1)
set_floor( floor, 1,11)
set_floor( floor,18,11)

set_floor( floor, 6, 2)
set_floor( floor,13, 2)
set_floor( floor, 6,10)
set_floor( floor,13,10)

fill_floor( floor, 4, 1, 12,1)
fill_floor( floor, 4,11, 12,1)

fill_floor( floor, 1, 3,  1,7)
fill_floor( floor,18, 3,  1,7)

draw_border( stone)

set_stone( "st-death", 1, 5)
set_stone( "st-death", 1, 7)

set_stone( "st-death",18, 5)
set_stone( "st-death",18, 7)

set_stone( "st-death", 9, 1)
set_stone( "st-death",10, 1)

set_stone( "st-death", 9,11)
set_stone( "st-death",10,11)

oxyd( 3, 0)
oxyd( 6, 0)
oxyd(13, 0)
oxyd(16, 0)
oxyd( 3,12)
oxyd( 6,12)
oxyd(13,12)
oxyd(16,12)

oxyd_shuffle()

set_item( "it-magnet", 1, 1, {name="magnet1"})
set_item( "it-magnet",18, 1, {name="magnet2"})
set_item( "it-magnet", 1,11, {name="magnet3"})
set_item( "it-magnet",18,11, {name="magnet4"})

set_stone( "st-switch", 0, 2, {action="onoff", target="magnet2"})
set_stone( "st-switch",19, 2, {action="onoff", target="magnet1"})
set_stone( "st-switch", 0,10, {action="onoff", target="magnet4"})
set_stone( "st-switch",19,10, {action="onoff", target="magnet3"})


set_actor("ac-blackball", 10, 6.5)












