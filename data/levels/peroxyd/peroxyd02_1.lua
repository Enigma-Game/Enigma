--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13
stone="st-greenbrown"

create_world( levelw, levelh)
draw_border( stone)
fill_floor("fl-gray", 0,0,levelw,levelh)
set_stone("st-wood", 8, 6)
set_floor("fl-abyss", 9,5)

--document( 6,6, "Manche Steine und Steinreihen lassen sich verschieben!")
document(6,6, "Some stones and stone rows can be moved!")
oxyd( 2, 4)
oxyd( 4, 4)
oxyd( 3, 6)
oxyd( 2, 8)
oxyd( 4, 8)
oxyd( 18,2)
oxyd_shuffle()
set_stone( stone, 12, 1)
set_stone( stone, 12, 2)
set_stone( stone, 11, 6)
draw_stones( stone, {8,4},{3,0},3)
draw_stones( stone, {9,1},{0,1},4)
draw_stones( stone, {9,6},{0,1},6)
draw_stones( stone, {13,6},{1,0},3)
draw_stones( stone, {13,8},{0,1},4)
draw_stones( stone, {16,1},{0,1},6)
puzzle( 7, 4,PUZ_0010)
puzzle( 7, 5,PUZ_1010)
puzzle( 7, 6,PUZ_1010)
puzzle( 7, 7,PUZ_1000)

set_stone(stone, 11,3)
set_stone(stone, 14,3)
puzzle( 11, 2,PUZ_0100)
puzzle( 12, 2,PUZ_0001)
set_floor("fl-abyss", 13,2)
set_floor("fl-abyss", 14,2)

puzzle( 10, 5,PUZ_0100)
puzzle( 11, 5,PUZ_0101)
puzzle( 12, 5,PUZ_0101)
puzzle( 13, 5,PUZ_0001)

puzzle( 12, 6,PUZ_0010)
puzzle( 12, 7,PUZ_1010)
puzzle( 12, 8,PUZ_1010)
puzzle( 12, 9,PUZ_1010)
puzzle( 12,10,PUZ_1000)

set_actor("ac-blackball", 6, 6)











