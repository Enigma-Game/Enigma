-- Copyright (C) 2002 Daniel Heck 
-- Licensed under GPL v2.0 or above
levelw = 58
levelh = 13

create_world(levelw, levelh)
draw_border("st-greenbrown")
fill_floor("fl-leaves")

draw_floor("fl-abyss", {3,0}, {0,1}, level_height)
draw_floor("fl-abyss", {54,0}, {0,1}, level_height)
abyss(3,3)

oxyd(1,1)
oxyd(level_width-2,level_height-2)
oxyd(level_width-2, 1)
oxyd(1,level_height-2)
--oxyd_shuffle()

mirror3(29,6,TRUE,FALSE, 4)
mirrorp(26,6,TRUE,FALSE, 4)
mirrorp(32,6,TRUE,FALSE, 2)

set_attrib(laser(29,0, FALSE, SOUTH), "name", "laser")
set_stone("st-switch", 29,12, {action="onoff", target="laser"})

set_actor("ac-blackball", 30,10)
















