levelw = 20
levelh = 13
create_world(levelw, levelh)
enigma.ConserveLevel = FALSE

draw_border("st-stone1")
fill_floor("fl-leaves", 0,0, level_width,level_height)

fill_floor ("fl-water", 8, 1, 4, 11)
fill_floor ("fl-stwood", 8, 1, 4, 1)
fill_floor ("fl-stwood", 8, 11, 4, 1)

-- st = set_stone ("st-grate1", 13, 10)
-- n=25
-- for i=1,n do
--     angle = (270 + 360/n*(i-1))-- / (180 / 3.141)
--     x = 3*cos (angle) 
--     y = 3*sin (angle)
--     ac = set_actor ("ac-whiteball-small", 13.5 + x, 10.5+y, {controllers=0})
--     AddRubberBand (ac, st, 10000, 3.00, 3.0)
-- end

--fill_stones ("st-white4", 1, 4, 2, 6)
set_stone ("st-white4", 2,5)
set_stone ("st-white4", 1,4)
set_stone ("st-white4", 1,6)
set_stone ("st-white4", 2,7)
set_stone ("st-white4", 1,8)

set_stone ("st-white4", 17,5)
set_stone ("st-white4", 18,4)
set_stone ("st-white4", 18,6)
set_stone ("st-white4", 17,7)
set_stone ("st-white4", 18,8)


oxyd_default_flavor = "d"
oxyd (1, 1)
oxyd (18, 1)
oxyd (1, 3)
oxyd (18, 3)
oxyd (1, 5)
oxyd (18, 5)
oxyd (1, 7)
oxyd (18, 7)
oxyd (1, 9)
oxyd (18, 9)
oxyd (1, 11)
oxyd (18, 11)


oxyd_shuffle()

ac1 = set_actor("ac-blackball", 5, 6.5)
ac2 = set_actor("ac-whiteball", 15, 6.5, {player=0} )
AddRubberBand (ac1, ac2, 10000, 10, 10)

