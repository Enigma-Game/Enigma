-- Wild -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-09

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
cells[" "]=cell{floor="fl-metal"}
cells["#"]=cell{stone="st-rock1"}
cells["S"]=cell{stone="st-rock1"}

actors={}
cells["O"]=cell{{{add_multiactor, "ac-blackball", actors, {player=0}}}}
cells["Q"]=cell{{{add_multiactor, "ac-whiteball", actors, {player=1}}}}
cells["0"]=oxyd

cells["V"]=cell{{{gradient, 1}}}
cells["^"]=cell{{{gradient, 2}}}
cells[">"]=cell{{{gradient, 3}}}
cells["<"]=cell{{{gradient, 4}}}
cells["T"]=cell{{{gradient, 5}}}
cells["L"]=cell{{{gradient, 7}}}
cells["R"]=cell{{{gradient, 6}}}
cells["/"]=cell{{{gradient, 8}}}

level = {
   "####################",
   "#TVVVVVVVVVVVVVVVVR#",
   "0>                <0",
   "#>                <#",
   "0>                <0",
   "#>       Q        <#",
   "0>                <0",
   "#>        O       <#",
   "0>                <0",
   "#>                <#",
   "0>                <0",
   "#L^^^^^^^^^^^^^^^^/#",
   "####################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor = "a"
set_default_parent(cells[" "])
create_world_by_map(level)
add_rubber_bands(actors, actors, 100, 0)
oxyd_shuffle()


















