-- Meditation -- Oxyd Magnum -- the Enigma Level
-- (c) 2002 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-21

Require(enigma.FindDataFile("levels/lib/ant.lua")
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
normal = cell{floor="fl-brick"}
stone  = cell{stone="st-marble"}
marble = cell{actor={"ac-whiteball-small", {player=0, mouseforce=1}}}
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
create_world(20, 13)
draw_border_func(abyss)
fill_world_func(normal, 0,1,0,-2)
draw_border_func(stone, 0,1,0,-2)
draw_func_corners(hollow, 2,3,-4,-6)
draw_func_corners(stone,  4,3,-8,-6)
draw_func_corners(stone,  7,5,-14,-10)
draw_func_corners(marble, 8,5,-16,-10)
draw_func(stone, {{7,4},{7,-4}}, {1,0}, 6)
draw_func(stone, {{5,5},{-5,5}}, {0,1}, 3)
stone({{2,6},{-2,6}})











