-- Chained -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-02
-- 2003-02-10 -- keeping up to date with latest additions to ant.lua

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells["."]=abyss
cells[" "]=cell{floor="fl-metal"}
cells["_"]=cell{floor="fl-normal"}
cells["#"]=cell{stone="st-metal"}
cells["x"]=cell{stone="st-grate1"}
cells["0"]=oxyd

blocks = {};
actors = {};
cells["W"]=cell{{cells["_"],{add_multistone, "st-wood", blocks}}}
cells["O"]=cell{{cells["_"],{add_multiactor, "ac-blackball", actors, {player=0}}}}

level = {
   "#0#####......#####0#",
   "0     #......#     0",
   "#     #......#     #",
   "#     #......#     #",
   "#     ########     #",
   "#xxxxxxxxxxxxxxxxxx#",
   "#________W_O_______#",
   "#xxxxxxxxxxxxxxxxxx#",
   "#     ########     #",
   "#     #......#     #",
   "#     #......#     #",
   "0     #......#     0",
   "#0#####......#####0#"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor = "b"
set_default_parent(cells[" "])
create_world_by_map(level)
add_rubber_bands(actors, blocks, 50, 0)
oxyd_shuffle()

















