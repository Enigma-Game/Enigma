--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


oxyd_default_flavor = "d"

dofile(enigma.FindDataFile("levels/lib/ant.lua"))

cells={}

cells[" "]=cell{floor="fl-leaves"}
cells["1"]=cell{floor="fl-water"}
cells["#"]=cell{stone="st-likeoxyda"}
cells["d"]=cell{{{document, "Careful - you can not swim in the water."}}}
cells["o"]=cell{actor={"ac-blackball", {player=0}}}

level = {
   "####################",
   "#o     d  11111    #",
   "#   d  11 11111    #",
   "#11111 11 11111  1 #",
   "#1111  11 11111  1 #",
   "#111  111 11111  1 #", 
   "#11  1111 1111  11 #",
   "#1  11         11  #",
   "#  11       1111   #",
   "0 11   d   1111    #",
   "#11       11    d  0",
   "#1       11        #",
   "##0########0########"
}


set_default_parent(cells[" "])

create_world_by_map(level,cells)

oxyd_shuffle()
