-- Down Hill -- Oxyd Extra -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-16

dofile(enigma.FindDataFile("levels/lib/ant.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
actor = cell{actor={"ac-blackball", {player=0}}}
gradw = cell{parent={{gradient, SLOPE_W}}}
grade = cell{parent={{gradient, SLOPE_E}}}
normal= cell{floor="fl-normal"}
worm2 = cell{parent={{wormhole, 9.5, 8.5, {range=7}}}}
worm7 = cell{parent={{wormhole, 9.5, 3.5, {range=7.5}}}}
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

create_world(20, 13)
fill_world_func(abyss)

for _,y in {2,7} do
   draw_border_func({fakeoxyd, normal}, 0, y, 0, 3)
   draw_func(normal, {0,y+1}, {1,0}, 20)
   gradw( 8,y+1)
   grade(10,y+1)
   set_funcs(oxyd, {{3,y},{3,y+2},{16,y},{16,y+2}})
   dostring("set_funcs(worm"..y..", {{1,"..(y+1).."},{18,"..(y+1).."}})")
end

actor(9, 8)
oxyd_shuffle()










