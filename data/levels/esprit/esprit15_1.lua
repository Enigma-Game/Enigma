--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 39, 13)
oxyd_default_flavor = "a"

fill_floor( "fl-sahara")
fill_floor( "fl-abyss", 7,0, 6, 13)

draw_border( "st-rock4",  0, 0, 7, 13)
draw_border( "st-rock4", 13, 0, 7, 13)
draw_stones( "st-rock4", { 1,4}, {1,0},5)
draw_stones( "st-rock4", { 1,8}, {1,0},5)
draw_stones( "st-rock4", {14,4}, {1,0},5)
draw_stones( "st-rock4", {14,8}, {1,0},5)
draw_stones( "st-rock4", { 3,1}, {0,1},11)
draw_stones( "st-rock4", {16,1}, {0,1},11)

oxyd( 3, 2)
oxyd( 3, 6)
oxyd( 3,10)
oxyd(16, 2)
oxyd(16, 6)
oxyd(16,10)

oxyd_shuffle()

SetDefaultAttribs("it-wormhole", {range=5.0, strength=5})

set_item("it-wormhole", 1,  1, {targetx=4.5,targety=2.5})
set_item("it-wormhole", 5,  1, {targetx=14.5,targety=6.5})
set_item("it-wormhole",18,  6, {targetx=4.5,targety=5.5})
set_item("it-wormhole",14,  6, {targetx=17.5,targety=6.5})
set_item("it-wormhole",18, 11, {targetx=4.5,targety=10.5})
set_item("it-wormhole", 1,  6, {targetx=2.5,targety=10.5})
set_item("it-wormhole", 1, 11, {targetx=15.5,targety=2.5})
set_item("it-wormhole",14,  1, {targetx=18.5,targety=11.5})
set_item("it-wormhole", 5,  6, {targetx=2.5,targety=5.5})
set_item("it-wormhole",14, 11, {targetx=5.5,targety=6.5})
set_item("it-wormhole", 5, 11, {targetx=2.5,targety=2.5})
set_item("it-wormhole",18,  5, {targetx=17.5,targety=2.5})
set_item("it-wormhole",18,  7, {targetx=17.5,targety=10.5})
set_item("it-wormhole", 1,  7, {targetx=17.5,targety=1.5})
set_item("it-wormhole", 1,  5, {targetx=1.5,targety=2.5})
set_item("it-wormhole",18,  1, {targetx=15.5,targety=7.5})

set_actor("ac-blackball", 4.5, 6.5)











