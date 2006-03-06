-- A level for Enigma
-- Name:	Something Went Wrong
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

dofile(enigma.FindDataFile("levels/lib/ant.lua"))
cells={}

cells[" "]=cell{floor="fl-black"}
cells["#"]=cell{stone="st-brick"}
cells["b"]=cell{stone="st-actorimpulse_invisible"}
cells["S"]=cell{stone="st-swap"}
cells["h"]=cell{parent=cells[" "],item="it-hollow"}

level = {
--  01234567890123456789
   "####################",--00
   "#                  #",--01
   "# bbb          bbb #",--02
   "#  hb          bh  #",--03
   "# bbb          bbb #",--04
   "#       SDDS       #",--05
   "#       SDDS       #",--06
   "#       SDDS       #",--07
   "# bbb          bbb #",--08
   "# bh           bhb #",--09
   "# bbb          b b #",--10
   "#                  #",--11
   "####################" --12
}
set_default_parent(cells[" "])

create_world_by_map(level,cells)

set_actor("ac-whiteball-small", 8.5, 2.5, {player=0, mouseforce=-1, essential=1})
set_actor("ac-whiteball-small",11.5, 2.5, {player=0, mouseforce= 1, essential=1})
set_actor("ac-whiteball-small", 8.5,10.5, {player=0, mouseforce=-1, essential=1})
set_actor("ac-whiteball-small",11.5,10.5, {player=0, mouseforce= 1, essential=1})


















