-- Inverse -- Oxyd Extra -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-05-11

Require("levels/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
cells[" "]=cell{floor="fl-inverse"}
cells["#"]=cell{stone="st-rock1"}
cells["."]=cell{item= {"it-crack1", {fixed=1}}}
cells[","]=cell{item= {"it-crack2", {fixed=1}}}
cells[":"]=cell{item= {"it-crack3", {fixed=1}}}

level = {
   "####################",
   "#       :          #",
   "#      ,           #",
   "#  0            0  #",
   "#                . #",
   "#   D .        D   #",
   "#          O       #",
   "#   D          D   #",
   "#    ,             #",
   "#  0            0  #",
   "#           :,     #",
   "#,                 #",
   "####################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

set_default_parent(cells[" "])
oxyd_default_flavor = "c"
create_world_by_map(level)
oxyd_shuffle()
