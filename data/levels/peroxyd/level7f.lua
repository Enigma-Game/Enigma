--
-- A level for Enigma
--
-- Copyright (c) 2002 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-rock5")
fill_floor("fl-samba")

oxyd_default_flavor = "b"

oxyd ( 4, 2)
oxyd ( 7, 2)
oxyd (12, 2)
oxyd (15, 2)
oxyd ( 4,10)
oxyd ( 7,10)
oxyd (12,10)
oxyd (15,10)

oxyd_shuffle()

draw_stones("st-rock5", { 9,1}, {0,1},5)
draw_stones("st-rock5", {10,1}, {0,1},5)
draw_stones("st-rock5", { 9,7}, {0,1},5)
draw_stones("st-rock5", {10,7}, {0,1},5)

puzzle(2,2,PUZ_0010)
puzzle(5,2,PUZ_0010)
puzzle(13,2,PUZ_0010)
puzzle(16,2,PUZ_0010)
puzzle(2,3,PUZ_1010)
puzzle(5,3,PUZ_1010)
puzzle(13,3,PUZ_1010)
puzzle(16,3,PUZ_1010)
puzzle(2,4,PUZ_1010)
puzzle(5,4,PUZ_1010)
puzzle(13,4,PUZ_1010)
puzzle(16,4,PUZ_1010)
puzzle(2,5,PUZ_1010)
puzzle(5,5,PUZ_1010)
puzzle(13,5,PUZ_1010)
puzzle(16,5,PUZ_1010)
puzzle(1,6,PUZ_0100)
puzzle(2,6,PUZ_1111)
puzzle(3,6,PUZ_0101)
puzzle(4,6,PUZ_0101)
puzzle(5,6,PUZ_1111)
puzzle(6,6,PUZ_0101)
puzzle(7,6,PUZ_0101)
puzzle(8,6,PUZ_0101)
puzzle(9,6,PUZ_0101)
puzzle(10,6,PUZ_0101)
puzzle(11,6,PUZ_0101)
puzzle(12,6,PUZ_0101)
puzzle(13,6,PUZ_1111)
puzzle(14,6,PUZ_0101)
puzzle(15,6,PUZ_0101)
puzzle(16,6,PUZ_1111)
puzzle(17,6,PUZ_0001)
puzzle(2,7,PUZ_1010)
puzzle(5,7,PUZ_1010)
puzzle(13,7,PUZ_1010)
puzzle(16,7,PUZ_1010)
puzzle(2,8,PUZ_1010)
puzzle(5,8,PUZ_1010)
puzzle(13,8,PUZ_1010)
puzzle(16,8,PUZ_1010)
puzzle(2,9,PUZ_1010)
puzzle(5,9,PUZ_1010)
puzzle(13,9,PUZ_1010)
puzzle(16,9,PUZ_1010)
puzzle(2,10,PUZ_1000)
puzzle(5,10,PUZ_1000)
puzzle(13,10,PUZ_1000)
puzzle(16,10,PUZ_1000)

set_item("it-yinyang",  3,4)
set_item("it-yinyang", 14,8)

set_actor("ac-blackball",  3.5, 4.5)
set_actor("ac-whiteball", 14.5, 8.5)












