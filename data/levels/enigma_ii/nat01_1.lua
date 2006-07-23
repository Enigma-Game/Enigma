-- Indoor Pool
-- A level for Enigma
-- Copyright (c) 2002 Nat Pryce
-- License: GPL v2.0 or above


create_world(20,13)
oxyd_default_flavor = "a"       -- Default flavor for oxyd stones.


draw_border("st-metal")    
fill_floor("fl-leaves")
fill_floor("fl-gray", 5,3, level_width-10, level_height-6 )
set_floor("fl-gray", level_width-5, level_height/2 )

draw_stones( "st-puzzle", {0, 6}, {1,0}, 6, {connections=PUZ_0101} )
set_stone( "st-puzzle", 6, 6, {connections=PUZ_0001} )
set_stone( "st-metal", 7, 6 )

draw_stones( "st-metal", {5,2}, {1,0}, 5 )
draw_stones( "st-metal", {9,3}, {1,0}, 6 )
draw_stones( "st-metal", {14,3}, {0,1}, 3 )
set_stone( "st-metal", 15, 5 )

draw_stones( "st-metal", {5,10}, {1,0}, 5 )
draw_stones( "st-metal", {9,9}, {1,0}, 6 )
draw_stones( "st-metal", {14,9}, {0,-1}, 3 )
set_stone( "st-metal", 15, 7 )

draw_stones( "st-metal", {5,4}, {1,0}, 3 )
draw_stones( "st-metal", {5,8}, {1,0}, 3 )

draw_stones( "st-metal", {10,5}, {1,0}, 3 )
draw_stones( "st-metal", {10,7}, {1,0}, 3 )

draw_floor( "fl-water", {9,6}, {1,0}, 4 )

draw_stones( "st-grate1", {6,3}, {0,2}, 4 )
draw_stones( "st-wood",   {7,3}, {0,2}, 4 )
draw_stones( "st-wood",   {8,5}, {0,2}, 2 )

set_stone( "st-wood", 14, 2 )
set_stone( "st-wood", 15, 3 )
set_stone( "st-wood", 14, level_height-3 )
set_stone( "st-wood", 15, level_height-4 )

set_item( "it-trigger", 11,4, {action="openclose", target="door1"})
set_item( "it-trigger", 11,8, {action="openclose", target="door2"})

oxyd(1,1)
set_stone( "st-metal", 2, 1 )
set_stone( "st-metal", 2, 2 )
doorv( 2, 3, {name="door1"} )
set_stone( "st-metal", 2, 4 )
set_stone( "st-metal", 2, 5 )
oxyd(1,5)

oxyd(1,level_height-2)
set_stone( "st-metal", 2, level_height-2 )
set_stone( "st-metal", 2, level_height-3 )
doorv( 2, level_height-4, {name="door2"} )
set_stone( "st-metal", 2, level_height-5 )
set_stone( "st-metal", 2, level_height-6 )
oxyd(1,level_height-6)

oxyd_shuffle()

set_actor("ac-blackball", level_width-2.5, level_height/2.0)



















