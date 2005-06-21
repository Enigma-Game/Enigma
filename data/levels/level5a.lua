--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 25

create_world(levelw, levelh)
draw_border("st-marble")
fill_floor("fl-metal")

oxyd( 3, 4)
oxyd( 10, 2)
oxyd( 17, 10)
oxyd( 9, 22)
oxyd_shuffle()

hammer(9,10)

draw_stones("st-thief", {3,9}, {0,1},7)
draw_stones("st-thief", {5,9}, {0,1},7)
draw_stones("st-grate2", {4,9}, {0,1},7)
draw_stones("st-invisible", {4,16}, {0,1},1)

draw_stones("st-thief", {8,9}, {0,1},7)
draw_stones("st-thief", {10,9}, {0,1},7)
draw_stones("st-grate2", {9,9}, {0,1},6)

draw_stones("st-thief", {13,9}, {0,1},7)
draw_stones("st-thief", {15,9}, {0,1},7)
draw_stones("st-grate2", {14,9}, {0,1},6)

draw_stones("st-stone_break", {8,21}, {0,1},3)
draw_stones("st-stone_break", {10,21}, {0,1},3)
draw_stones("st-stone_break", {9,21}, {1,0},1)
draw_stones("st-stone_break", {9,23}, {1,0},1)

set_stone("st-key_a", 9, 15, {action="openclose", target="door1"})
set_stone("st-door_b", 14,15, {name="door1", type="h"})
set_item("it-key_a", 4 ,10)

draw_stones("st-marble", {1,12}, {1,0},2)
draw_stones("st-marble", {6,12}, {1,0},2)
draw_stones("st-marble", {11,12}, {1,0},2)
draw_stones("st-marble", {16,12}, {1,0},3)

set_actor("ac-blackball", 2.5, 6.5)
