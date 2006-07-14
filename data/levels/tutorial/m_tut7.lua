--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


oxyd_default_flavor = "b"

dofile(enigma.FindDataFile("levels/lib/ant.lua"))

cells={}

cells[" "]=cell{floor="fl-leavesb"}
cells["3"]=cell{floor="fl-leavesd4"}
cells["4"]=cell{floor="fl-leavesd3"}
cells["5"]=cell{floor="fl-leavesd2"}
cells["6"]=cell{floor="fl-leavesd1"}
cells["#"]=cell{stone="st-death"}
if (not difficult) then 
    cells["x"]=cell{stone="st-actorimpulse"}
else
    cells["x"]=cell{stone="st-death"}
end
cells["d"]=cell{{{document, "Don't touch everything!"}}}
cells["o"]=cell{actor={"ac-blackball", {player=0}}}
if (not difficult) then 
    cells["Q"]=cell{parent=cells["#"]}
else
    cells["Q"]=cell{{{oxyd}}}
end

level = {
   "##0##Q##0##0##Q##0##",
   "#  x  35  x     x  #",
   "#     46      35   #",
   "# 35         x46   #",
   "0 46  x   35       0",
   "#      d  46     d #", 
   "# x       o        #",
   "#   35        x    #",
   "0   46   x         0",
   "#              35  #",
   "#  x   d35     46  #",
   "#       46  x      #",
   "##0##Q##0##0##Q##0##"
}


set_default_parent(cells[" "])

create_world_by_map(level,cells)

oxyd_shuffle()

