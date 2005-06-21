--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)

draw_border("st-rock2")

oxyd (18,1)
oxyd (18,11)

fill_floor("fl-hay", 0,0, levelw,levelh)

draw_floor("fl-abyss", {11,4}, {0,1},3)

set_actor("ac-blackball", 3,5.5)

draw_stones("st-rock2", {5,3}, {1,0},10)
draw_stones("st-rock2", {5,7}, {1,0},7)
draw_stones("st-rock2", {13,7}, {1,0},4)
draw_stones("st-rock2", {16,1}, {0,1},3)
draw_stones("st-rock2", {16,8}, {0,1},3)
draw_stones("st-rock2", {11,8}, {0,1},3)


doorv(11,11, {name="door1"})
shogundot1(13,11, {action="openclose", target="door1"})

set_stone("st-shogun", 14,11)

puzzle(8,4,PUZ_0101)
puzzle(10,6,PUZ_0101)
puzzle(15,4,PUZ_0010)
puzzle(13,4,PUZ_0011)
puzzle(6,5,PUZ_0110)
puzzle(13,6,PUZ_1001)
puzzle(7,5,PUZ_1010)
puzzle(9,5,PUZ_1100)
puzzle(15,6,PUZ_1100)
puzzle(14,5,PUZ_1111)
puzzle(15,5,PUZ_1111)
puzzle(16,5,PUZ_0001)
puzzle(16,6,PUZ_0001)
