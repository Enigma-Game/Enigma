--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)

fill_floor("fl-swamp", 0,0,levelw,levelh)
fill_floor("fl-wood", 1, 1, levelw-2, levelh-2)
draw_border( "st-invisible")
set_item("it-document", 10, 6, {text="Some stones are movable."})

draw_stones("st-greenbrown_hole", {1,1},{1,0},18)
draw_stones("st-greenbrown_hole", {1,11},{1,0},18)
draw_stones("st-greenbrown_hole", {1,3},{1,0},2)
draw_stones("st-greenbrown_hole", {1,4},{0,1},7)
draw_stones("st-greenbrown_hole", {18,2},{0,1},9)
draw_stones("st-greenbrown", {1,2},{1,0},17)
draw_stones("st-greenbrown", {2,10},{1,0},16)
draw_stones("st-greenbrown", {2,4},{0,1},6)
draw_stones("st-greenbrown", {17,3},{0,1},1)
draw_stones("st-greenbrown", {17,5},{0,1},5)
draw_stones("st-greenbrown", {4,3},{1,1},4)
draw_stones("st-greenbrown", {3,6},{1,1},3)
draw_stones("st-greenbrown", {3,8},{1,1},2)
draw_stones("st-greenbrown_move", {3,4},{1,1},3)
draw_stones("st-greenbrown_move", {14,4},{1,1},3)
draw_stones("st-greenbrown_move", {14,6},{1,1},3)
draw_stones("st-greenbrown_move", {12,6},{1,1},3)
set_stones("st-greenbrown_hole", {{17,4}})
set_stones("st-greenbrown_move", {{16,4}})
set_stones("st-greenbrown", {{15,3},{17,3},{15,9},{6,7},{13,5}})

document(14,5, "You lost!")

oxyd( 6, 6)
oxyd( 13, 6)

set_actor("ac-blackball", 10, 6.5)
