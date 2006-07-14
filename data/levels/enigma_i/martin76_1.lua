--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


levelw = 20
levelh = 13

create_world( levelw, levelh)
draw_border("st-death")
draw_stones("st-death", {5,1},{0,1},5)
draw_stones("st-death", {14,7},{0,1},5)

fill_floor("fl-space")

fill_floor("fl-metal", 1,1,4,4)
fill_floor("fl-metal", 15,8,4,4)

SetDefaultAttribs("it-wormhole", {range=50, strength=50})

set_item("it-wormhole", 13,6,{targetx=18.6, targety=10.6})
set_item("it-wormhole", 6,6,{targetx=18.6, targety=10.6})

set_item("it-hollow", 18,10)

oxyd(1,1)
oxyd(18,11)

oxyd_shuffle()

set_actor("ac-blackball", 18.6, 10.6)
















