--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

create_world( 20, 13)

fill_floor( "fl-brick", 0, 0, 20, 13)

draw_border( "st-brick")

draw_stones( "st-brick", { 3,4}, {0,1}, 5)
draw_stones( "st-brick", { 7,4}, {0,1}, 5)
draw_stones( "st-brick", {12,4}, {1,0}, 5)
draw_stones( "st-brick", {12,8}, {1,0}, 5)

draw_stones( "st-wood", { 4,4}, {1,0}, 3)
draw_stones( "st-wood", { 4,8}, {1,0}, 3)
draw_stones( "st-wood", {12,5}, {0,1}, 3)
draw_stones( "st-wood", {16,5}, {0,1}, 3)

set_stone( "st-grate1",  4,6)
set_stone( "st-grate1",  6,6)
set_stone( "st-grate1", 14,5)
set_stone( "st-grate1", 14,7)

oxyd_default_flavor = "d"

oxyd( 5, 6)
oxyd(14, 6)

set_item( "it-hammer", 9, 6)
set_item( "it-umbrella", 9, 7)
set_item( "it-dynamite", 10, 7)

set_actor("ac-blackball", 10, 6.5)










