-- A level for Enigma
-- Name:        U-Swap
-- Filename:    ralf15.lua
-- Copyright: 	(C) Jul 2003 Ralf Westram
-- Contact: 	amgine@reallysoft.de
-- License: 	GPL v2.0 or above

Require("levels/lib/ralf.lua")

level={
   "###o###########o####",
   "#_pgp_________pgp__#",
   "#p___p_______p___p_#",
   "#_____p_____p_____p#",
   "#______p___p_______#",
   "#gg_____p_p________#",
   "oag______p_________o",
   "#gg_____p_p________#",
   "# _____p___p_______#",
   "#_____p_____p_____p#",
   "#p___p_______p___p_#",
   "#_pgp_________pgp__#",
   "###o###########o####",
}

if not difficult then
   seed = 947360
else
   seed = 8375
end

randomseed(seed)

function randstone(x,y)
   local r = random(1,7)
   local st = "st-death"
   if r<3 then
      st = "st-stoneimpulse_movable"
   elseif r<5 then
      st = "st-grate1"
   elseif r<7 then
      st = "st-swap"
   elseif r==7 then
      if (random(1,6)>1) then
         if (difficult) then
            return -- no stone
         else
            st = "st-grate1"
         end
      end
   end
   set_stone(st,x,y)
end

cells={}

cells[" "] = cell{floor="fl-sahara"}
cells["#"] = cell{parent=cells[" "],stone="st-rock1"}
cells["o"] = cell{oxyd}

--cells["x"] = cell{parent=cells[" "],stone="st-swap"}
cells["p"] = cell{parent=cells[" "],stone="st-pull"}
cells["g"] = cell{parent=cells[" "],stone="st-grate1"}
--cells["I"] = cell{parent=cells[" "],stone="st-stoneimpulse"}
--cells["i"] = cell{parent=cells[" "],stone="st-stoneimpulse_movable"}
cells["_"] = cell{parent={cells[" "],randstone}}

cells["a"] = cell{parent=cells[" "],actor={"ac-blackball", {player=0}}}

rs_create_world(level,cells)
oxyd_shuffle()

if not difficult then
   enigma.ConserveLevel=TRUE
else
   enigma.ConserveLevel=FALSE
end
















