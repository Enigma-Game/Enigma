-- A level for Enigma
-- Name:        Im-pull-se
-- Filename:    ralf07.lua
-- Copyright: 	(C) May 2003 Ralf Westram
-- Contact: 	amgine@reallysoft.de
-- License: 	GPL v2.0 or above

dofile(enigma.FindDataFile("levels/lib/ralf.lua"))

--debug_mode()

level={
   "!!!!!!!!!o!!!!!!!!!!",
   "!!!!!!!!   !!!!!!!!!",
   "!!!!!!xxxXxxx!!!!!!!",
   "!!!!!xX     Xx!!!!!!",
   "!!!!!x R_I_R x!!!!!!",
   "!!!! x _#i#_ x !!!!!",
   "!!!o X Ii iI X o!!!!",
   "!!!! x _#i#_ x !!!!!",
   "!!!!!x R_I_R x!!!!!!",
   "!!!!!xX  a  Xx!!!!!!",
   "!!!!!!xxxXxxx!!!!!!!",
   "!!!!!!!!   !!!!!!!!!",
   "!!!!!!!!!o!!!!!!!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

triggerstate="000000000000"
triggers=0

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

function open(x,y)
   x,y = x+xlo,y+ylo
   enigma.KillStone(x,y)
   set_stone("st-pull",x,y)
end

function check(which)
   local state = strsub(triggerstate,which,which)
   state = 1-state
   triggerstate = strsub(triggerstate,1,which-1)..state..strsub(triggerstate,which+1)

   if (triggerstate=="111111111111") then
      open(9,2)
      open(9,10)
      open(5,6)
      open(13,6)
   end
end

function set_trigger(x,y)
   triggers=triggers+1
   local funcn = "trig_"..triggers
   dostring(funcn.." = function() check("..triggers..") end")
   set_item("it-trigger",x,y,{action="callback",target=funcn})
end

function my_floor(x,y)
   if mod(x+y,2)==1 then
      set_floor("fl-bluegray",x,y)
   else
      set_floor("fl-bluegreen",x,y)
   end
end

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells["!"] = cell{floor="fl-abyss"}
cells["."] = cell{floor="fl-sahara"}
cells[" "] = cell{parent=my_floor}

cells["#"] = cell{parent=cells["."],stone="st-glass"}
cells["x"] = cell{parent=cells[" "],stone="st-glass1"}
cells["X"] = cell{parent=cells["."],stone="st-glass1"}

cells["a"] = cell{parent=cells[" "],actor={"ac-blackball", {player=0}}}
cells["o"] = cell{oxyd}

cells["_"] = cell{parent={cells[" "],set_trigger}}
cells["R"] = cell{parent=cells["_"],stone="st-pull"}

cells["i"] = cell{parent=cells[" "],stone="st-stoneimpulse"}
cells["I"] = cell{parent=cells["."],stone="st-stoneimpulse"}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

rs_create_world(level,cells)
oxyd_shuffle()















