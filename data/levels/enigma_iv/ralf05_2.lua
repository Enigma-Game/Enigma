-- A level for Enigma
-- Name:        Stay there!
-- Filename: 	ralf05.lua
-- Copyright: 	(C) Apr 2003 Ralf Westram
-- Contact: 	amgine@reallysoft.de
-- License: 	GPL v2.0 or above

Require("levels/lib/ralf.lua")

--debug_mode()

level={
   "!!#######y#!!!!!!!!!",
   "!!y       #!!0!!!0!!",
   "!!# #y### #!! !!! !!",
   "!!# #     y!#_#!#_#!",
   "!!# # #y##y## ### ##",
   "!!#ay   #          y",
   "#yy y## #  ## ##   #",
   "#     y #  #...#   #",
   "y xxx y     ...    #",
   "# xxx #%#  #...#   #",
   "y xxx y%%  ## ##   #",
   "y     #,#          #",
   "##y#yy############y#",
}

triggers = 0
doors = 0
doorsopen = 0

state = {0,0,0,0,0,0,0,0,0}

function trigger(num)
   state[num] = 1-state[num]
   all=1
   for x=0,triggers-1 do
      if (state[x]==0) then
         all = 0
      end
   end

   if (doorsopen==0 and all==1) then
       SendMessage("door1","open")
       SendMessage("door2","open")
       doorsopen = 1
   elseif (doorsopen==1 and all==0) then
       SendMessage("door1","close")
       SendMessage("door2","close")
       doorsopen = 0
   end
end

function set_trigger(x,y)
   triggers = triggers + 1
   state[triggers] = 0
   local funcn = "trig_"..triggers
   dostring(funcn.." = function() trigger("..triggers..") end")
   set_item("it-trigger",x,y,{action="callback",target=funcn});
end

function set_door(x,y)
   doors = doors + 1
   set_stone("st-door",x,y,{name="door"..doors,type="h"})
end

cells={}

cells[" "] = cell{floor="fl-tigris"}
cells["!"] = cell{floor="fl-space"}

cells["."] = cell{parent={cells[" "],set_trigger}}

cells["#"] = cell{parent=cells[" "],stone="st-rock1"}
cells["y"] = cell{parent=cells[" "],stone="st-stoneimpulse"}

cells["%"] = cell{parent=cells[" "],stone="st-grate1"}
cells[","] = cell{parent=cells["%"],item="it-seed"}

cells["x"] = cell{parent=cells[" "],stone="st-stoneimpulse_movable"}
cells["a"] = cell{parent=cells[" "],actor={"ac-blackball", {player=0}}}
cells["_"] = cell{parent={cells[" "],set_door}}

-- debug
--cells["."] = cell{parent={cells["x"],set_trigger}}


rs_create_world(level,cells)
















