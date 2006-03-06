-- The Laser Magic -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-12

Require("levels/lib/ant.lua")
enigma.ConserveLevel=FALSE                  -- \dh\

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

floor = cell{floor={face="fl-floor_001"}}
stone = cell{stone={face="st-marble"}}
block = cell{stone={face="st-block"}}
stbrk = cell{stone={face="st-stone_break"}}
door  = cell{stone={face="st-door", attr={name="doorA", type="v"}}}

cslt1 = cell{stone={face="st-coinslot", attr={action="openclose", target="doorA"}}}
cslt2 = cell{stone={face="st-coinslot", attr={action="onoff", target="laserA"}}}
coin1 = cell{item= {face="it-coin1"}}

actor = cell{actor={face="ac-blackball", attr={player=0, mouseforce=1}}}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
create_world(20, 13)
oxyd_default_flavor = "b"
fill_world_func(floor)
draw_border_func(stone)

draw_func(stone, {3,1}, {0,1}, 11)
draw_func(stone, {16,1}, {0,1}, 11)

set_funcs(fakeoxyd, {{1,1},{1,11}})
set_funcs(oxyd, {{18,1},{18,11}})

door(3, 6)
cslt1(5, 6)
Document(2, 3, "Give it another try...")

set_attrib(laser(16, 11, FALSE, WEST), "name", "laserA")
stbrk(16, 6)
cslt2(14, 6)

actor(7,7)
coin1(9,7)
coin1(10,7)

block(11, 3)















