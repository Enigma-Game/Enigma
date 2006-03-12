--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


oxyd_default_flavor = "d"

dofile(enigma.FindDataFile("levels/lib/ant.lua"))

cells={}

cells[" "]=cell{floor="fl-leavesb"}
cells["1"]=cell{floor="fl-leaves"}
cells["2"]=cell{floor="fl-leavesc1"}
cells["3"]=cell{floor="fl-leavesc2"}
cells["4"]=cell{floor="fl-leavesc3"}
cells["5"]=cell{floor="fl-leavesc4"}
cells["6"]=cell{floor="fl-leavesd1"}
cells["7"]=cell{floor="fl-leavesd2"}
cells["8"]=cell{floor="fl-leavesd3"}
cells["9"]=cell{floor="fl-leavesd4"}
cells["#"]=cell{stone="st-likeoxyda"}
cells["d"]=cell{{{document, "Learn to move your black ball properly."}}}
cells["o"]=cell{actor={"ac-blackball", {player=0}}}
if (not difficult) then
    cells["Q"]=cell{parent=cells["1"]}
else
    cells["Q"]=cell{{{oxyd}}}
end

level = {
   "####################",
   "#21114#            #",
   "#11011# 211111114  #",
   "#11Q11# 11Q16  81  #",
   "#31d15# 1111 #  1  #",
   "# 816 # 1111 #  1  #",
   "#  1  # 1o11 #  1  #",
   "#  1  # 1111 # 917 #",
   "#  1  # 1111 #21d14#",
   "#  17  91Q11 #11Q11#",
   "#  311111115 #11011#",
   "#            #31115#",
   "####################"
}


set_default_parent(cells["1"])

create_world_by_map(level,cells)

oxyd_shuffle()
