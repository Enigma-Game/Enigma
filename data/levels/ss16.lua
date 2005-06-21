-- A level for Enigma
-- Name:	Seven Friends
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

dofile(enigma.FindDataFile("levels/ant.lua"))
cells={}

cells[" "]=cell{floor="fl-wood"}
cells["#"]=cell{stone="st-brick"}
cells["b"]=cell{stone="st-actorimpulse"}
cells["h"]=cell{parent=cells[" "],item="it-hollow"}
cells["x"]=cell{actor={"ac-blackball",{player=0,essential=1}}}

level = {
--  01234567890123456789
   "#b################b#",--00
   "b                  b",--01
   "#  h               #",--02
   "#                  #",--03
   "#                  #",--04
   "#                  #",--05
   "# x    h           #",--06
   "#                  #",--07
   "#        h         #",--08
   "#                  #",--09
   "#            h     #",--10
   "b                  b",--11
   "#b################b#" --12
}
set_default_parent(cells[" "])

create_world_by_map(level,cells)

local top=set_actor("ac-top", 9,5, {mouseforce=0, range=20, force=-20, attacknearest=0})
local ac1=set_actor("ac-whiteball-small", 9,6, {player=1, mouseforce=1, controllers=1, essential=1})
local ac2=set_actor("ac-whiteball-small", 9,7, {player=1, mouseforce=1, controllers=1, essential=1})
local ac3=set_actor("ac-whiteball-small", 9,8, {player=1, mouseforce=1, controllers=1, essential=1})
local ac4=set_actor("ac-whiteball-small", 9,9, {player=1, mouseforce=1, controllers=1, essential=1})
local rotor=set_actor("ac-rotor", 9,11, {mouseforce=2, range=10, force=-20, attacknearest=0})

AddRubberBand(top, ac1, 6, 1)
AddRubberBand(ac1, ac2, 6, 1)
AddRubberBand(ac2, ac3, 6, 1)
AddRubberBand(ac3, ac4, 6, 1)
AddRubberBand(ac4, rotor, 3, 5)


