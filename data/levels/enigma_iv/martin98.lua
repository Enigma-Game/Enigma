--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


dofile(enigma.FindDataFile("levels/lib/ant.lua"))

cells={}

cells[" "]=cell{floor="fl-rough-blue"}
if (not difficult) then
	cells["#"]=cell{stone="st-invisible",floor="fl-rough-red"}
else
	cells["#"]=cell{stone="st-death_invisible",floor="fl-rough-red"}
end

cells["x"]=cell{actor={"ac-rotor", {range=25, force=40}}}
cells["o"]=cell{actor={"ac-blackball", {player=0, mouseforce=1}}}

level = {
   "#######################################",
   "#                                     #",
   "# o             0               0     #",
   "#                      #              #",
   "#                                     #",
   "#       0                             #",
   "#                                     #",
   "#                    0         #      #",
   "#   #                          x      #",
   "#                                     #",
   "#         x #                         #",
   "#                                     #",
   "#                                     #",
   "#                        0       #    #",
   "#  0                                  #",
   "#              0                      #",
   "#                                     #",
   "#                                     #",
   "#     #                 #             #",
   "#                                     #",
   "#           0                         #",
   "#                                     #",
   "#  0            #             0    x  #",
   "#                                     #",
   "#######################################"
}


set_default_parent(cells[" "])

create_world_by_map(level,cells)
                                                                                                                              
oxyd_shuffle()















