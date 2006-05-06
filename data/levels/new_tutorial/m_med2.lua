--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


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
cells["d"]=cell{{{document, "The blue stones are oxyd stones. Open them by touching them using your black marble."}}}
cells["o"]=cell{actor={"ac-whiteball-small", {player=0}}}
cells["+"]=cell{item="it-hollow"}
if (not difficult) then 
    cells["q"]=cells["1"]
    cells["*"]=cells["1"]
else
    cells["q"]=cell{actor={"ac-whiteball-small", {player=0}}}
    cells["*"]=cell{item="it-hollow"}
end

level = {
   "####################",
   "#                  #",
   "# 2111111111111114 #",
   "# 1*16  8116  81*1 #",
   "# 311  # 11 #  115 #",
   "#  81  # 11 #  16  #", 
   "### 1 ## 11 ## 1 ###",
   "#  91 #  11  # 17  #",
   "# 211 # 2qq4 # 114 #",
   "# 1+1 # 1oo1 # 1+1 #",
   "# 315 # 3115 # 315 #",
   "#     #      #     #",
   "####################"
}


set_default_parent(cells["1"])

create_world_by_map(level,cells)

oxyd_shuffle()
