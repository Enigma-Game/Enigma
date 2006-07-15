-- NAME, a level for Enigma
-- Copyright (C) YEAR AUTHOR
-- Licensed under GPL v2.0 or above 

-- GENERAL --
Require("levels/lib/libterrain.lua")

levelw = 58
levelh = 31

create_world(levelw, levelh)

display.SetFollowMode(display.FOLLOW_SCROLLING)

fill_floor("fl-abyss")

draw_ring(1,1,56,29,1)
draw_ring(2,2,54,27,1)
draw_ring(3,3,52,25,1)

fill_floor("fl-water",4,4,50,23)

draw_ring(5,5,48,21,0)
draw_ring(6,6,46,19,0)
draw_ring(7,7,44,17,0)

draw_full(8,8,7,7,0)
draw_full(15,8,7,7,1)
draw_full(22,8,7,7,0)
draw_full(29,8,7,7,1)
draw_full(36,8,7,7,0)
draw_full(43,8,7,7,1)

draw_full(8,16,7,7,1)
draw_full(15,16,7,7,0)
draw_full(22,16,7,7,1)
draw_full(29,16,7,7,0)
draw_full(36,16,7,7,1)
draw_full(43,16,7,7,0)

set_floor("fl-gray",11,11)
set_floor("fl-gray",18,11)
set_floor("fl-gray",25,11)
set_floor("fl-gray",32,11)
set_floor("fl-gray",39,11)
set_floor("fl-gray",46,11)

set_floor("fl-gray",11,19)
set_floor("fl-gray",18,19)
set_floor("fl-gray",25,19)
set_floor("fl-gray",32,19)
set_floor("fl-gray",39,19)
set_floor("fl-gray",46,19)

fill_floor("fl-gray",8,15,42,1)

--oxyds:
oxyd(18,11)
oxyd(32,11)
oxyd(46,11)

oxyd(11,19)
oxyd(25,19)
oxyd(39,19)

oxyd_shuffle()

-- Special
enigma.SlopeForce=50
set_actor("ac-blackball", 23.5,13.5, {player=0})  

