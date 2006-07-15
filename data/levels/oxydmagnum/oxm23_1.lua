-- Black Holes -- Oxyd Magnum -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-03-13

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
multiplayer_mode();

cells={}
use_cells(cells, "2")

cells[" "]=cell{floor="fl-wood"}
cells["_"]=cell{floor="fl-normal"}
cells["#"]=cell{stone="st-brick"}
cells["s"]=cell{item= "it-spring1"}

holes={}
targets={}
worm_hole_pair(cells, "A", "a", cells[" "], cells[" "], holes, targets, 1)
worm_hole_pair(cells, "B", "b", cells["_"], cells["_"], holes, targets, 2)
worm_hole_pair(cells, "C", "c", cells["_"], cells["_"], holes, targets, 3)
worm_hole_pair(cells, "D", "d", cells["_"], cells["_"], holes, targets, 4)
worm_hole_pair(cells, "E", "e", cells["_"], cells["_"], holes, targets, 5)
cells["d"]=cell{{cells["2"], cells["d"]}} -- add an actor to cell "d"

slopes={}
pivots={}
cells["*"]=cell{{{add_multicell, slopes, 1}}}
cells["&"]=cell{{{add_multicell, pivots, slopes}, cells["_"]}}

level = {
   "....................",
   "....................",
   "....................",
   "....................",
   "##########.....##0##",
   "#         .....#   #",
   "#s       1.. A.# a #",
   "#         .....#   #",
   "##########.....##0##",
   "....................",
   "....................",
   "....................",
   "....................", -- screen edge
   "....................",
   ".*******....*******.",
   ".*B_&e0*....*0b&_C*.",
   ".*******....*******.",
   "....................",
   "....................",
   ".*******....*******.",
   ".*E_&d0*....*0c&_D*.",
   ".*******....*******.",
   "....................",
   "....................",
   "...................."
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

set_default_parent(cells[" "])
oxyd_default_flavor = "c"
create_world_by_map(level)
render_wormholes(holes, targets, {strength=10, range=5})
spread_tag(pivots)
render_slopes(slopes, 1)
oxyd_shuffle()











