-- Dance With Me -- Oxyd Extra -- the Enigma Level
-- (c) 2002 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-21
--
--- NOTE
--- there has to be a killer actor in this level - small marble that kills the big one,
--- if the two touch. As soon as it's, replace ac-whiteball-small with correct model name,
--- or supply coorect attributes.
--
--- ALSO NOTE
--- in Oxyd Extra, the killer marble was a lot faster than black one. I tried to use
--- mouseforce=2 with no effect, please repair as soon as this feature becomes
--- implemented.
---
--- FINAL NOTE
--- everything should work now :-) Daniel
---
--- POST SCRIPTUM
--- yes, it does :) Thanks, Petr


Require("levels/lib/ant.lua")
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
fl0 = cell{floor="fl-gray"}
marble = cell{actor={"ac-blackball", {player=0}}}
killer = cell{actor={"ac-killerball", {mouseforce=2, controllers=1}}}
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
create_world(20, 13)
oxyd_default_flavor="c"
fill_world_func(abyss)
fill_world_func(fl0, 7, 4, 5, 5)
draw_func(oxyd, {{8,3},{8,-3}}, {1,0}, 3)
draw_func(oxyd, {{6,5},{-7,5}}, {0,1}, 3)
draw_func_corners(oxyd, 7, 4, 5, 5)

marble(9, -4)
killer(9, 4)

oxyd_shuffle()










