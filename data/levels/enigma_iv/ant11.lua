-- Cannonball -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-01-08
-- 2003-01-10 -- modified base floor beneath doorE, doorF, removed document
-- 2003-01-11 -- modified so that it uses object groups from ant.lua
-- 2003-02-09 -- fixes in object groups
-- 2003-06-24 -- changes in AddConstantForce
-- 2003-10-18 -- too strong gravity fixed, also some minor cleanups

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
function construct(x, y) set_floor("fl-metal", x, y) end
function destruct(x, y)  set_floor("fl-abyss", x, y) end

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
force = 100
act_state = 0

function forceup()
   enigma.AddConstantForce(force,0)
end

function forcedn()
   enigma.AddConstantForce(0,0)
end

function run_trigger_action()
   if (act_state == 0) then
      act_state = 1
   end
end

function restart_trigger_action()
   act_state = 0
   forcedn()
end

function tick()
   if (act_state == 0) then
      send_group_message(doors_A, "open", nil)
   elseif (act_state == 1) then
      send_group_message(doors_A, "close", nil)
      act_state = act_state + 1
   elseif (act_state == 2) then
      send_group_message(doors_B, "open", nil)
      act_state = act_state + 1
   elseif (act_state == 3) then
      forceup()
      act_state = act_state + 1
   elseif (act_state == 4) then
      send_group_message(doors_B, "close", nil)
      act_state = 0
   else
      act_state = 0
   end
end

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
cells[";"]=cell{floor="fl-tigris"}
cells[":"]=cell{floor="fl-sahara"}
cells["."]=cell{floor="fl-abyss"}
cells["_"]=cell{floor="fl-metal"}
cells["w"]=cell{floor="fl-wood"}

cells["v"]=cell{{{gradient, 01}}}
cells["^"]=cell{{{gradient, 02}}}
cells[">"]=cell{{{gradient, 23}}}
cells["|"]=cell{{{gradient, 03}}}
cells["T"]=cell{{{gradient, 05}}}
cells["L"]=cell{{{gradient, 07}}}
cells["R"]=cell{{{gradient, 09}}}
cells["/"]=cell{{{gradient, 10}}}

cells[" "]=cell{{{checkerfloor,{cells[":"], cells[";"]}}}}

cells["="]=cell{stone="st-rock5"}
cells["#"]=cell{stone="st-rock6"}

cells["&"]=cell{{{oneway, EAST}}}
cells["%"]=cell{{{oneway, WEST}}}

cells["x"]=cell{stone="st-grate1"}
cells["e"]=cell{parent=cells["w"],stone={"st-door", {name="doorE", type="v"}}}
cells["f"]=cell{parent=cells["w"],stone={"st-door", {name="doorF", type="v"}}}

doors_A = {};
doors_B = {};
cells["?"]=cell{{{add_multistone, "st-door", doors_A, {type="h"}}}}
cells["!"]=cell{{{add_multistone, "st-door_b", doors_B, {type="v"}}}}

cells["D"]=cell{parent=cells["w"],stone="st-death"}
cells["W"]=cell{parent=cells["w"],stone="st-brick"}
cells["E"]=cell{parent=cells["w"],stone={"st-floppy", {action="openclose", target="doorE"}}}
cells["F"]=cell{parent=cells["w"],stone={"st-floppy", {action="openclose", target="doorF"}}}

path0 = {}
loco0 = {}
cells["$"]=cell{parent={cells["."], {add_multicell, path0, 0}}}
cells["@"]=cell{parent={cells["$"], {add_multicell, path0, 1}, cells["_"]}}
cells["c"]=cell{parent={cells["@"], {add_multicell, loco0, construct}}}
cells["d"]=cell{parent={cells["$"], {add_multicell, loco0, destruct}}}
rail0 = new_rail(loco0, path0)

cells["~"]=cell{stone={"st-timer", {action="callback", target="tick", interval=1}}}
cells["`"]=cell{stone={"st-timer", {action="callback", target="rail0", interval=0.15}}}

cells["+"]=cell{item={"it-trigger", {action="callback", target="forceup"}}}
cells["-"]=cell{parent=cells["_"],item={"it-trigger", {action="callback", target="forcedn"}}}
cells["*"]=cell{parent={cells[">"], cells["+"]}}
cells["o"]=cell{item={"it-trigger", {action="callback", target="run_trigger_action"}}}
cells["9"]=cell{item={"it-trigger", {action="callback", target="restart_trigger_action", invisible=1}}}

cells["h"]=cell{parent=cells["w"],item="it-floppy"}
cells["s"]=cell{parent=cells["x"],item="it-spring1"}

cells["X"]=cell{{cells["x"], cells["$"]}}
cells["Y"]=cell{{cells["x"], cells["@"]}}
cells["y"]=cell{{cells["x"], cells["w"]}}

cells["0"]=oxyd

cells["O"]=cell{parent=cells["9"],actor={"ac-blackball", {player=0}}}

level = {
   "====================.....................................~WWWWWWWWWWWWWWWWWWW",
   "=xxxxxxxxxxxxxxxxxx==wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwWW......$$c@Y@@d$...W",
   "=xx        O       %ww=..............www..............Www%wwwwwwXwwwWwwwXwwwW",
   "=x 9               ====...............w...............WWWW...wwwXXXXXXXXXwwwW",
   "=x#?###############=..................w............WWWWWWW...wwwwwwwWwwwwwWWW",
   "=x# !TvvvvvvvvvvvR&__................www...........wwwwww&wwwwwwwwwwWEwwwwe0W",
   "=x#o!|>>*>>>*>>>*>&____________-.....whw.........wwwwwwwDWWWWWWWWWWWWWWWWWWWW",
   "=x# !L^^^^^^^^^^^/&__................www...........wwwwww&wwwwwwwwwwWFwwwwf0W",
   "=x#?###############=..................w............WWWWWWW...wwwwwwwWwwwwwWWW",
   "=x 9               ====...............w...............WWWW...wwwXXXXXXXXXwwwW",
   "=xx                %ww=..............www..............Www%wwwwwwXwwwWwwwXwwwW",
   "=sxxxxxxxxxxxxxxxxx==wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwWW......$$c@Y@@d$...W",
   "====================.....................................`WWWWWWWWWWWWWWWWWWW"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
randomseed(enigma.GetTicks())
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()















