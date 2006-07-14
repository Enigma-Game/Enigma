--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelh = 10 
create_world( 20, levelh*13-levelh+1)

fill_floor("fl-ice_001")

if not difficult then
    wall_stone = "st-actorimpulse_invisible"
else
    wall_stone = "st-death_invisible"
end

draw_border(wall_stone)

oxyd(10,3)
oxyd(11,levelh*12)

for y = 1, levelh-1 do
    x = random(1,level_width-7)
    draw_stones(wall_stone, {0,y*12}, {1,0}, x)
    draw_stones(wall_stone, {level_width-1,y*12}, {-1,0}, level_width-(x+6))
    set_stone("st-actorimpulse_invisible", x, y*12)
    set_stone("st-actorimpulse_invisible", x+5, y*12)
    set_item("it-flagwhite", x, y*12)
    set_item("it-flagblack", x+5, y*12)
end


set_actor("ac-blackball",10.5, 1.5)

enigma.AddConstantForce(0,5)
display.SetFollowMode(display.FOLLOW_SCREENSCROLLING)
















