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
cells["d"]=cell{{{document, "Prepare for even more mouse gymnastics in the next levels."}}}
cells["o"]=cell{actor={"ac-blackball", {player=0}}}
if (not difficult) then
    cells["Q"]=cell{parent=cells["#"]}
else
    cells["Q"]=cell{{{oxyd}}}
end

level = {
   "####################",
   "#     #            #",
   "01114 # 211111111110",
   "#d111 # 16    81111#",
   "Q1111 # 1 #### 1111Q",
   "#1111 # 17   # 1111#",
   "01o11 # 3114 # 11110",
   "#1111 #   81 # 1111#",
   "Q1111 #### 1 # 1111Q",
   "#1d117    91 # 1111#",
   "011111111115 # 31110",
   "#            #     #",
   "####################"
}


set_default_parent(cells["1"])

create_world_by_map(level,cells)

oxyd_shuffle()
