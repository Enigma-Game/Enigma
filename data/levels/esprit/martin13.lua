--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw =  20
levelh = 121

create_world( levelw, levelh)

fill_floor("fl-gray", 0,0,levelw,levelh)
stone = "st-rock2"

draw_border( stone)
oxyd(  2,72)
oxyd(  4,93)
oxyd( 14,40)
oxyd( 17,58)

oxyd_shuffle()

set_stone( stone, 5, levelh-3)
set_stone( stone,14, levelh-3)

oneway( 3, 1, enigma.WEST)
oneway( 6, 1, enigma.WEST)
oneway(13, 1, enigma.EAST)
oneway(16, 1, enigma.EAST)
oneway( 4, levelh-3, enigma.SOUTH)
oneway( 3, levelh-2, enigma.EAST)
oneway(15, levelh-3, enigma.SOUTH)
oneway(16, levelh-2, enigma.WEST)

draw_stones( stone, { 3,2}, {0,1}, levelh-4)
draw_stones( stone, { 6,2}, {0,1}, levelh-4)
draw_stones( stone, {13,2}, {0,1}, levelh-4)
draw_stones( stone, {16,2}, {0,1}, levelh-4)

set_stone( "st-death", 7, 10)
set_stone( "st-death",12, 23)
set_stone( "st-death", 7, 45)
set_stone( "st-death",12, 57)
set_stone( "st-death", 7, 71)
set_stone( "st-death",12, 84)
set_stone( "st-death", 7, 96)

set_item( "it-hollow", 9, 35)
set_item( "it-hollow",11, 65)
set_item( "it-hollow",10, 77)
set_item( "it-hollow", 8, 90)
set_item( "it-hill",  10,107)
set_item( "it-hill",   8,117)



enigma.AddConstantForce(0,25)
set_actor("ac-blackball", 9, levelh-2)










