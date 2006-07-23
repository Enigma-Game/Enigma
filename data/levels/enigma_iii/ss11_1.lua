-- A level for Enigma
-- Name:	Blind Date
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

world.DefineSimpleStoneMovable("invisible_move", "st-rock2", 0)

display.DefineAlias("invisible_move", "st-invisible")

Require("levels/lib/ant.lua")
cells={}

cells[" "]=cell{floor="fl-leaves"}
cells["#"]=cell{stone="st-invisible"}
cells["h"]=cell{parent=cells[" "],item="it-hollow"}
cells["g"]=cell{parent=cells[" "],stone="st-grate1"}
cells["S"]=cell{parent=cells[" "],stone="st-swap"}
cells["i"]=cell{parent=cells[" "],stone="invisible_move"}
cells["x"]=cell{actor={"ac-whiteball-small", {player=0, mouseforce=1, essential=1}}}

level = {
   "####################",
   "#               #gh#",
   "#  x       i    #gg#",
   "#               ####",
   "#                  #",
   "#         S        #",
   "#                  #",
   "#                  #",
   "#  ##              #",
   "### i              #",
   "# i i            x #",
   "#h  i              #",
   "####################"
}


set_default_parent(cells[" "])

create_world_by_map(level,cells)






















