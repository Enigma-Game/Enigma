--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-metal")
fill_floor("fl-gray")

draw_stones("st-metal",   {17,1}, {0,1}, 5)
draw_stones("st-metal",   {17,7}, {0,1}, 5)
draw_stones("st-metal",   {5,6}, {1,0}, 2)
draw_stones("st-metal",   {5,8}, {1,0}, 3)
draw_stones("st-metal",   {10,6}, {1,0}, 2)
draw_stones("st-metal",   {9,8}, {1,0}, 3)

set_stone("st-metal", 7,4)
set_stone("st-metal", 9,4)
set_stone("st-metal", 8,7)

draw_stones("st-grate1", {4,6}, {0,1} ,3)
draw_stones("st-grate1", {12,6}, {0,1} ,3)
draw_stones("st-grate1", {7,3}, {1,0} ,3)

set_stone("st-swap", 7,5)
set_stone("st-grate1", 9,5)

set_stone("st-door", 17, 6, {name="door", type="v"})

shogundot3(8,8, {target="door", action="openclose"})
set_stone("st-shogun-sml", 8,5)

oxyd(18,1)
oxyd(18,11)

set_actor("ac-blackball" ,2.5, 6.5)


















