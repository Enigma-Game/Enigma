-- Pull Me -- Oxyd Magnum -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-03-08

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
use_cells(cells, "W")
cells[" "]=cell{floor="fl-rough"}
cells["#"]=cell{stone="st-rock6"}
cells["v"]=cell{item ="it-puller-s"}
cells["^"]=cell{item ="it-puller-n"}
cells["<"]=cell{item ="it-puller-w"}
cells[">"]=cell{item ="it-puller-e"}
cells["Y"]=cell{{cells["W"],cells["^"]}}
cells["Z"]=cell{{cells["W"],cells[">"]}}

level = {
   "##0############0####",
   "#                  #",
   "#                  #",
   "# WY      <    <   #",
   "#  W    W   W      #",
   "#    ^         W   #",
   "#     Z < O<       0",
   "#  W               #",
   "#                  #",
   "#        W    W  v #",
   "#W    <            #",
   "0W                 #",
   "####################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

set_default_parent(" ")
oxyd_default_flavor = "c"
create_world_by_map(level)
oxyd_shuffle()










