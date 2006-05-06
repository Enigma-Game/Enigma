--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


oxyd_default_flavor = "d"

dofile(enigma.FindDataFile("levels/lib/ant.lua"))

cells={}

cells[" "]=cell{floor="fl-leavesb"}
cells["1"]=cell{floor="fl-swamp"}
cells["#"]=cell{stone="st-likeoxyda"}
cells["d"]=cell{{{document, "Don't drown in the swamp"}}}
cells["o"]=cell{actor={"ac-blackball", {player=0}}}

level = {
   "####################",
   "#     11111111     #",
   "# d   111111111  d #",
   "0   o 1111111111   0",
   "#     11111111111  #",
   "#    1111111111111 #", 
   "#111111111111111111#",
   "#    11111111111111#",
   "#     1111111111111#",
   "0     11111111111110",
   "# d   1111111111111#",
   "#     1111111111111#",
   "####################"
}


set_default_parent(cells[" "])

create_world_by_map(level,cells)

oxyd_shuffle()
