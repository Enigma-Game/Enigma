--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)
draw_border("st-fakeoxyd")
fill_floor("fl-leavesb", 0,0,levelw,levelh)

oxyd( 1,0)
oxyd( 0,1)
oxyd( 18,0)
oxyd( 19,1)
oxyd( 0,11)
oxyd( 1,12)
oxyd( 18,12)
oxyd( 19,11)
oxyd( 5,6)
oxyd( 14,6)
oxyd_shuffle()

document(2,2," . . . There are more than two . . .")
set_actor("ac-blackball", 10, 6)















