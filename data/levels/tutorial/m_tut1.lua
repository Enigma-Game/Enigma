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
cells["d"]=cell{{{document, "The blue stones are Oxyd stones. Open them by touching them with your black marble."}}}
cells["o"]=cell{actor={"ac-blackball", {player=0}}}
cells["0"]=cell{{{oxyd}}}
if (not difficult) then
    cells["Q"]=cell{parent=cells["1"]}
else
    cells["Q"]=cell{{{oxyd}}}
end

level = {
   "####################",
   "#                  #",
   "#       2111111114 #",
   "#       1011111101 #",
   "# 214   1111111111 #",
   "# 1d17 91111111111 #", 
   "# 1o11111111QQ1111 #",
   "# 1d16 81111111111 #",
   "# 315   1111111111 #",
   "#       1011111101 #",
   "#       3111111115 #",
   "#                  #",
   "####################"
}


set_default_parent(cells["1"])

create_world_by_map(level,cells)

oxyd_shuffle()

