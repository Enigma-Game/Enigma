--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

Require("levels/lib/ant.lua")

cells={}

cells[" "]=cell{floor="fl-metal"}
cells["#"]=cell{stone="st-rock2"}
cells["~"]=cell{floor="fl-water"}
if not difficult then -- easy
	cells["-"]=cells[" "]
else
	cells["-"]=cells["~"]
end
cells["w"]=cell{item="it-weight"}
cells["k"]=cell{item="it-floppy"}
cells["<"]=cell{stone="st-oneway-w"}
cells[">"]=cell{stone="st-oneway-e"}
cells["S"]=cell{stone={"st-floppy", {action="openclose", target="door1"}}}
cells["="]=cell{{{doorh, {name="door1"}}}}
cells["o"]=cell{parent=cells[" "],actor={"ac-blackball", {player=0, mouseforce=1}}}

level = {
   "####################",
   "#                  #",
   "#                o #",
   "#                  #",
   "#   ###########~~~~#",
   "#   >wwwwwwwwwww~~~#",
   "#   ###########w~~~#",
   "#   <kwwwwwwwwww~~~#",
   "##=S###########~~~~#",
   "#                  #",
   "# -- -- -  - -- -- #",
   "# -- -- -  - -- -- #",
   "#0##0##0#00#0##0##0#"
}


set_default_parent(cells[" "])

create_world_by_map(level,cells)

oxyd_shuffle()


















