--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)
draw_border("st-rock6")
draw_checkerboard_floor("fl-sahara", "fl-tigris", 0, 0, levelw, levelh)

set_item("it-hammer", 5,2)

draw_border("st-break_acwhite", 15,4,3,3)
oxyd(16,5)

draw_border("st-break_acwhite", 2,8,3,3)
oxyd( 3,9)

draw_border("st-break_acwhite", 1,2,3,3)
oxyd( 2,3)

draw_border("st-break_acwhite", 6,5,3,3)
oxyd( 7,6)

draw_border("st-break_acwhite", 9,3,3,3)
oxyd(10,4)

draw_border("st-break_acwhite", 13,9,3,3)
oxyd(14,10)

draw_border("st-break_acwhite", 8,8,3,3)
oxyd(9,9)

draw_border("st-break_acwhite", 16,1,3,3)
oxyd(17,2)

oxyd_shuffle()

set_actor("ac-blackball", 1.5, 1.5)
set_actor("ac-whiteball", 1.5, 10.5, {mouseforce=0})















