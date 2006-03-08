-- The Bug -- Oxyd Extra -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-05-11
-- 2003-06-21 -- fixed

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
cells={}
multiplayer_mode()
use_cells(cells, "2")

cells[" "]=cell{floor="fl-floor_001"}
cells["#"]=cell{stone="st-rock3"}
cells["2"]=cell{cells["2"];item="it-hammer"}
cells["*"]=cell{stone="st-stone_break"}
cells["<"]=cell{stone={"st-oneway_white-w"}}
--cells["@"]=cell{{{wormhole, 16.5, 6.5, {range=1, strength=10}}}}

level = {
   ".......#############",
   "########           #",
   "0           #   #  #",
   "######## #         #",
   "0*******    #   #  #",
   "######## #  O      #",
   "0      <    0 # 2 ##",
   "######## #  o      #",
   "0*******    #   #  #",
   "######## #         #",
   "0           #   #  #",
   "########           #",
   ".......#############"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

set_default_parent(cells[" "])
oxyd_default_flavor = "c"
create_world_by_map(level)
oxyd_shuffle()










