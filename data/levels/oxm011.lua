-- Marbles & Mountains -- Oxyd Magnum -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-03-13

Require("levels/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
cells={}
cells[" "]=cell{floor="fl-normal"}
cells["#"]=cell{stone="st-greenbrown"}
cells["v"]=cell{item= "it-tinyhollow"}
cells["^"]=cell{item= "it-hill"}
cells["+"]=cell{item= "it-tinyhill"}
cells["o"]=cell{actor={"ac-killerball", {player=0, controllers=1,mouseforce=1}; mode=2}, parent=cells["+"]}
cells["O"]=cell{actor={"ac-blackball", {player=0}; mode=2}}


slopes={}
pivots={}
cells["*"]=cell{{{add_multicell, slopes, 1}}}
cells["&"]=cell{{{add_multicell, pivots, slopes}, cells[" "]}}

level = {
   "####################",
   "#******************#",
   "#*v   ^  &   ^   v*#",
   "#*  0          0  *#",
   "#*       +o       *#",
   "#*                *#",
   "#* ^   0  O 0   ^ *#",
   "#*                *#",
   "#*       +o       *#",
   "#*  0          0  *#",
   "#*v   ^      ^   v*#",
   "#******************#",
   "####################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

set_default_parent(cells[" "])
oxyd_default_flavor = "c"
create_world_by_map(level)
spread_tag(pivots)
render_slopes(slopes, 1)
oxyd_shuffle()
