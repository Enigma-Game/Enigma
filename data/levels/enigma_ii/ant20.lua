-- Come To Me -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-01-24
-- 2003-02-09 -- fixes in object groups
-- 2003-02-10 -- change in interfacing parent functions in ant.lua, rubber band changed
-- 2003-02-20 -- rewrite
-- 2003-03-25 -- using slope generator

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

num = 0
function switchcell(x, y, boundwith, tgt)
   num = num +1
   local fname = "switchfunc"..num

   local func0 = function()
		    kill_stone(x, y)
		    local obj = {set_stone("st-death", x, y, {})}
		    add_rubber_bands(boundwith, obj, 16, 0)
		    send_group_message(tgt, "open")
		 end

   setglobal(fname, func0)

   switch(x, y, fname, "callback")
end
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
cells={}

cells["_"]=cell{floor="fl-normal"}
cells[" "]=cell{floor="fl-sand"}
cells["#"]=cell{stone="st-rock2"}

slopes={}
pivots={}
cells["*"]=cell{{{add_multicell, slopes, -1}}}
cells["%"]=cell{{{add_multicell, slopes, 1}}}
cells["&"]=cell{{{add_multicell, pivots, slopes}}}

actors = {}
cells["O"]=cell{{{add_multiactor, "ac-blackball", actors, {player=0}}}}

doors = {}
cells["b"]=cell{{cells["_"],{add_multistone, "st-door_b", doors, {type="v"}}}}
cells["B"]=cell{{cells["_"],{switchcell, actors, doors}}}

cells["0"]=oxyd

level = {
   "#0################0#",
   "0b****************b0",
   "#**              **#",
   "#*  O             *#",
   "#*     _____      *#",
   "#*     _%%%_      *#",
   "#*     _%B%_      *#",
   "#*     _%%%_  &   *#",
   "#*     _____      *#",
   "#*                *#",
   "#**              **#",
   "0b****************b0",
   "#0################0#"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
set_default_parent(cells[" "])
create_world_by_map(level)

spread_tag(pivots)
render_slopes(slopes)

oxyd_shuffle()















