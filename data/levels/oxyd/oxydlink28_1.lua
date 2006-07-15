--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-greenbrown")
fill_floor("fl-leaves")
enigma.ConserveLevel = FALSE

oxyd_default_flavor = "d"

oxyd( 1, 1)
oxyd( 1, 3)
oxyd( 1, 5)
oxyd( 1, 7)
oxyd( 1, 9)
oxyd( 1,11)
oxyd(18, 1)
oxyd(18, 3)
oxyd(18, 5)
oxyd(18, 7)
oxyd(18, 9)
oxyd(18,11)

oxyd_shuffle()

draw_stones("st-greenbrown", { 1, 2}, {1,0}, 4)
draw_stones("st-greenbrown", { 7, 2}, {1,0},12)
draw_stones("st-greenbrown", { 1, 4}, {1,0}, 4)
draw_stones("st-greenbrown", {15, 4}, {1,0}, 2)
draw_stones("st-greenbrown", { 3, 6}, {1,0}, 2)
draw_stones("st-greenbrown", {15, 6}, {1,0}, 4)
draw_stones("st-greenbrown", { 1, 8}, {1,0}, 4)
draw_stones("st-greenbrown", {15, 8}, {1,0}, 2)
draw_stones("st-greenbrown", { 3,10}, {1,0},10)
draw_stones("st-greenbrown", {15,10}, {1,0}, 4)
set_stones("st-greenbrown", {{1,6},{18,4},{1,10},{18,8}})

set_stone("st-oneway_black-w", 3, 3)
set_stone("st-oneway_black-e", 4, 5)
set_stone("st-oneway_black-n", 2, 6)
set_stone("st-oneway_white-s", 2,10)
set_stone("st-oneway_white-s",17, 4)
set_stone("st-oneway_white-w",15, 5)
set_stone("st-oneway_black-s",17, 8)
set_stone("st-oneway_black-w",15, 9)

set_stones("st-yinyang1",{{4,1},{9,1},{5,2},{6,2},{4,3},
                         {15,3},{4,7},{4,9},{15,7},{13,10},
                         {14,10},{10,11},{15,11}})

set_item("it-yinyang",  8, 6)
set_item("it-yinyang", 11, 6)

set_actor("ac-blackball", 8.5, 6.5)
set_actor("ac-whiteball",11.5, 6.5)











