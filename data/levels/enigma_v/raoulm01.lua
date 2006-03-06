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
boden = "fl-sahara"
spitze = "fl-sahara"

-- FLOOR --
fill_floor(boden, 0,0,20,13)

-- Orte der berge
if not difficult then
 xb1 = 3
 yb1 = 6
 xb2 = 8
 yb2 = 6
 xb3 = 15
 yb3 = 6
else
 xb1 = 1+random(2)
 yb1 = 1+random(9)
 xb2 = 6+random(2)
 yb2 = 2+random(7)
 xb3 = 13+random(2)
 yb3 = 3+random(5)
end

-- berg 1
set_floor(spitze,xb1,yb1)
set_floor("fl-gradient3",xb1+1,yb1)
set_floor("fl-gradient1",xb1,yb1+1)
set_floor("fl-gradient4",xb1-1,yb1)
set_floor("fl-gradient2",xb1,yb1-1)

set_floor("fl-gradient12",xb1-1,yb1-1)
set_floor("fl-gradient10",xb1+1,yb1-1)
set_floor("fl-gradient11",xb1-1,yb1+1)
set_floor("fl-gradient9",xb1+1,yb1+1)

-- berg 2
set_floor(spitze,xb2,yb2)
set_floor("fl-gradient3",xb2+1,yb2)
set_floor("fl-gradient1",xb2,yb2+1)
set_floor("fl-gradient4",xb2-1,yb2)
set_floor("fl-gradient2",xb2,yb2-1)

set_floor("fl-gradient12",xb2-1,yb2-1)
set_floor("fl-gradient10",xb2+1,yb2-1)
set_floor("fl-gradient11",xb2-1,yb2+1)
set_floor("fl-gradient9",xb2+1,yb2+1)

draw_floor("fl-gradient2",{xb2-1,yb2-2},{1,0},3)
draw_floor("fl-gradient3",{xb2+2,yb2-1},{0,1},3)
draw_floor("fl-gradient1",{xb2+1,yb2+2},{-1,0},3)
draw_floor("fl-gradient4",{xb2-2,yb2+1},{0,-1},3)

set_floor("fl-gradient12",xb2-2,yb2-2)
set_floor("fl-gradient10",xb2+2,yb2-2)
set_floor("fl-gradient11",xb2-2,yb2+2)
set_floor("fl-gradient9",xb2+2,yb2+2)

-- berg 3
set_floor(spitze,xb3,yb3)
set_floor("fl-gradient3",xb3+1,yb3)
set_floor("fl-gradient1",xb3,yb3+1)
set_floor("fl-gradient4",xb3-1,yb3)
set_floor("fl-gradient2",xb3,yb3-1)

set_floor("fl-gradient12",xb3-1,yb3-1)
set_floor("fl-gradient10",xb3+1,yb3-1)
set_floor("fl-gradient11",xb3-1,yb3+1)
set_floor("fl-gradient9",xb3+1,yb3+1)

draw_floor("fl-gradient2",{xb3-1,yb3-2},{1,0},3)
draw_floor("fl-gradient3",{xb3+2,yb3-1},{0,1},3)
draw_floor("fl-gradient1",{xb3+1,yb3+2},{-1,0},3)
draw_floor("fl-gradient4",{xb3-2,yb3+1},{0,-1},3)

set_floor("fl-gradient12",xb3-2,yb3-2)
set_floor("fl-gradient10",xb3+2,yb3-2)
set_floor("fl-gradient11",xb3-2,yb3+2)
set_floor("fl-gradient9",xb3+2,yb3+2)

draw_floor("fl-gradient2",{xb3-2,yb3-3},{1,0},5)
draw_floor("fl-gradient3",{xb3+3,yb3-2},{0,1},5)
draw_floor("fl-gradient1",{xb3+2,yb3+3},{-1,0},5)
draw_floor("fl-gradient4",{xb3-3,yb3+2},{0,-1},5)

set_floor("fl-gradient12",xb3-3,yb3-3)
set_floor("fl-gradient10",xb3+3,yb3-3)
set_floor("fl-gradient11",xb3-3,yb3+3)
set_floor("fl-gradient9",xb3+3,yb3+3)

-- STONES --
draw_border(rand)

-- ITEMS --
set_item("it-hollow", xb1, yb1);
set_item("it-hollow", xb2, yb2);
set_item("it-hollow", xb3, yb3);

-- ACTORS --
-- Garantiert, dass keine Kugeln schon in den Loecher sind
xp1=0+random(6)
yp1=0+random(11)
 while xp1==xb1 and yp1==yb1 do
  xp1=0+random(6)
  yp1=0+random(11)
 end

xp2=6+random(6)
yp2=0+random(11)
 while xp2==xb2 and yp2==yb2 do
  xp2=6+random(6)
  yp2=0+random(11)
 end

xp3=12+random(6)
yp3=0+random(11)
 while xp3==xb3 and yp3==yb3 do
  xp3=12+random(6)
  yp3=0+random(11)
 end

set_actor("ac-whiteball-small", xp1+.5,yp1+.5, {player=0})
set_actor("ac-whiteball-small", xp2+.5,yp2+.5, {player=0})
set_actor("ac-whiteball-small", xp3+.5,yp3+.5, {player=0})





























