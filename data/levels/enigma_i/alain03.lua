-- A meditation level for Enigma
-- Copyright (C) 2005 Alain Busser
-- Licensed under GPL v2.0 or above 

Require("levels/lib/ant.lua")

cells={}

cells[" "]=cell{floor="fl-leaves"}
cells["#"]=cell{stone="st-likeoxyda"}
cells["o"]=cell{actor={"ac-whiteball-small", {player=0}}}
cells["+"]=cell{item="it-hollow"}

level = {
   "####################",
   "#        oo        #",
   "# ################ #",
   "# #              # #",
   "# # #####  ##### # #",
   "# # #          # # #", 
   "#   # # # +# # #   #",
   "# # #          # # #",
   "# # #####  ##### # #",
   "# #              # #",
   "# ################ #",
   "#        +         #",
   "####################"
}


set_default_parent(cells[" "])

create_world_by_map(level,cells)

oxyd_shuffle()































