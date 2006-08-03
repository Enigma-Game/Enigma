-- Spiders -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-09

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells["."]=abyss
cells[" "]=cell{floor="fl-himalaya"}

cells["#"]=cell{stone="st-metal"}
cells["="]=cell{stone="st-glass"}

actors={}
cells["y"]=cell{item={face="it-yinyang"}}
cells["O"]=cell{parent={cells["y"],{add_multiactor, "ac-blackball", actors, {player=0}}}}
cells["Q"]=cell{parent={cells["y"],{add_multiactor, "ac-whiteball", actors, {player=1}}}}

cells["A"]=cell{item={face="it-trigger", attr={action="trigger", target="oxydA"}}}
cells["B"]=cell{item={face="it-trigger", attr={action="trigger", target="oxydB"}}}
cells["C"]=cell{item={face="it-trigger", attr={action="trigger", target="oxydC"}}}
cells["D"]=cell{item={face="it-trigger", attr={action="trigger", target="oxydD"}}}
cells["E"]=cell{item={face="it-trigger", attr={action="trigger", target="oxydE"}}}
cells["F"]=cell{item={face="it-trigger", attr={action="trigger", target="oxydF"}}}
cells["G"]=cell{item={face="it-trigger", attr={action="trigger", target="oxydG"}}}
cells["H"]=cell{item={face="it-trigger", attr={action="trigger", target="oxydH"}}}
cells["I"]=cell{item={face="it-trigger", attr={action="trigger", target="oxydI"}}}
cells["J"]=cell{item={face="it-trigger", attr={action="trigger", target="oxydJ"}}}

cells["a"]=cell{stone={face="st-oxyd", attr={flavor="b", color="0", name="oxydA"}}}
cells["b"]=cell{stone={face="st-oxyd", attr={flavor="b", color="1", name="oxydB"}}}
cells["c"]=cell{stone={face="st-oxyd", attr={flavor="b", color="2", name="oxydC"}}}
cells["d"]=cell{stone={face="st-oxyd", attr={flavor="b", color="3", name="oxydD"}}}
cells["e"]=cell{stone={face="st-oxyd", attr={flavor="b", color="4", name="oxydE"}}}
cells["f"]=cell{stone={face="st-oxyd", attr={flavor="b", color="0", name="oxydF"}}}
cells["g"]=cell{stone={face="st-oxyd", attr={flavor="b", color="1", name="oxydG"}}}
cells["h"]=cell{stone={face="st-oxyd", attr={flavor="b", color="2", name="oxydH"}}}
cells["i"]=cell{stone={face="st-oxyd", attr={flavor="b", color="3", name="oxydI"}}}
cells["j"]=cell{stone={face="st-oxyd", attr={flavor="b", color="4", name="oxydJ"}}}

level = {
   "####################",
   "#.   .=.    .=.   .#",
   "#. A .=. af .=. F .#",
   "#.   .=.    .=.   .#",
   "#. B .=. bg .=. G .#",
   "#.  O.=.    .=.   .#",
   "#. C .=. ch .=. H .#",
   "#.   .=.    .=.Q  .#",
   "#. D .=. di .=. I .#",
   "#.   .=.    .=.   .#",
   "#. E .=. ej .=. J .#",
   "#.   .=.    .=.   .#",
   "####################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
set_default_parent(cells[" "])
create_world_by_map(level)
add_rubber_bands(actors, actors, 10, 11)
oxyd_shuffle()




















