--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

stone="st-death"
create_world( levelw, levelh)
draw_border( stone)
draw_checkerboard_floor( "fl-normal", "fl-inverse", 0, 0, levelw, levelh)

draw_stones( stone, { 6,1}, {0,1}, 6)
draw_stones( stone, {13,6}, {0,1}, 6)
draw_floor( "fl-water", { 3,3}, {0,1}, 6)
draw_floor( "fl-water", {16,4}, {0,1}, 6)
draw_floor( "fl-water", {11,3}, {1,0}, 6)
draw_floor( "fl-water", { 3,9}, {1,0}, 6)
draw_floor( "fl-water", {10,3}, {0,1}, 5)
draw_floor( "fl-water", { 9,5}, {0,1}, 5)

oxyd( 0, 3)
oxyd( 0, 9)
oxyd(19, 3)
oxyd(19, 9)
oxyd(13, 0)
oxyd( 6,12)
oxyd(10, 6)
oxyd( 9, 6)
oxyd_shuffle()

set_actor("ac-blackball", 8.5, 2.5)
















