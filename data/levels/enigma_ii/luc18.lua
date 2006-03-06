-- More Black Holes, a level for Enigma
-- Copyright (C) 2005 Lukas Schueller
-- Licensed under GPL v2.0 or above
-- Thanks to Petr Machata for his ant.lua script
-----------------------------------------------
-- Environment
Require("levels/lib/ant.lua")
cells={}
cells[","]=cell{floor="fl-gravel"}
cells[" "]=cell{floor="fl-space"}
cells["#"]=cell{stone="st-invisible"}
map_cell_meaning("x", cell{item={"it-wormhole", {targetx="0.5", targety="0.5", range=3, strength=10}}})
level = {
".###################",
"#0    x      x    0#",
"#                  #",
"#   x    xx    x   #",
"#                  #",
"#      ,,,,,,      #",
"#0  x  ,,,,,,  x  0#",
"#      ,,,,,,      #",
"#                  #",
"#   x    xx    x   #",
"#                  #",
"#0    x      x    0#",
"####################"
}
oxyd_default_flavor = "d"
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()
set_actor("ac-blackball", 10,6.5, {player="0"})























