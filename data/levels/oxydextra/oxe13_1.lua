-- Mirror Moves -- Oxyd Extra -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-03-07

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
cells[" "]=cell{floor="fl-bluegray"}
cells["#"]=cell{stone="st-rock5"}
cells["B"]=cell{stone="st-break_acwhite"}
cells["="]=cell{stone="st-glass"}
cells["S"]=cell{stone={"st-switch", {action="onoff",  target="laserA"}}}
cells["L"]=cell{stone={"st-laser", {on=FALSE, dir=EAST, name="laserA"}}}
cells["-"]=cell{{{mirrorp, FALSE, FALSE, 1}}}
cells["/"]=cell{{{mirrorp, TRUE,  FALSE, 4}}}
cells["<"]=cell{{{mirror3, FALSE, FALSE, 2}}}
cells[">"]=cell{{{mirror3, FALSE, FALSE, 4}}}
cells["c"]=cell{item={"it-coin1"}}
cells["0"]=oxyd

cells["O"]=cell{actor={"ac-blackball", {player=0}}}

level = {
   "##########0#######0#",
   "#   B  c  B       B#",
   "#   B    BBBB c   >#",
   "#    BB   BB       #",
   "#B    B   BB       #",
   "0BB   Bc   B    c  #",
   "#BB      c  c      #",
   "#        O      c  #",
   "#            /     #",
   "# ###===    B      #",
   "# S#L  =  - BBB   <#",
   "# ###  =     B    B#",
   "##################0#"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

set_default_parent(cells[" "])
oxyd_default_flavor = "c"
create_world_by_map(level)
oxyd_shuffle()











