-- Copyright (C) 2003 Daniel Heck
-- Licensed under GPL v2.0 or above
levelw = 20
levelh = 13

create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
fill_floor("fl-abyss")
fill_floor("fl-metal", 2,2,16,9)

set_item("it-hollow", 7,4)
set_item("it-hollow", 5,7)
set_item("it-hollow", 9,6)
set_item("it-hollow", 13,5)

draw_floor("fl-gradient", {17,3}, {0,1}, 7, {type=3})
draw_floor("fl-gradient", {2,3}, {0,1}, 7, {type=4})
draw_floor("fl-gradient", {3,2}, {1,0}, 14, {type=2})
draw_floor("fl-gradient", {3,10}, {1,0}, 14, {type=1})
set_floor("fl-gradient", 17,10, {type=9})
set_floor("fl-gradient", 17,2, {type=10})
set_floor("fl-gradient", 2,10, {type=11})
set_floor("fl-gradient", 2,2, {type=12})

local ac1=set_actor("ac-whiteball-small", 8,5, {player=0})
local ac2=set_actor("ac-whiteball-small", 10,5, {player=0})
local ac3=set_actor("ac-whiteball-small", 10,7, {player=0})
local ac4=set_actor("ac-whiteball-small", 10,9, {player=0})

AddRubberBand(ac1, ac2, 6, 1)
AddRubberBand(ac2, ac3, 6, 1)
AddRubberBand(ac2, ac4, 6, 1)

