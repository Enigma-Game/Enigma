--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 25)
oxyd_default_flavor = "a"

fill_floor( "fl-abyss")
fill_floor( "fl-gray", 1,2,18,21)
fill_floor( "fl-gray", 2,1,16,1)
fill_floor( "fl-gray", 2,23,16,1)

set_stone("st-actorimpulse", 1, 2)
set_stone("st-actorimpulse",18, 2)
draw_stones("st-actorimpulse", {2,1}, {1,0}, 16)

oxyd(3,3)
oxyd(16,3)
oxyd(3,19)
oxyd(16,19)
oxyd(7,21)
oxyd(12,21)
oxyd_shuffle()

local ac = set_actor("ac-blackball", 10, 5)
local st = set_stone("st-grate1", 10, 4)

AddRubberBand( ac, st, 30, 1)

enigma.AddConstantForce(0,50)











