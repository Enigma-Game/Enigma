--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 13)
oxyd_default_flavor = "d"

fill_floor( "fl-rough")

draw_border( "st-wood")

draw_stones( "st-wood", {1, 1}, {1,0}, 18)
draw_stones( "st-wood", {1,11}, {1,0}, 18)

draw_stones( "st-wood", { 1,2}, {0,2}, 5)
draw_stones( "st-wood", { 2,3}, {0,2}, 4)
draw_stones( "st-wood", { 3,2}, {0,2}, 5)
draw_stones( "st-wood", { 4,3}, {0,2}, 4)
set_stone( "st-wood", 4, 2)
draw_stones( "st-wood", { 5,4}, {0,2}, 3)
set_stone( "st-wood", 5, 3)
set_stone( "st-wood", 5, 9)
draw_stones( "st-wood", { 6,5}, {0,2}, 3)
set_stone( "st-wood", 6, 2)
set_stone( "st-wood", 6, 6)
draw_stones( "st-wood", { 7,4}, {0,2}, 4)
set_stone( "st-wood", 7, 3)
draw_stones( "st-wood", { 8,5}, {0,2}, 3)
set_stone( "st-wood", 8, 2)
set_stone( "st-wood", 8, 4)
set_stone( "st-wood", 8,10)
draw_stones( "st-wood", { 9,4}, {0,1}, 2)
draw_stones( "st-wood", { 9,7}, {0,1}, 2)
set_stone( "st-wood", 9,10)
draw_stones( "st-wood", {10,2}, {0,2}, 4)
set_stone( "st-wood",10, 9)
draw_stones( "st-wood", {11,3}, {0,2}, 4)
set_stone( "st-wood",11, 4)
set_stone( "st-wood",11,10)
draw_stones( "st-wood", {12,6}, {0,2}, 3)
set_stone( "st-wood",12, 5)
draw_stones( "st-wood", {13,2}, {0,1}, 3)
draw_stones( "st-wood", {13,7}, {0,2}, 2)
draw_stones( "st-wood", {14,2}, {0,3}, 3)
draw_stones( "st-wood", {14,4}, {0,3}, 3)
draw_stones( "st-wood", {15,4}, {0,2}, 3)
set_stone( "st-wood",15, 9)
draw_stones( "st-wood", {16,2}, {0,3}, 2)
draw_stones( "st-wood", {16,7}, {0,3}, 2)
draw_stones( "st-wood", {17,2}, {0,2}, 3)
set_stone( "st-wood",17,10)
draw_stones( "st-wood", {18,3}, {0,2}, 4)


oxyd( 3, 3)
oxyd( 3, 9)
oxyd(16, 3)
oxyd(16, 9)

oxyd_shuffle()

set_actor("ac-blackball", 9.5, 6.5)
