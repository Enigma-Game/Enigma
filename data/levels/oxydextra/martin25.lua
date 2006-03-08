--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

stone="st-marble"

create_world( levelw, levelh)
draw_border( stone)

draw_checkerboard_floor("fl-leaves", "fl-bluegreen", 0,0,levelw,levelh)

oxyd( 1, 1)
oxyd(18, 1)
oxyd( 1,11)
oxyd(18,11)
oxyd_shuffle()

draw_stones( stone, {2,1},{0,1},3)
draw_stones( stone, {1,9},{1,0},2)
set_stone( stone, 2,11)
draw_stones( stone, {15,4},{1,-1},3)
draw_stones( stone, {12,10},{3,0},3)
set_stone( stone, 13,11)
set_stone( stone, 14,9)
set_stone( stone, 17, 10)

doorh( 1, 3, {name="door1"})
doorv(17, 1, {name="door2"})
doorh(18, 2, {name="door3"})
doorv( 2,10, {name="door4"})
doorv(15,11, {name="door5"})

shogundot1( 7, 3, {action="openclose", target="door5"})
shogundot1( 6, 5, {action="openclose", target="door4"})
shogundot1(13, 4, {action="openclose", target="door2"})
shogundot1(10,11, {action="openclose", target="door3"})
shogundot1(17,11, {action="openclose", target="door1"})

set_stone("st-shogun-s", 7, 9)
set_stone("st-shogun-s",16,10)


set_actor("ac-blackball", 10, 5)










