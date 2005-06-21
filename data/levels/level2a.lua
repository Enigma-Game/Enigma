--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-invisible")

fill_floor("fl-space")

fill_floor("fl-gray", 9,6, 2,1)
fill_floor("fl-gray", 1,3, 18,1)
fill_floor("fl-gray", 1,9, 18,1)

abyss(3,3)
abyss(5,3)
abyss(3,9)
abyss(5,9)
abyss(14,3)
abyss(16,3)
abyss(14,9)
abyss(16,9)
abyss(8,6)
abyss(11,6)

shogundot1 (18,4)
shogundot2 (9,1)
shogundot3 (6,11)
oxyd(2,6)
oxyd(15,6)
oxyd(17,6)
oxyd(6,6)
oxyd(13,6)
oxyd(4,6)

fakeoxyd(0,3)
fakeoxyd(19,3)
fakeoxyd(0,9)
fakeoxyd(19,9)

--document(10,6, "Vorsicht, schwarze Loecher")
document(10,6, "Beware! Black holes!")

set_actor("ac-blackball" ,9.5, 6.5)

