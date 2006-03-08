--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

stone = "st-rock2"

create_world( levelw, levelh)
fill_floor("fl-abyss", 1, 11, 18,1)
fill_floor("fl-plank", 0, 0, levelw, levelh-2)

draw_stones(stone, { 2,2}, {0,1}, 6)
draw_stones(stone, { 3,7}, {1,0}, 4)
draw_stones(stone, { 6,2}, {0,1}, 5)
draw_stones(stone, { 4,1}, {0,1}, 5)
draw_stones(stone, { 1,9}, {1,0}, 8)
draw_stones(stone, { 8,4}, {0,1}, 5)
draw_stones(stone, { 8,2}, {1,0}, 2)
draw_stones(stone, {11,2}, {0,1}, 3)
draw_stones(stone, {13,2}, {0,1}, 3)
draw_stones(stone, {11,6}, {1,0}, 3)
draw_stones(stone, {11,7}, {1,0}, 3)
draw_stones(stone, {12,8}, {0,1}, 2)
draw_stones(stone, {15,2}, {0,1}, 2)
draw_stones(stone, {15,5}, {0,1}, 3)
draw_stones(stone, {17,2}, {0,1}, 5)
draw_stones(stone, {13,10},{1,0}, 2)
set_stone( stone, 12, 4)
set_stone( stone, 16, 7)
set_stone( stone, 18, 9)

draw_border( stone)

set_stone( "st-shogun-s", 3, 5)
set_stone( "st-shogun-s", 8, 3)
set_stone( "st-shogun-s",10, 7)
set_stone( "st-shogun-s",10, 9)
set_stone( "st-shogun-s",15, 8)
set_stone( "st-shogun-s",16, 2)

shogundot1( 3, 6, {action="openclose", target="bridge1"})
shogundot1( 7, 8, {action="openclose", target="bridge2"})
shogundot1(12, 5, {action="openclose", target="bridge3"})
shogundot1(13, 9, {action="openclose", target="bridge4"})
shogundot1(16, 6, {action="openclose", target="bridge5"})
shogundot1(17, 9, {action="openclose", target="bridge6"})

set_floor("fl-bridge", 1,11, {name="bridge1"})
set_floor("fl-bridge", 4,11, {name="bridge2"})
set_floor("fl-bridge", 7,11, {name="bridge3"})
set_floor("fl-bridge",12,11, {name="bridge4"})
set_floor("fl-bridge",15,11, {name="bridge5"})
set_floor("fl-bridge",18,11, {name="bridge6"})

oxyd( 1,12)
oxyd( 4,12)
oxyd( 7,12)
oxyd(12,12)
oxyd(15,12)
oxyd(18,12)

oxyd_shuffle()

set_actor("ac-blackball", 10, 5)










