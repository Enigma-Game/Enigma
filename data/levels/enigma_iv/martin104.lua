--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

Require("levels/lib/ant.lua")

cells={}

cells[" "]=cell{floor="fl-sand"}
cells["#"]=cell{floor="fl-water"}
cells["."]=cell{item="it-landmine"}
cells["D"]=cell{item={"it-document", {text="! -  DANGER  -  NO TRESPASSING  -  DANGER  - !"}}}

cells["o"]=cell{actor={"ac-blackball", {player=0, mouseforce=1}}}

level = {
   "#######################################",
   "######                                #",
   "#   ###                    .          #",
   "#     ##         .        ...         #",
   "#       ##         .     ..0..        #",
   "#   .     #               ...         #",
   "#          #               .          #",
   "#      .    #                         #",
   "#     .0.    #                        #",
   "#      .     #                        #",
   "#   .       #     ...                 #",
   "#           #                   .     #",
   "#           #                         #",
   "#          #                          #",
   "#           # ####  #                 #",
   "#            # .  ## ###              #",
   "#     .       .   .   . ##            #",
   "#           .    #  .     #           #",
   "#          ....##    ######           #",
   "#         ....#   .  #      .         #",
   "#         ..0# .    #                 #",
   "#  .      ..###    . # o # .          #",
   "# .0.      ..####     ###     .       #",
   "#  .          ####  .  D .            #",
   "#######################################"
}


set_default_parent(cells[" "])

create_world_by_map(level,cells)
                                                                                                                              
oxyd_shuffle()

if not difficult then -- easy
	display.SetFollowMode(display.FOLLOW_SCROLLING)
end















