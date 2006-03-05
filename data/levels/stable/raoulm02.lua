-- A meditation level for Enigma
-- Copyright (C) 2005 Raoul
-- Licensed under GPL v2.0 or above

-- GENERAL --
levelw = 20
levelh = 13
create_world(levelw, levelh)
enigma.ConserveLevel = FALSE

-- DESIGN
rand = "st-glass"
boden = "fl-abyss"
ring = "fl-sahara"

-- FLOOR --
fill_floor(boden, 0,0,20,13)

--ecken aussen
set_floor("fl-gradient5",1,1)
set_floor("fl-gradient7",1,11)
set_floor("fl-gradient6",18,1)
set_floor("fl-gradient8",18,11)

--ecken innen
set_floor("fl-gradient12",3,3)
set_floor("fl-gradient11",3,9)
set_floor("fl-gradient10",16,3)
set_floor("fl-gradient9",16,9)

--rand aussen
draw_floor("fl-gradient3",{1,2},{0,1},9)
draw_floor("fl-gradient4",{18,2},{0,1},9)
draw_floor("fl-gradient1",{2,1},{1,0},16)
draw_floor("fl-gradient2",{2,11},{1,0},16)

--rand innen
draw_floor("fl-gradient4",{3,4},{0,1},5)
draw_floor("fl-gradient3",{16,4},{0,1},5)
draw_floor("fl-gradient2",{4,3},{1,0},12)
draw_floor("fl-gradient1",{4,9},{1,0},12)

--boden
draw_floor(ring,{2,2},{0,1},9)
draw_floor(ring,{17,2},{0,1},9)
draw_floor(ring,{3,2},{1,0},14)
draw_floor(ring,{3,10},{1,0},14)

if difficult then
  x1=random(13)
  x2=random(13)
  x3=random(6)
  x4=random(6)
  --oben
  set_floor("fl-gradient3",2+x1,2)
  set_floor("fl-gradient4",2+x1+1,2)
  --unten
  set_floor("fl-gradient3",2+x2,10)
  set_floor("fl-gradient4",2+x2+1,10)
  --links
  set_floor("fl-gradient1",2,2+x3)
  set_floor("fl-gradient2",2,2+x3+1)
  --rechts
  set_floor("fl-gradient1",17,2+x4)
  set_floor("fl-gradient2",17,2+x4+1)
end

-- ITEMS --
set_item("it-hollow", 2, 2);
set_item("it-hollow", 2, 10);
set_item("it-hollow", 17, 2);
set_item("it-hollow", 17, 10);

set_actor("ac-whiteball-small", 10,2.5, {player=0})
set_actor("ac-whiteball-small", 10,10.5, {player=0})
set_actor("ac-whiteball-small", 2.5,6.5, {player=0})
set_actor("ac-whiteball-small", 17.5,6.5, {player=0})


























