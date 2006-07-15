--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 25

create_world( levelw, levelh)
draw_border("st-greenbrown")
draw_stones("st-greenbrown_hole", {1,12},{1,0},18)
fill_floor("fl-inverse", 0,0,levelw,13)
fill_floor("fl-hay", 0,12,levelw,13)

oxyd( 3, 3)
oxyd( 3, 9)
oxyd( 3,15)
oxyd( 3,21)
oxyd(16, 3)
oxyd(16, 9)
oxyd(16,15)
oxyd(16,21)
oxyd_shuffle()

set_actor("ac-blackball", 11, 19)











