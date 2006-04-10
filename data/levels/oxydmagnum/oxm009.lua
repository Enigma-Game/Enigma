-- What's That?! -- Oxyd Magnum -- the Enigma Level
-- (c) 2002 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-21

Require("levels/lib/ant.lua")
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
normal = cell{floor="fl-normal"}
invers = cell{floor="fl-inverse"}
checker= cell{{{checkerfloor, {normal, invers}}}}
death  = cell{stone="st-death"}
spring1= cell{item="it-spring1"}
spring2= cell{item="it-spring2"}
glass  = cell{stone="st-glass"}

marble = cell{actor={"ac-blackball", {player=0}}}
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
create_world(58, 13)
oxyd_default_flavor="d"

draw_border_func(abyss)
fill_world_func(normal,   0, 1, 20, 11)
draw_border_func(glass,   0, 1, 20, 11)
draw_border_func(glass,   2, 5, 3, 3)
draw_border_func(glass,   6, 8, 3, 3)
draw_border_func(glass,  11, 6, 3, 3)
draw_border_func(glass,  15, 3, 3, 3)
draw_border_func(glass,   7, 1, 3, 5)
draw_border_func(glass,  11, 1, 3, 4)
draw_border_func(glass,  15, 7, 3, 5)
death({{7,5},{11,4}})
oxyd({{3,3},{3,-3}})
spring2(9,7)
spring1(14,3)
marble(10,6)

fill_world_func(invers,  20, 1, 19, 11)
draw_border_func(glass,  19, 1, 20, 11)
draw_border_func(glass,  23, 3, 3, 3)
draw_border_func(glass,  27, 7, 3, 3)
draw_border_func(glass,  31, 6, 3, 3)
draw_border_func(glass,  36, 9, 3, 3)
draw_border_func(glass,  29, 1, 3, 5)
draw_border_func(glass,  34, 1, 3, 4)
death({{29,7},{31,5},{34,4}})

fill_world_func(checker, 39, 1, 19, 11)
draw_border_func(glass,  38, 1, 20, 11)
draw_border_func(glass,  41, 1, 3, 3)
draw_border_func(glass,  41, 5, 3, 3)
draw_border_func(glass,  42, 9, 3, 3)
draw_border_func(glass,  47, 9, 3, 3)
draw_border_func(glass,  50, 5, 3, 3)
draw_border_func(glass,  51, 1, 3, 3)
draw_border_func(glass,  51, 9, 3, 3)
draw_border_func(glass,  46, 1, 3, 5)
draw_border_func(glass,  54, 5, 4, 3)
death({{44,9},{47,5},{52,5}})
oxyd({{-2,3},{-2,-3}})

kill_stone(19,9)
kill_stone(38,2)

oxyd_shuffle()










