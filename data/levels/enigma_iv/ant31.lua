-- Atrocity -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-03-13

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
multiplayer_mode()

cells={}
use_cells(cells, "O")

cells[" "]=cell{}
cells["-"]=cell{{{gradient, FLAT_FORCE_S}}}
cells["_"]=cell{{{gradient, FLAT_FORCE_N}}}
cells["#"]=cell{stone="st-rock2"}
cells["%"]=cell{stone="st-scissors"}
cells["w"]=cell{stone="st-white4"}
cells["b"]=cell{stone="st-black4"}

stones={}
actors={}
cells["?"]=cell{{{add_multistone, "st-rock2", stones}}}
cells["2"]=cell{{{add_multiactor, "ac-whiteball", actors, {player=1, mouseforce=0}}}}
cells["O"]=cell{{{add_multiobject, actors, cells["O"]}}}

slopes={}
pivots={}
cells["*"]=cell{{{add_multicell, slopes, 1}}}
cells["&"]=cell{{{add_multicell, pivots, slopes}, cells["~"]}}

level = {
   "#0#0#0#0###DDDDDDDD#",
   "#bbbbbbbb##XXXXXXXX#",
   "#******************#",
   "#*&  ?  ****  %  &*#",
   "#*   O  *##*      *#",
   "#*      *##*      *#",
   "#*      *##*      *#",
   "#*      *##*      *#",
   "#*      *##*  2   *#",
   "#*   %  ****  ?   *#",
   "#******************#",
   "#XXXXXXXX##wwwwwwww#",
   "#DDDDDDDD###0#0#0#0#"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

set_default_parent(function(x,y) if (x>=10) then cells["_"](x,y) else cells["-"](x,y) end end)
oxyd_default_flavor = "d"
create_world_by_map(level)

add_rubber_band_pairs(actors, stones, 10, 0)

spread_tag(pivots)
render_slopes(slopes)

oxyd_shuffle()

















