-- A meditation level for Enigma
-- Copyright (C) 2005 Raoul Bourquin
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 20
levelh = 13
create_world(levelw, levelh)
enigma.ConserveLevel = FALSE

-- FLOOR --
fill_floor("fl-abyss", 0,0,20,13)

draw_floor("fl-gray",{2,2},{1,0},16)
draw_floor("fl-gray",{2,6},{1,0},16)
draw_floor("fl-gray",{2,10},{1,0},16)

draw_floor("fl-gradient1",{2,1},{1,0},16)
draw_floor("fl-gradient1",{2,5},{1,0},16)
draw_floor("fl-gradient1",{2,9},{1,0},16)

draw_floor("fl-gradient2",{2,3},{1,0},16)
draw_floor("fl-gradient2",{2,7},{1,0},16)
draw_floor("fl-gradient2",{2,11},{1,0},16)

set_floor("fl-gradient5",1,1)
set_floor("fl-gradient5",1,5)
set_floor("fl-gradient5",1,9)

set_floor("fl-gradient7",1,3)
set_floor("fl-gradient7",1,7)
set_floor("fl-gradient7",1,11)

set_floor("fl-gradient6",18,1)
set_floor("fl-gradient6",18,5)
set_floor("fl-gradient6",18,9)

set_floor("fl-gradient8",18,3)
set_floor("fl-gradient8",18,7)
set_floor("fl-gradient8",18,11)

set_floor("fl-gradient3",1,2)
set_floor("fl-gradient3",1,6)
set_floor("fl-gradient3",1,10)
set_floor("fl-gradient4",18,2)
set_floor("fl-gradient4",18,6)
set_floor("fl-gradient4",18,10)

-- ITEMS --
set_item("it-hollow", 15,2, {essential=1})
set_item("it-hollow", 10,6, {essential=1})
set_item("it-hollow", 4,10, {essential=1})

-- ACTORS --
if not difficult then
 ac1=set_actor("ac-whiteball-small", 10,2.5,{player=0,essential=1})
 ac2=set_actor("ac-whiteball-small", 10,6.5,{player=0,essential=1})
 ac3=set_actor("ac-whiteball-small", 10,10.5,{player=0,essential=1})
else
 ac1=set_actor("ac-whiteball-small", 10,2.5,{essential=0,controllers=2})
 ac2=set_actor("ac-whiteball-small", 10,6.5,{player=0,essential=1})
 ac3=set_actor("ac-whiteball-small", 10,10.5,{essential=0,controllers=2})
end

AddRubberBand(ac1,ac2,4,3)
AddRubberBand(ac2,ac3,4,3)


