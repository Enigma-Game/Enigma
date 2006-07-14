--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


oxyd_default_flavor = "d"

Require("levels/lib/ant.lua")

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
cells["M"]=cell{floor="fl-metal"}
cells["I"]=cell{floor="fl-ice_001"}
cells["R"]=cell{floor="fl-rough_slow"}
cells["D"]=cell{floor="fl-dunes"}
cells["S"]=cell{floor="fl-space"}
cells["S"]=cell{floor="fl-space"}
cells["C"]=cell{floor="fl-sand"}
cells["#"]=cell{stone="st-likeoxyda"}
cells["d"]=cell{{{document, "Friction and acceleration depend on the floor type."}}}
cells["o"]=cell{parent=cells["C"],actor={"ac-blackball", {player=0}}}
if (not difficult) then 
    cells["Q"]=cell{stone="st-likeoxydd"}
else
    cells["Q"]=cell{{{oxyd}}}
end

level = {
   "##########################################################",
   "#                  #                  #                  #",
   "# 2111111111111114 # 2111111111111114 # 2111111111111114 #",
   "# 1IIIIIIIIIIIIII1 # 1RRRRRRRRRRRRRR1 # 1DDDDDDDDDDDDDD1 #",
   "# 1IIIIIIIIIIIIII1 # 1RRRRRRRRRRRRRR1 # 1DDDDDDDDDDDDDD1 #",
   "# 1II0IIIIIIIIIII1   1RR0RRRRRRRRRRR1   1DD0DDDDDDDDDDD1 #",
   "# 1IIIIIIIIIIIIII11111RRRRRRRRRRRRRR11111DDDDDDDDDDDDDD1 #",
   "# 1IIIIIIIIIIIQII1   1RRRRRRRRRRRQRR1   1DDDDDDDDDDDQDD1 #",
   "# 1IIIIIIIIIIIIII1 # 1RRRRRRRRRRRRRR1 # 1DDDDDDDDDDDDDD1 #",
   "# 1IIIIIIIIIIIIII1 # 1RRRRRRRRRRRRRR1 # 1DDDDDDDDDDDDDD1 #",
   "# 3111111111111115 # 3111111111111115 # 3111111111111115 #",
   "#        1         #         1        #         1        #",
   "######## 1 ################# 1 ################ 1 ########",
   "#        1         #         1        #         1        #",
   "# 2111111111111114 # 21111111d1111114 # 2111111111111114 #",
   "# 1MMMMMMMMMMMMMM1 # 1CCCCCCCCCCCCCC1 # 1SSSSSSSSSSSSSS1 #",
   "# 1MMMMMMMMMMMMMM1 # 1CCCCCCCCCCCCCC1 # 1SSSSSSSSSSSSSS1 #",
   "# 1MM0MMMMMMMMMMM1   1CC0CCCCCCCCCCC1   1SS0SSSSSSSSSSS1 #",
   "# 1MMMMMMMMMMMMMM1111dCCCCCCCoCCCCCCd1111SSSSSSSSSSSSSS1 #",
   "# 1MMMMMMMMMMMQMM1   1CCCCCCCCCCCQCC1   1SSSSSSSSSSSQSS1 #",
   "# 1MMMMMMMMMMMMMM1 # 1CCCCCCCCCCCCCC1 # 1SSSSSSSSSSSSSS1 #",
   "# 1MMMMMMMMMMMMMM1 # 1CCCCCCCCCCCCCC1 # 1SSSSSSSSSSSSSS1 #",
   "# 3111111111111115 # 3111111111111115 # 3111111111111115 #",
   "#                  #                  #                  #",
   "##########################################################"
}


set_default_parent(cells["1"])

create_world_by_map(level,cells)

oxyd_shuffle()

