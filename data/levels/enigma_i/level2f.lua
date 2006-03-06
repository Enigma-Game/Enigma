--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-marble")
fill_floor("fl-wood")

oxyd(3,2)
oxyd(8,3)
oxyd(13,4)
oxyd(16,3)
oxyd(12,8)
oxyd(8,6)
oxyd(9,10)
oxyd(18,11)
oxyd_shuffle()

draw_stones("st-marble", {7,8}, {1,0},3)
draw_stones("st-marble", {11,6}, {1,0},5)
draw_stones("st-marble", {13,8}, {1,0},6)
draw_stones("st-marble", {13,1}, {0,1},3)
draw_stones("st-marble", {16,4}, {0,1},2)
draw_stones("st-marble", {10,4}, {0,1},2)
draw_stones("st-marble", {10,7}, {0,1},4)
draw_stones("st-marble", {6,1}, {0,1},2)
draw_stones("st-marble", {6,4}, {0,1},2)
draw_stones("st-marble", {3,3}, {0,1},5)
draw_stones("st-marble", {11,10}, {1,0},8)
set_stones("st-marble", {{7,6},{9,3},{7,10}})

set_item("it-hollow", 2,9)
set_item("it-hollow", 2,11)
set_item("it-hollow", 3,11)
set_item("it-hollow", 4,11)
set_item("it-hollow", 5,11)
set_item("it-hollow", 6,11)
set_item("it-hollow", 7,11)
set_item("it-hollow", 7,9)

oneway(3,9, enigma.EAST)
oneway(4,9, enigma.EAST)
oneway(5,9, enigma.EAST)
oneway(6,9, enigma.EAST)
oneway(1,10, enigma.NORTH)

draw_stones("st-death", {3,8}, {1,0},4)
draw_stones("st-death", {2,10}, {1,0},5)
set_stone("st-death", 6,3)
set_stone("st-death", 6,6)
set_stone("st-death", 10,3)
set_stone("st-death", 10,6)
set_stone("st-death", 16,6)
set_stone("st-death", 9,9)

set_actor("ac-blackball", 18.5,9.5)















