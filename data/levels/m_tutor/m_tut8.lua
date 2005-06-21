--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


oxyd_default_flavor = "d"

dofile(enigma.FindDataFile("levels/ant.lua"))

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
cells["d"]=cell{{{document, "Levels can be larger than one screen."}}}
cells["o"]=cell{actor={"ac-blackball", {player=0}}}
if (not difficult) then 
    cells["Q"]=cell{parent=cells["#"]}
else
    cells["Q"]=cell{{{oxyd}}}
end

level = {
   "##########################################################",
   "#       #                                        #       #",
   "0111114 # 21111111111111111111111111111111111114 # 2111110",
   "#11d111 # 16                                  81 # 111111#",
   "Q111111 # 1 ################################## 1 # 111111Q",
   "#111111 # 17                 #                91 # 111111#",
   "0111o11 # 311111111111111114 # 21111111111111115 # 1111110",
   "#111111 #                 81 # 16                # 111111#",
   "Q111111 ################## 1 # 1 ################# 111111Q",
   "#111d117                  91 # 17                  111111#",
   "0111111111111111111111111115 # 311111111111111111111111110",
   "#                            #                           #",
   "##########################################################"
}


set_default_parent(cells["1"])

create_world_by_map(level,cells)

oxyd_shuffle()
