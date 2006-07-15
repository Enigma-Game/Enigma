--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 13)
oxyd_default_flavor = "d"

draw_border("st-marble")

fill_floor("fl-marble")

function puzzle_cross( x, y)
	puzzle( x,y, PUZ_1111)
	puzzle( x-1,y, PUZ_0100)
	puzzle(x+1,y, PUZ_0001)
	puzzle(x,y-1, PUZ_0010)
	puzzle(x,y+1, PUZ_1000)
end

puzzle_cross( 4,4)
puzzle_cross( 7,4)
puzzle_cross(10,4)
puzzle_cross(13,4)
puzzle_cross(16,4)

puzzle_cross( 4,7)
puzzle_cross( 7,7)
puzzle_cross(10,7)
puzzle_cross(13,7)
puzzle_cross(16,7)

draw_stones( "st-marble", {3, 1}, {3,0}, 5)
draw_stones( "st-grate1", {4, 2}, {3,0}, 5)
draw_stones( "st-grate1", {2, 3}, {3,0}, 6)
draw_stones( "st-grate1", {2, 6}, {3,0}, 6)
draw_stones( "st-grate1", {2, 9}, {3,0}, 6)
draw_stones( "st-grate1", {3,10}, {3,0}, 5)
draw_stones( "st-marble", {4,11}, {3,0}, 5)

draw_stones( "st-marble", { 1,4}, {0,3}, 2)
draw_stones( "st-marble", {18,4}, {0,3}, 2)

set_stone( "st-marble", 4, 2)
set_stone( "st-marble",10, 2)
set_stone( "st-marble",16, 2)
set_stone( "st-marble", 3,10)
set_stone( "st-marble", 9,10)
set_stone( "st-marble",12,10)

oxyd( 1,12)
oxyd( 5, 6)
oxyd( 8, 3)
oxyd( 8, 9)
oxyd(11, 6)
oxyd(14, 3)
oxyd(14, 9)
oxyd(19, 1)

oxyd_shuffle()

set_item( "it-yinyang", 1, 1)
set_item( "it-yinyang",18,11)

set_actor("ac-blackball", 18.5, 11.5)
set_actor("ac-whiteball",  1.5,  1.5)











