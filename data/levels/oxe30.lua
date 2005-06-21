-- Meditation -- Oxyd Extra -- the Enigma Level
-- (c) 2002 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-21

dofile(enigma.FindDataFile("levels/ant.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
function get_hollow_1() return hollows[1] end
function get_hollow_2() return hollows[2] end
function get_hollow_3() return hollows[3] end
function get_hollow_4() return hollows[4] end
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
-- and finally, the map itself
cells={}
cells["."]=abyss
cells[" "]=cell{floor="fl-rough"}
cells["#"]=cell{{{checkerfloor, {cell{stone="st-marble"},cell{stone="st-rock6"}}}}}
cells["o"]=cell{actor={"ac-whiteball-small", {player=0, mouseforce=1}}}
hollows = {}
cells["O"]=cell{{{add_multiitem, "it-hollow", hollows}}}
cells["1"]=cell{item={"it-trigger", {action="trigger", target="it-hollow1"}}}
cells["2"]=cell{item={"it-trigger", {action="trigger", target="it-hollow2"}}}
cells["3"]=cell{item={"it-trigger", {action="trigger", target="it-hollow3"}}}
cells["4"]=cell{item={"it-trigger", {action="trigger", target="it-hollow4"}}}

level = {
   "....................",
   ".....  ## .## ......",
   "...    ##  ##     ..",
   ".    ##O2##1O##    .",
   "  #####3o##o4#####  ",
   " ##### ##  ## ##### ",
   " ##### ##  ## ##### ",
   "  #####1o##o2#####  ",
   ".    ##O4##3O##     ",
   ".. ... ##  ##   .  .",
   "..... ###  ### .....",
   "..... ## .  ##  ....",
   "....................",
}


-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

set_default_parent(cells[" "])
create_world_by_map(level)

