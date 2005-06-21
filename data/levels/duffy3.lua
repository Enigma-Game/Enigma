-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Laser Spiral

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-greenbrown")
fill_floor("fl-hay")

oxyd(2,0)
oxyd(3,0)
oxyd(4,0)
oxyd(5,0)
oxyd(6,0)
oxyd(7,0)
oxyd(2,12)
oxyd(3,12)
oxyd(4,12)
oxyd(5,12)
oxyd(6,12)
oxyd(7,12)
oxyd(0,6)
oxyd(0,7)
oxyd_shuffle()

mirrorp(2,11,FALSE,FALSE, 4)
mirrorp(1,11,FALSE,FALSE, 2)
mirrorp(1,7,TRUE,FALSE, 4)

mirrorp(2,1,FALSE,FALSE, 4)
mirrorp(3,2,FALSE,FALSE, 4)
mirrorp(4,3,FALSE,FALSE, 4)
mirrorp(5,4,FALSE,FALSE, 4)
mirrorp(6,5,FALSE,FALSE, 4)
mirrorp(7,6,FALSE,FALSE, 4)

mirrorp(18,1,FALSE,FALSE, 2)
mirrorp(17,2,FALSE,FALSE, 2)
mirrorp(16,3,FALSE,FALSE, 2)
mirrorp(15,4,FALSE,FALSE, 2)
mirrorp(14,5,FALSE,FALSE, 2)

mirrorp(3,11,FALSE,FALSE, 2)
mirrorp(4,10,FALSE,FALSE, 2)
mirrorp(5,9,FALSE,FALSE, 2)
mirrorp(6,8,FALSE,FALSE, 2)
mirrorp(7,7,FALSE,FALSE, 2)

mirrorp(18,11,FALSE,FALSE, 4)
mirrorp(17,10,FALSE,FALSE, 4)
mirrorp(16,9,FALSE,FALSE, 4)
mirrorp(15,8,FALSE,FALSE, 4)
mirrorp(14,7,FALSE,FALSE, 4)

set_attrib(laser(19,6, TRUE, WEST), "name", "laser")

set_actor("ac-blackball", 1.5,6)
