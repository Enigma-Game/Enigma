--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)
draw_border("st-bluegray")
draw_border("st-bluegray", 0,5,6,3)

oneway( 5,6, EAST)

fill_floor("fl-space")

fill_floor("fl-bluegray", 1,1,5,11)

SetDefaultAttribs("it-wormhole", {range=4, strength=20})

set_item("it-wormhole", 7,3,{targetx=3, targety=10})
set_item("it-wormhole", 7,9,{targetx=3, targety=3})
set_item("it-wormhole", 11,5,{targetx=3, targety=10})
set_item("it-wormhole", 11,7,{targetx=3, targety=3})
set_item("it-wormhole", 14,4,{targetx=3, targety=10})
set_item("it-wormhole", 14,8,{targetx=3, targety=3})
set_item("it-wormhole", 16,6,{targetx=3, targety=6.5})


oxyd(1,1)
oxyd(1,6)
oxyd(1,11)
oxyd(18,1)
oxyd(18,6)
oxyd(18,11)

oxyd_shuffle()

set_actor("ac-blackball", 3, 3)
