-- Circularity -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-01-07
-- 2003-02-20 -- major rewrite

Require("levels/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
function construct0(x, y) set_floor("fl-wood", x, y)  end
function construct1(x, y) set_floor("fl-samba", x, y) end
function destruct(x, y)   set_floor("fl-abyss", x, y) end

function tick()
   rail0()
   rail1()
end
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
cells[";"]=cell{floor="fl-tigris"}
cells[","]=cell{floor="fl-sahara"}
cells["."]=cell{floor="fl-abyss"}

cells["train0"]=cell{floor="fl-wood"}
cells["train1"]=cell{floor="fl-samba"}

cells[" "]=cell{{{checkerfloor,{cells[","], cells[";"]}}}}
cells["="]=cell{parent=cells[" "],stone="st-rock5"}
cells["O"]=cell{parent=cells[" "],actor={"ac-blackball", {player=0}}}

path0 = {}
loco0 = {}
cells["!"]=cell{parent={cells["."], {add_multicell, path0, 0}}}
cells["_"]=cell{parent={cells["!"], {add_multicell, path0, 1}, cells["train0"]}}
cells["c"]=cell{parent={cells["_"], {add_multicell, loco0, construct0}}}
cells["d"]=cell{parent={cells["!"], {add_multicell, loco0, destruct}}}
cells["C"]=cell{parent={cells["_"], cells["="]}}
cells["D"]=cell{parent={cells["!"], cells["="]}}
rail0 = new_rail(loco0, path0)

path1 = {}
loco1 = {}
cells["?"]=cell{parent={cells["."], {add_multicell, path1, 0}}}
cells["-"]=cell{parent={cells["?"], {add_multicell, path1, 1}, cells["train1"]}}
cells["a"]=cell{parent={cells["-"], {add_multicell, loco1, construct1}}}
cells["b"]=cell{parent={cells["?"], {add_multicell, loco1, destruct}}}
cells["A"]=cell{parent=cells["-"], stone="st-grate1"}
cells["B"]=cell{parent=cells["?"], stone="st-grate1"}
rail1 = new_rail(loco1, path1)

cells["~"]=cell{parent=cells[" "], stone={"st-timer", {action="callback", target="tick", interval=0.20}}}
cells["0"]=oxyd

level = {
   "====================",
   "=   !c___CC___d!   =",
   "= 0 !b---AA---a! 0 =",
   "=   !?___CC___?!   =",
   "=   !?c------d?!   =",
   "=   !?!-__d!-!?!   =",
   "=...!?!b_ O!a!?!...=",
   "=   !?!?_c!!?!?!   =",
   "=   !?!??????!?!   =",
   "=   !?!!!DD!!!?!   =",
   "= 0 !????BB????! 0 =",
   "=   !!!!!DD!!!!!   =",
   "~==================="
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor = "b"
create_world_by_map(level)
oxyd_shuffle()
