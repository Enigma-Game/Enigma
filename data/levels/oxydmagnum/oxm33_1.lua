--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 13)
oxyd_default_flavor = "d"

fill_floor("fl-bluegray")

draw_border("st-bluegray")
draw_stones("st-bluegray", {1,7}, {0,1}, 3)
draw_stones("st-bluegray", {18,7}, {0,1}, 5)

draw_stones( "st-stoneimpulse", {0,3}, {0,1}, 10)
draw_stones( "st-stoneimpulse", {1,12}, {1,0}, 19)
draw_stones( "st-stoneimpulse", {19,9}, {0,1}, 3)
draw_stones( "st-stoneimpulse-hollow", {2,9}, {1,0}, 16)
set_stone( "st-stoneimpulse", 18, 9)

draw_stones( "st-glass", {1,1}, {2,0}, 9)
draw_stones( "st-glass", {2,2}, {2,0}, 9)
draw_stones( "st-glass", {2,7}, {1,0},16)
draw_stones( "st-glass", {3,8}, {2,0}, 8)
set_stone( "st-glass",  1, 2)
set_stone( "st-glass", 18, 1)
set_stone( "st-glass",  2, 8)

draw_stones( "st-pmirror", {2,11}, {1,0}, 16, {movable=1})
draw_stones( "st-3mirror", {2,1}, {2,0}, 8, {orientation=1,movable=1})
draw_stones( "st-3mirror", {4,8}, {2,0}, 7, {orientation=3,movable=1})

draw_items( "it-trigger", {3,3}, {2,0}, 8)

laser( 1, 11, 1, EAST)

oxyd( 3,2)
oxyd( 5,2)
oxyd( 7,2)
oxyd( 9,2)
oxyd(11,2)
oxyd(13,2)
oxyd(15,2)
oxyd(17,2)

oxyd_shuffle()

set_actor("ac-blackball", 1.5, 4.5)











