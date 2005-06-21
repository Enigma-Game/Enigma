--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

create_world( 20, 13)
oxyd_default_flavor = "d"

fill_floor( "fl-brick")
fill_floor( "fl-space", 1,7,18,5)
fill_floor( "fl-brick", 9,9,2,1)

draw_border( "st-brick")
draw_stones( "st-brick", {1,6}, {1,0}, 18)

set_stone("st-death", 1, 1)
set_stone("st-death",18, 1)
set_stone("st-death", 1, 5)
set_stone("st-death",18, 5)
set_stone("st-death", 6, 7)
set_stone("st-death",13, 7)

oxyd(1,7)
oxyd(1,10)
oxyd(18,7)
oxyd(18,10)
oxyd(3,7)
oxyd(16,7)
oxyd(4,11)
oxyd(15,11)
oxyd(8,7)
oxyd(9,7)
oxyd(10,7)
oxyd(11,7)

oxyd_shuffle()

local ac1 = set_actor("ac-blackball", 10, 4.5)
local ac2 = set_actor("ac-whiteball", 10, 9.5, {player=0, mouseforce=0})

AddRubberBand( ac1, ac2, 6, 5)
