-- Meditation -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-02
-- 2003-02-09 -- fixes in object groups
-- 2003-02-10 -- rubber band fix

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells[" "]=cell{floor={face="fl-metal"}}

puzzles = {};
cells["#"]=cell{parent={{add_multicell, puzzles}}}

cells["O"]=cell{item={face="it-hollow"}}
cells["a"]=cell{item={face="it-trigger", attr={action="openclose", target="doorA"}}}
cells["b"]=cell{item={face="it-trigger", attr={action="openclose", target="doorB"}}}
cells["c"]=cell{item={face="it-trigger", attr={action="openclose", target="doorC"}}}
cells["d"]=cell{item={face="it-trigger", attr={action="openclose", target="doorD"}}}
cells["A"]=cell{parent=cells["O"], stone={face="st-door_b", attr={type="v", name="doorA"}}}
cells["B"]=cell{parent=cells["O"], stone={face="st-door_b", attr={type="v", name="doorB"}}}
cells["C"]=cell{parent=cells["O"], stone={face="st-door_b", attr={type="v", name="doorC"}}}
cells["D"]=cell{parent=cells["O"], stone={face="st-door_b", attr={type="v", name="doorD"}}}

blocks = {};
actors = {};
cells["W"]=cell{parent={{add_multistone, "st-block", blocks}}}
cells["o"]=cell{parent={{add_multiactor, "ac-whiteball-small", actors, {player=0, mouseforce=1, essential=1}}}}

level = {
   "####################",
   "#o  #          #  o#",
   "#   #          #   #",
   "#  W            W  #",
   "###       a      ###",
   "#         b        #",
   "#       AB CD      #",
   "#         c        #",
   "###       d      ###",
   "#  W            W  #",
   "#   #          #   #",
   "#o  #          #  o#",
   "####################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
set_default_parent(cells[" "])
create_world_by_map(level)
add_rubber_band_pairs(actors, blocks, 10, 0)
render_puzzles(puzzles);
oxyd_shuffle()














