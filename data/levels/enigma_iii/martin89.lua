--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-rubberband")
fill_floor("fl-hay")

draw_stones("st-death", {5,5}, {4,0},4)
draw_stones("st-death", {2,7}, {4,0},4)

oxyd_default_flavor = "d"

oxyd(2,6)
oxyd(3,6)
oxyd(4,6)
oxyd(5,6)
oxyd(6,6)
oxyd(7,6)
oxyd(8,6)
oxyd(9,6)
oxyd(10,6)
oxyd(11,6)
oxyd(12,6)
oxyd(13,6)
oxyd(14,6)
oxyd(15,6)
oxyd(16,6)
oxyd(17,6)



oxyd_shuffle()

st = set_stone("st-rubberband", 10,12)
ac = set_actor("ac-blackball", 10,11)

AddRubberBand(ac, st, 16,1)















