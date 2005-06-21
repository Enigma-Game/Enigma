-- Copyright (C) 2002 Daniel Heck 
-- Licensed under GPL v2.0 or above
levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-rock2")
fill_floor("fl-rough")


oxyd(2,2)
oxyd(level_width-3,level_height-3)

draw_stones("st-rock2", {5,1}, {0,1}, 11)
set_stone("st-door", 5, 10 , {name="door1", type="v"})

draw_stones("st-rock2", {14,1}, {0,1}, 11)
set_stone("st-door", 14, 2 , {name="door2", type="v"})


shogundot3(9,6, {target="door1", action="openclose"})
shogundot3(10,6, {target="door2", action="openclose"})
set_stone("st-shogun-sml", 10,7)



set_actor("ac-blackball", 10,9.5)

