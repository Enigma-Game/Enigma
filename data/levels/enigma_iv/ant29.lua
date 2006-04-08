-- Robbery -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-11

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

function f_doorA() send_group_message(doorsA, "openclose") end

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

floor = cell{floor="fl-himalaya"}
stone = cell{stone="st-rock4"}
actor = cell{actor={"ac-blackball", {player=0}}}
doorsA = {}
doorA = cell{{{add_multistone, "st-door_a", doorsA, {type="v"}}}}
cslt1 = cell{stone={"st-coinslot", {action="callback", target="f_doorA"}}}
coin1 = cell{item= {"it-coin1"}}
coin2 = cell{item= {"it-coin2"}}
coin3 = cell{item= {"it-coin4"}}
coinr = cell{{{randomfloor, {coin1, coin2, coin3}}}}
crack3 = cell{item= {"it-crack3", {fixed=1}}}
crack2 = cell{item= {"it-crack1", {fixed=1}}}
spring = cell{item= "it-spring2"}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

create_world(20, 13)
oxyd_default_flavor = "a"
fill_world_func(floor)
draw_border_func(stone)
draw_func(abyss, {13,0}, {0,1}, 13)
for i=0,4 do draw_func(coinr, {14+i,6-i}, {0,1}, 2*i+1) end
draw_func(stone, {12,1}, {0,1}, 3)
draw_func(stone, {12,11}, {0,-1}, 3)
draw_func(stone, {14,1}, {0,1}, 3)
draw_func(stone, {14,11}, {0,-1}, 3)

set_funcs(oxyd, {{1,1},{1,11},{11,1},{11,11}})
set_funcs(stone, {{1,2},{2,2},{1,10},{2,10},{11,2},{10,2},{11,10},{10,10}})
set_funcs(doorA, {{2,1},{2,11},{10,1},{10,11}})
cslt1(6,6)
coin1(8,7)

crack3(11,9)
crack2(10,9)
crack2(11,8)
spring(10,11)

actor(5,5)

oxyd_shuffle()
enigma.ConserveLevel = FALSE















