--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


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
cells["d"]=cell{{{document, "Move all small marbles into the holes. Only one marble per hole is allowed."}}}
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
   "#      2o11o4      #",
   "#      111111      #",
   "#     91q11q17     #",
   "#    211d11d114    #", 
   "#   911111111117   #",
   "#  211+111111+114  #",
   "#  1*1111111111*1  #",
   "#  31111111111115  #",
   "#                  #",
   "#                  #",
   "####################"
}


set_default_parent(cells["1"])

create_world_by_map(level,cells)

oxyd_shuffle()
