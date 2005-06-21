--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)

fill_floor("fl-abyss")
fill_floor("fl-ice", 1,1, 18,11)

oxyd (0,0)
oxyd (19,0)
oxyd (0,12)
oxyd (19,12)
oxyd_shuffle()

draw_stones("st-greenbrown_move", {3, 2}, {1,0},14)
draw_stones("st-greenbrown_move", {3, 10}, {1,0},14)
draw_stones("st-greenbrown_move", {2, 3}, {0,1},7)
draw_stones("st-greenbrown_move", {17, 3}, {0,1},7)

draw_stones("st-greenbrown_move", {6, 4}, {1,0},8)
draw_stones("st-greenbrown_move", {6, 8}, {1,0},8)
draw_stones("st-greenbrown_move", {5, 5}, {0,1},3)
draw_stones("st-greenbrown_move", {14, 5}, {0,1},3)

set_stones("st-death", {{2,2},{2,10},{17,2},{17,10}})
set_stones("st-death", {{5,4},{5,8},{14,4},{14,8}})

set_actor("ac-blackball", 10.5, 6.5)

