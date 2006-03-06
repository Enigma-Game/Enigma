-- Pain -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-09
-- 2003-02-10 -- keeping up to date with latest additions to ant.lua

dofile(enigma.FindDataFile("levels/lib/ant.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells["."]=cell{floor="fl-abyss"}
cells[" "]=cell{floor="fl-bluegreen"}
cells["#"]=cell{stone="st-rock3"}

actors={}
stones={}
cells["O"]=cell{{{add_multiactor, "ac-blackball", actors, {player=0}, 2}}}
cells["%"]=cell{{{add_multistone, "st-rock3", stones}}}
cells["0"]=oxyd

level = {
   "####################",
   "#..................#",
   "#.                .#",
   "#.0              0.#",
   "#.                .#",
   "##0              0##",
   "#%#       O      #%#",
   "##0              0##",
   "#.                .#",
   "#.0              0.#",
   "#.                .#",
   "#..................#",
   "####################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor = "a"
set_default_parent(cells[" "])
create_world_by_map(level)
add_rubber_bands(actors, stones, -10, 4)
oxyd_shuffle()















