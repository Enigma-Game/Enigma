--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)
draw_border("st-death_invisible")
fill_floor("fl-metal")


draw_stones("st-actorimpulse_invisible", {3,1}, {0,3},4)
draw_stones("st-actorimpulse_invisible", {6,2}, {0,3},4)
draw_stones("st-death_invisible", {9,3}, {0,3},3)
draw_stones("st-death_invisible", {10,3}, {0,3},3)
draw_stones("st-actorimpulse_invisible", {13,2}, {0,3},4)
draw_stones("st-actorimpulse_invisible", {16,1}, {0,3},4)

document( 4, 5, "The danger is not visible!!!")
set_item("it-extralife", 11, 3)
set_item("it-extralife", 18, 11)

oxyd( 0, 2)
oxyd( 0, 6)
oxyd( 0,10)
oxyd(19, 2)
oxyd(19, 6)
oxyd(19,10)

oxyd_shuffle()

set_actor("ac-blackball", 2.3, 2.3)















