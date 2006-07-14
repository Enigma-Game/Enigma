-- Dependants -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-08
-- 2003-02-10 -- keeping up to date with latest additions to ant.lua

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells[" "]=cell{floor={face="fl-sahara"}}
cells["#"]=cell{stone={face="st-rock3"}}

puzzles = {}
cells["*"]=cell{parent={{add_multicell, puzzles}}}

smallmarbles={}
bigmarbles={}
cells["y"]=cell{item={face="it-yinyang"}}
cells["o"]=cell{parent={cells[" "],{add_multiactor, "ac-whiteball-small", smallmarbles, {player=0, mouseforce=0}}}}
cells["O"]=cell{parent={cells["y"],{add_multiactor, "ac-blackball", bigmarbles, {player=0}}}}
cells["Q"]=cell{parent={cells["y"],{add_multiactor, "ac-whiteball", bigmarbles, {player=1}}}}

cells["A"]=cell{stone={face="st-door", attr={name="doorA", type="h"}}}
cells["B"]=cell{stone={face="st-door", attr={name="doorB", type="h"}}}
cells["a"]=cell{item={face="it-trigger", attr={action="openclose", target="doorA"}}}
cells["b"]=cell{item={face="it-trigger", attr={action="openclose", target="doorB"}}}

cells["0"]=oxyd

level = {
   "##########################################################",
   "#                  #O                 #                  #",
   "#                  #                  #                  #",
   "#                  #A##################                  #",
   "#                  # a         ********************      #",
   "#                  0           *   *  0                  #",
   "#                  0           * o *  0                  #",
   "#                  0           *   *  0                  #",
   "#               ********************b #                  #",
   "#                  ##################B#                  #",
   "#                  #                  #                  #",
   "#                  #                 Q#                  #",
   "##########################################################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
set_default_parent(cells[" "])
create_world_by_map(level)
render_puzzles(puzzles);
add_rubber_bands(smallmarbles, bigmarbles, 10)
oxyd_shuffle()
enigma.ConserveLevel = FALSE


















