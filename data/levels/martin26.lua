--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 25

stone="st-greenbrown"

create_world( levelw, levelh)
draw_border( stone)

draw_checkerboard_floor("fl-bluegreen", "fl-bluegreenx", 0,0,levelw,levelh)

draw_stones( stone, {2,5},{1,0},17)
oxyd( 1, 7)
oxyd( 3, 7)
oxyd( 5, 7)
oxyd( 7, 7)
oxyd( 9, 7)
oxyd(11, 7)
oxyd(13, 7)
oxyd(15, 7)
oxyd_shuffle()

draw_stones( stone, { 2,7},{0,1},11)
draw_stones( stone, { 4,7},{0,1},11)
draw_stones( stone, { 6,7},{0,1},11)
draw_stones( stone, { 8,7},{0,1},11)
draw_stones( stone, {10,7},{0,1},11)
draw_stones( stone, {12,7},{0,1},11)
draw_stones( stone, {14,7},{0,1},11)
draw_stones( stone, {16,7},{0,1},11)

oneway( 2,6, enigma.EAST)
oneway( 4,6, enigma.EAST)
oneway( 6,6, enigma.EAST)
oneway( 8,6, enigma.EAST)
oneway(10,6, enigma.EAST)
oneway(12,6, enigma.EAST)
oneway(14,6, enigma.EAST)

-- this doesn't work in version 0.50 and later.  The marble gets
-- crushed by the door.
-- set_item( "it-trigger", 1,5, {action="open", target="door1"})
-- doorv(16,6, {name="door1", state=OPEN})
-- set_item( "it-trigger", 17,6, {action="close", target="door1"})
oneway(16,6, EAST)

set_stone( "st-bolder-s", 1,17)
set_stone( "st-bolder-s", 3,17)
set_stone( "st-bolder-s", 5,17)
set_stone( "st-bolder-s", 7,17)
set_stone( "st-bolder-s", 9,17)
set_stone( "st-bolder-s",11,17)
set_stone( "st-bolder-s",13,17)
set_stone( "st-bolder-s",15,17)

draw_stones( "st-grate1", {1,18}, {1,0}, 18)

set_item( "it-magicwand", 1,18)

set_actor("ac-blackball", 3, 2)
