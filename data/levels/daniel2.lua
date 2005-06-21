-- Copyright (C) 2003 Daniel Heck 
-- Licensed under GPL v2.0 or above
levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "c"       -- Default flavor for oxyd stones.

enigma.ConserveLevel = FALSE
fill_floor("fl-gray")
draw_border("st-greenbrown")
draw_border("st-greenbrown", 0,5,3,3)
draw_border("st-greenbrown", 17,5,3,3)
oxyd(1,1)
oxyd(7,1)
oxyd(12,1)
oxyd(18,1)
oxyd(3,6)
oxyd(16,6)
oxyd(1,11)
oxyd(7,11)
oxyd(12,11)
oxyd(18,11)
oxyd_shuffle()

local ac=set_actor("ac-blackball", 10,6.5)
local st1=set_stone("st-brownie", 1,6)
local st2=set_stone("st-brownie", 18,6)
AddRubberBand(ac, st1, 15)
AddRubberBand(ac, st2, 15)


