-- NAME DES LEVELS
-- Copyright (C) 2003 Martin Hawlisch
-- Licensed under GPL v2.0

levelw = 20
levelh = 13

create_world( levelw, levelh)
draw_border("st-greenbrown")
fill_floor("fl-sand", 0,0,levelw,levelh)

oxyd( 4,4)
oxyd( 4,6)
oxyd( 4,8)
oxyd( 6,4)
--oxyd( 6,6)
oxyd( 6,8)
oxyd( 8,4)
oxyd( 8,6)
oxyd( 8,8)
oxyd(11,4)
oxyd(11,6)
oxyd(11,8)
oxyd(13,4)
--oxyd(13,6)
oxyd(13,8)
oxyd(15,4)
oxyd(15,6)
oxyd(15,8)
oxyd_shuffle()

set_actor("ac-blackball", 10, 5)





























