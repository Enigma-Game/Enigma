-- Copyright (C) 2002 Daniel Heck 
-- Licensed under GPL v2.0 or above
create_world(20, 13)
draw_border("st-rock2")
fill_floor("fl-gray")

oxyd(1,1)
oxyd(level_width-2,level_height-2)
oxyd(level_width-2, 1)
oxyd(1,level_height-2)

fakeoxyd(1,2)
fakeoxyd(level_width-3,1)
fakeoxyd(2,11)
fakeoxyd(18,10)

draw_stones("st-rock2", {1,level_height/2}, {1,0}, level_width-2)
draw_stones("st-rock2", {level_width/2,1}, {0,1}, level_height/2-1)
draw_stones("st-rock2", {level_width/2-1,level_height/2}, {0,1}, level_height/2)

oneway(5,level_height/2, enigma.SOUTH)
oneway(9,9, enigma.EAST)
oneway(10,3, enigma.EAST)
oneway(14,6, enigma.SOUTH)

document(4,4, "Look closely!")
document(15,8, "... press F3 to try again ...")

draw_stones("st-grate1", {9,1},{0,1},5)
draw_stones("st-grate1", {11,1},{0,1},5)
draw_stones("st-grate1", {8,7},{0,1},5)
draw_stones("st-grate1", {10,7},{0,1},5)
set_item("it-magicwand",11,1)
set_item("it-yinyang", 12,1)
set_actor("ac-whiteball", 7,5)
set_actor("ac-blackball", 14,5)



















