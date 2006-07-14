-- Mirror Room, a level for Enigma
-- Copyright (C) 2005 Lukas Schueller
-- Licensed under GPL v2.0 or above
-------------------------------------------------
-- Environment
CreateWorld(20, 13) 
draw_border("st-stone2")
fill_floor("fl-abyss", 0,0, 20,13)
fill_floor("fl-plank", 2,2, 16,9)
set_attrib(laser(19,6, FALSE, WEST), "name", "laser")
set_stone("st-switch", 2,6, {action="onoff", target="laser"})
mirror3(3,4,FALSE,FALSE, 4)
mirror3(3,6,FALSE,FALSE, 4)
mirror3(3,8,FALSE,FALSE, 4)
mirror3(7,4,FALSE,FALSE, 4)
mirror3(7,6,FALSE,FALSE, 4)
mirror3(7,8,FALSE,FALSE, 4)
mirror3(12,4,FALSE,FALSE, 4)
mirror3(12,6,FALSE,FALSE, 4)
mirror3(12,8,FALSE,FALSE, 4)
mirror3(16,4,FALSE,FALSE, 4)
mirror3(16,6,FALSE,FALSE, 4)
mirror3(16,8,FALSE,FALSE, 4)
oxyd_default_flavor = "d"
oxyd(3,0)
oxyd(7,0)
oxyd(12,0)
oxyd(16,0)
oxyd(3,12)
oxyd(7,12)
oxyd(12,12)
oxyd(16,12)
oxyd(0,4)
oxyd(0,8)
oxyd(19,4)
oxyd(19,8)
oxyd_shuffle()
set_actor("ac-blackball",10,6.5, {player=0})




















