--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 13)
oxyd_default_flavor = "b"

fill_floor( "fl-water")
fill_floor( "fl-sand", 1,1,18,11)
fill_floor( "fl-water", 8,5, 3,3)
set_floor( "fl-water", 3,4)
set_floor( "fl-water", 6,2)
set_floor( "fl-water", 13,9)
set_floor( "fl-water", 15,3)
set_floor( "fl-water", 16,10)
set_floor( "fl-water", 17,9)
set_floor( "fl-water", 9,8)
set_floor( "fl-water", 11,5)
set_floor( "fl-water", 5,6)
set_floor( "fl-water", 5,9)


oxyd(1,3)
oxyd(16,2)
oxyd(4,10)
oxyd(6,1)
oxyd(17,11)
oxyd(12,7)
oxyd_shuffle()

local ac = set_actor("ac-blackball", 3.3, 8.7)
local st = set_stone("st-grate1", 9, 6)

AddRubberBand( ac, st, 8, 1)















