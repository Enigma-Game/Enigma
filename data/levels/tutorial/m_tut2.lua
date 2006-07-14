--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


oxyd_default_flavor = "d"

dofile(enigma.FindDataFile("levels/lib/ant.lua"))

cells={}

cells[" "]=cell{floor="fl-leavesb"}
cells["3"]=cell{floor="fl-leavesc1"}
cells["4"]=cell{floor="fl-leavesc2"}
cells["5"]=cell{floor="fl-leavesc3"}
cells["6"]=cell{floor="fl-leavesc4"}
cells["#"]=cell{stone="st-likeoxyda"}
cells["d"]=cell{{{document, "Remember the colors."}}}
cells["o"]=cell{actor={"ac-blackball", {player=0}}}
if (not difficult) then
    cells["Q"]=cell{parent=cells["#"]}
else
    cells["Q"]=cell{{{oxyd}}}
end

level = {
   "##Q####0####0####Q##",
   "#3546 3546 3546 354#",
   "046 3546 3546d3546 0",
   "# 3546 3546 3546 35#",
   "#546 3546 3546 3546#",
   "#6 3546d3546 3546 3#", 
   "#3546 3546o3546 354#",
   "#46 3546 3546 3546 #",
   "# 3546 3546 3546d35#",
   "#546 3546 3546 3546#",
   "06 3546 3546 3546 30",
   "#3546 3546 3546 354#",
   "##Q####0####0####Q##"
}


set_default_parent(cells[" "])

create_world_by_map(level,cells)

oxyd_shuffle()

