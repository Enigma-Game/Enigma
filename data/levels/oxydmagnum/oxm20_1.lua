-- Meditation -- Oxyd Magnum -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-03-08

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
cells[" "]=cell{floor="fl-wood"}
cells["_"]=cell{floor="fl-normal"}
cells["o"]=cell{actor={"ac-whiteball-small", {player=0, mouseforce=1}; mode=0}, parent=cells["_"]}
cells["O"]=hollow
cells["@"]=cell{{hill,cells["_"]}}

level = {
   "....................",
   "....................",
   ".                  .",
   ". O              O .",
   ".      @@@@@@      .",
   ".      @____@      .",
   ".      @_o_o@      .",
   ".      @_o_o@      .",
   ".      @@@@@@      .",
   ". O              O .",
   ".                  .",
   "....................",
   "...................."
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

set_default_parent(cells[" "])
oxyd_default_flavor = "c"
create_world_by_map(level)
oxyd_shuffle()











