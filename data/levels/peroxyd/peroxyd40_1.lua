-- Copyright (C) 2003 Daniel Heck
-- Licensed under GPL v2.0 or above
levelw = 20
levelh = 13

create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
fill_floor("fl-water")
fill_floor("fl-leaves", 1,1, level_width-2,level_height-2)
--draw_border("st-rock2")


set_item("it-hollow", 4,4)
set_item("it-hollow", 7, 6)
set_item("it-hollow", 12, 6)
set_item("it-hollow", 15, 8)

local ac1=set_actor("ac-whiteball-small", 10,3, {player=0})
local ac2=set_actor("ac-whiteball-small", 10,5, {player=0})
local ac3=set_actor("ac-whiteball-small", 10,7, {player=0})
local ac4=set_actor("ac-whiteball-small", 10,9, {player=0})

AddRubberBand(ac1, ac2, 6, 1)
AddRubberBand(ac2, ac3, 6, 1)
AddRubberBand(ac3, ac4, 6, 1)












