--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 39
levelh = 25

create_world( levelw, levelh)

fill_floor("fl-water", 0,0,levelw,levelh)
stone = "st-wood"
floor = "fl-sand"

oxyd(  6, 0)
oxyd( 25, 0)
oxyd( 34, 0)
oxyd( 38, 6)
oxyd( 34,12)
oxyd( 22,12)
oxyd( 13,12)
oxyd(  4,12)

oxyd_shuffle()

fill_floor( floor, 5, 3, 6, 5)
fill_floor( floor,12, 7, 3, 5)
fill_floor( floor, 3, 9, 3, 3)
fill_floor( floor,16, 5, 3, 3)
fill_floor( floor,20, 5, 3, 3)
fill_floor( floor,21, 9, 3, 3)
fill_floor( floor,24, 1, 3, 3)
fill_floor( floor,25, 7, 3, 3)
fill_floor( floor,28, 2, 3, 3)
fill_floor( floor,29, 6, 3, 3)
fill_floor( floor,33, 1, 3, 3)
fill_floor( floor,33, 9, 3, 3)
fill_floor( floor,35, 5, 3, 3)

set_stone( stone, 7, 4)
set_stone( stone, 8, 5)
set_stone( stone, 6, 5)
set_stone( stone, 7, 6)
set_stone( stone,12, 9)
set_stone( stone,13, 9)
set_stone( stone,14, 8)
set_stone( stone,14, 7)
set_stone( stone,17, 5)
set_stone( stone,18, 6)
set_stone( stone,16, 6)
set_stone( stone,17, 7)
set_stone( stone,21, 6)
set_stone( stone,22,10)
set_stone( stone,29, 3)
set_stone( stone,30, 7)
set_stone( stone,34, 2)
set_stone( stone,36, 6)



set_actor("ac-blackball", 9, 6.5)















