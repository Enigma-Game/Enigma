-- Island of Mirrors -- Oxyd Extra -- the Enigma Level
-- (c) 2002 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-21

Require("levels/lib/ant.lua")
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
num = 0
function engine(x, y, delta, faces, attribs, modattrib, modvalues)
   local it = {}
   it.state = 1

   num = num +1
   local fname = "switchfunc"..num

   attribs[1].action = "callback"
   attribs[1].target = fname

   local x2, y2 = x+delta[1], y+delta[2]
   set_stone(faces[1], x, y, attribs[1])
   it.tgt = set_stone(faces[2], x2, y2, attribs[2])

   local func0 = function()
		    -- switch
		    it.state = it.state +1
		    if (it.state>getn(modvalues)) then
		       it.state = 1
		    end
		    -- set state of desired target object
		    kill_stone(x2, y2)
		    attribs[2][modattrib] = modvalues[it.state]
		    set_stone(faces[2], x2, y2, attribs[2])
		    --set_attrib(it.tgt, modattrib, modvalues[it.state])
		 end

   setglobal(fname, func0)
end
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

local dlt  = {11,0} -- delta
local mir3 = {"st-switch", "st-3mirror"}
local mirp = {"st-switch", "st-pmirror"}
local lsr =  {"st-switch", "st-laser"}
local trn = {transparent=TRUE,  movable = TRUE}
local ntr = {transparent=FALSE, movable = FALSE}

cells={}
cells["."] = abyss
cells["0"] = oxyd
cells[" "] = cell{floor="fl-samba"}
cells["#"] = cell{stone="st-rock5"}
cells["O"] = cell{actor={"ac-blackball", {player=0}}}

cells["A"] = cell{{{engine, dlt, mir3, {{},{orientation=3}}, "orientation", {3,4}}}}
cells["B"] = cell{{{engine, dlt, mirp, {{},{orientation=2}}, "orientation", {2,3}}}}
cells["C"] = cell{{{engine, dlt, mir3, {{},{orientation=1}}, "orientation", {1,2}}}}
cells["D"] = cell{{{engine, dlt, mir3, {{},{orientation=1}}, "orientation", {1,2}}}}
cells["E"] = cell{{{engine, dlt, mirp, {{},{transparent=TRUE, orientation=1}}, "orientation", {1,2}}}}
cells["F"] = cell{{{engine, dlt, mir3, {{},{orientation=2}}, "orientation", {2,3}}}}
cells["G"] = cell{{{engine, dlt, mirp, {{},{orientation=1}}, "orientation", {1,2}}}}
cells["H"] = cell{{{engine, dlt, mir3, {{},{orientation=2}}, "orientation", {2,3}}}}
cells["L"] = cell{stone={"st-laser", {on=FALSE, dir=NORTH, name="laser1"}}}
cells["S"] = cell{{{switch, "laser1", "onoff"}}}

level = {
   "....................",
   "....................",
   ".#...#.......0...0..",
   ".       ....       .",
   ". A B C ....       0",
   ".       #...       .",
   ". D E F ...0       .",
   ".  O    ....       .",
   "# G   H ....       0",
   ".       ....       .",
   "....S...#......L.0..",
   "....................",
   "....................",
}
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor="c"
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()










