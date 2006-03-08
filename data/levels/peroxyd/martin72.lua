--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

create_world( 20, 13)
oxyd_default_flavor = "a"

fill_floor( "fl-dunes")

draw_border( "st-rock5")
draw_border( "st-white3", 3, 2, 14, 9)

oxyd(1,2)
oxyd(1,10)
oxyd(18,2)
oxyd(18,10)
oxyd(3,1)
oxyd(15,1)
oxyd(3,11)
oxyd(15,11)
oxyd_shuffle()

local ac1 = set_actor("ac-blackball", 10, 5)
local ac2 = set_actor("ac-whiteball", 10, 4, {player=0, mouseforce=0})

AddRubberBand( ac1, ac2, 30, 1)










