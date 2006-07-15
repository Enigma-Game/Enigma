--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-brownie")
fill_floor("fl-dunes")

oxyd(2,3)
oxyd(5,4)
oxyd(15,3)
oxyd(16,6)
oxyd(4,9)
oxyd(13,9)
oxyd_shuffle()

draw_stones("st-death", {8,0}, {1,0},4)
draw_stones("st-death", {8,12}, {1,0},4)

draw_stones("st-brownie", {1,1}, {1,1},8)
draw_stones("st-brownie", {1,3}, {1,1},8)
draw_stones("st-brownie", {1,5}, {1,1},7)
draw_stones("st-brownie", {1,7}, {1,1},5)
draw_stones("st-brownie", {1,9}, {1,1},3)
draw_stones("st-brownie", {1,11}, {1,1},1)
draw_stones("st-brownie", {3,1}, {1,1},6)
draw_stones("st-brownie", {5,1}, {1,1},4)
draw_stones("st-brownie", {7,1}, {1,1},2)
draw_stones("st-brownie", {12,1}, {1,1},7)
draw_stones("st-brownie", {14,1}, {1,1},5)
draw_stones("st-brownie", {16,1}, {1,1},3)
draw_stones("st-brownie", {18,1}, {1,1},1)
draw_stones("st-brownie", {11,2}, {1,1},8)
draw_stones("st-brownie", {11,4}, {1,1},8)
draw_stones("st-brownie", {11,6}, {1,1},6)
draw_stones("st-brownie", {11,8}, {1,1},4)
draw_stones("st-brownie", {11,10}, {1,1},2)

--document(13,1, "Heben Sie jeden Zettel auf, der irgendwo rumliegt ?")

set_actor("ac-blackball", 10.5,7.5)











