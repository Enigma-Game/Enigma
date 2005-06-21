--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


oxyd_default_flavor = "a"

dofile(enigma.FindDataFile("levels/ant.lua"))

cells={}

cells[" "]=cell{floor="fl-leaves"}
cells["3"]=cell{floor="fl-leavesd1"}
cells["4"]=cell{floor="fl-leavesd2"}
cells["5"]=cell{floor="fl-leavesd3"}
cells["6"]=cell{floor="fl-leavesd4"}
cells["#"]=cell{stone="st-likeoxyda"}
cells["d"]=cell{{{document, "Some stones look exactly like the Oxyd stones!"}}}
cells["o"]=cell{actor={"ac-blackball", {player=0}}}
if (not difficult) then
    cells["Q"]=cell{parent=cells["#"]}
else
    cells["Q"]=cell{{{oxyd}}}
end

level = {
   "###0########0#######",
   "#3546 3546 3546 354#",
   "#46 3546 3546 3546 #",
   "0 3546 3546 3546 35#",
   "#546 3546 3546 3546Q",
   "#6 3546d3546 3546 3#", 
   "#3546 3546o3546 354#",
   "#46 3546 3546 3546 #",
   "# 3546 3546 3546 35#",
   "Q546 3546 3546 3546#",
   "#6 3546 3546 3546 3#",
   "#3546 3546 3546 3540",
   "##Q####0####Q##0####"
}


set_default_parent(cells[" "])

create_world_by_map(level,cells)

oxyd_shuffle()
