-- Snow White -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-17
-- update 2003-09-18 Sven Siggelkow: inserted "controllers=1" because the white marble didn't move
dofile(enigma.FindDataFile("levels/lib/ant.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
actors = {}
bctors = {}
actor  = cell{{{add_multiactor, "ac-whiteball-small", actors, {player=0, mouseforce=0, essential=1}}}}
bctor  = cell{{{add_multiactor, "ac-whiteball", bctors, {player=0, controllers=1, essential=1}}}}
normal = cell{floor="fl-rough"}
invert = cell{floor="fl-inverse"}
stone  = cell{stone="st-glass"}
tiles  = cell{{{randomfloor, {normal, invert}}}}
mask   = cell{stone="st-chameleon"}
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
randomseed(666)

create_world(20, 13)
fill_world_func(tiles)
draw_border_func({normal, stone})
draw_border_func(abyss, 1, 1, -2, -2)

local x0, y0 = 4,4
bctor(x0,y0)
document(x0,y0,"Look... they're over there!")
ngon_funcs(actor, {x0,y0}, 2.25, 3, 60)
ngon_funcs({hollow, mask}, {15.5,7.5}, 2.5, 3, 0, floor)
rubber_band_circle(actors, -10, 0)
rubber_band_circle(actors, 40, 3.25)
add_rubber_bands(actors, bctors, 10, 0)













