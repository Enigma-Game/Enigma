--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

dofile(enigma.FindDataFile("levels/ant.lua"))

cells={}

cells[" "]=cell{floor="fl-stwood1"}
cells["."]=cell{floor="fl-stwood2"}
cells["#"]=cell{stone="st-wood"}
cells["~"]=cell{floor="fl-water"}
cells["+"]=cell{item="it-seed_volcano"}
cells["!"]=cell{floor="fl-stwood1",stone="st-chameleon",item="it-seed"}
cells["o"]=cell{parent=cells[" "],actor={"ac-blackball", {player=0, mouseforce=1}}}

level = {
   "####################",
   "#o ..  ..  .. ~..  #",
   "#.  ..  ..  ..~ .. 0",
   "#..  ..  ..  .~  ..#",
   "# ..  .. +..  ~.  .#",
   "#  ..  ..  .. ~..  0",
   "#.  ..  ..  ..~ .. #",
   "#..  ..  ..  .~  ..0",
   "# ..  ..  ..  ~.  .#",
   "#  .!  ..  .. ~..  #",
   "#.  ..  ..  ..~ .. 0",
   "#..  ..  ..  .~  ..#",
   "####################"
}


set_default_parent(cells[" "])

create_world_by_map(level,cells)

oxyd_shuffle()
