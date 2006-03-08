-- Copyright (C) 2003 Daniel Heck
-- Licensed under GPL v2.0 or above
levelw = 20
levelh = 13

create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
fill_floor("fl-rough")
draw_border("st-rock1")

set_item("it-hollow", 5, 6);
set_item("it-hollow", 14, 6);

local ac1=set_actor("ac-whiteball-small", 7,6.5, {player=0})
local ac2=set_actor("ac-whiteball-small", 13,6.5, {player=0})

AddRubberBand(ac1, ac2, 4, 4)











