--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


oxyd_default_flavor = "d"

dofile(enigma.FindDataFile("levels/lib/ant.lua"))

cells={}

cells[" "]=cell{floor="fl-leavesb"}
cells["3"]=cell{floor="fl-leavese1"}
cells["4"]=cell{floor="fl-leavese4"}
cells["5"]=cell{floor="fl-leavese1"}
cells["6"]=cell{floor="fl-leavese3"}
cells["#"]=cell{stone="st-likeoxyda"}
if (not difficult) then 
    cells["X"]=cell{parent=cells[" "]}
else
    cells["X"]=cell{stone="st-glass3"}
end
cells["x"]=cell{stone="st-glass3"}
cells["d"]=cell{{{document, "Take your time..."}}}
cells["o"]=cell{actor={"ac-blackball", {player=0}}}
if (not difficult) then
    cells["Q"]=cell{parent=cells["#"]}
else
    cells["Q"]=cell{{{oxyd}}}
end

level = {
   "##Q####0####0####Q##",
   "#3546X3546x3546X354#",
   "046x3546X3546x3546 0",
   "#x3546X3546x3546x35#",
   "#546X3546X3546X3546#",
   "#6x3546x3546d3546X3#", 
   "#3546X3546o3546x354#",
   "#46X3546d3546d3546x#",
   "#x3546X3546X3546x35#",
   "#546X3546X3546X3546#",
   "06x3546x3546x3546X30",
   "#3546x3546X3546X354#",
   "##Q####0####0####Q##"
}


set_default_parent(cells[" "])

create_world_by_map(level,cells)

oxyd_shuffle()

