--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)
draw_border("st-brick")
fill_floor( "fl-water", 0, 0, levelw, levelh)
fill_floor( "fl-brick", 1, 4, 9, 5)

set_stone( "st-wood", 6,4)
set_stone( "st-wood", 8,4)
set_stone( "st-wood", 6,5)
set_stone( "st-wood", 7,5)
set_stone( "st-wood", 9,5)
set_stone( "st-wood", 5,6)
set_stone( "st-wood", 7,6)
set_stone( "st-wood", 8,6)
set_stone( "st-wood", 6,7)
set_stone( "st-wood", 7,7)
set_stone( "st-wood", 9,7)
set_stone( "st-wood", 5,8)
set_stone( "st-wood", 8,8)

oxyd( 1,3)
oxyd( 1,9)
oxyd(18,5)
oxyd(18,7)
oxyd_shuffle()

set_actor("ac-blackball", 4.5, 5.5)










