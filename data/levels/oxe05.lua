-- Buzzer -- Oxyd Extra -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-03-07

Require("levels/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
cells["."]=abyss
cells[" "]=cell{floor="fl-rough"}
cells["#"]=cell{stone="st-rock3"}

doors={}
open_doors=cell{{{send_group_message, doors, "openclose", nil; mode=-1}}}
close_trap=cell{{{send_message_named, "bridgeA", "close", nil; mode=-1}}}
cells["T"]=cell{{{add_multifloor, "fl-bridge", doors, {name="bridgeA"}}}}
cells["|"]=cell{{{add_multistone, "st-door", doors, {type="v", name="doorA"}}}}
cells["C"]=cell{stone={"st-coinslot", {action="openclose", target="doorA"}}}
cells["t"]=cell{item= {"it-trigger",  {action="openclose", target="doorA"}}}
cells["S"]=cell{stone={"st-switch",   {action="callback",  target="open_doors"}}}
cells["%"]=cell{{cells["#"], close_trap}}

cells["f"]=cell{item="it-floppy"}
cells["s"]=cell{item="it-sword"}
cells["d"]=cell{item="it-dynamite"}
cells["k"]=cell{item="it-key"}
cells["c"]=cell{item={"it-coin1"}}

cells["O"]=cell{actor={"ac-blackball", {player=0}}}

cells["0"]=cell{stone={"st-oxyd", {flavor="d", color="0"}}}
cells["1"]=cell{stone={"st-oxyd", {flavor="d", color="1"}}}
cells["2"]=cell{stone={"st-oxyd", {flavor="d", color="2"}}}
cells["3"]=cell{stone={"st-oxyd", {flavor="d", color="3"}}}

level = {
   "#######################################",
   "#      f          ###                 #",
   "#                 ###                 #",
   "#k           s    C##                 #",
   "#  0  1           ###           2  3  #",
   "#                 ###                 #",
   "#   t     c       T|                  #",
   "#           O     ###                 #",
   "#  0  1           ###           2  3  #",
   "#           d     S##                 #",
   "#                 ###                 #",
   "#                 ###                 #",
   "######################################%"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

set_default_parent(cells[" "])
create_world_by_map(level)
