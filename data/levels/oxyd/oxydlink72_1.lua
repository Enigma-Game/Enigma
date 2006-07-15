--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

create_world( 20, 25)

draw_border( "st-rock6")
fill_floor("fl-ice_001")

draw_stones("st-rock6", { 1,12}, {1,0},8)
draw_stones("st-rock6", {11,12}, {1,0},8)

draw_stones("st-yinyang1", { 1, 6}, {1,1},3)
draw_stones("st-yinyang1", { 3, 6}, {1,1},3)
draw_stones("st-yinyang1", { 5, 6}, {1,1},3)
draw_stones("st-yinyang1", { 7, 6}, {1,1},3)
draw_stones("st-yinyang1", { 9, 6}, {1,1},3)
draw_stones("st-yinyang1", {11, 6}, {1,1},3)
draw_stones("st-yinyang1", {13, 6}, {1,1},3)
draw_stones("st-yinyang1", {15, 6}, {1,1},3)
draw_stones("st-yinyang1", {17, 6}, {1,1},2)
draw_stones("st-yinyang2", { 1, 7}, {1,1},2)
draw_stones("st-yinyang2", { 2, 6}, {1,1},3)
draw_stones("st-yinyang2", { 4, 6}, {1,1},3)
draw_stones("st-yinyang2", { 6, 6}, {1,1},3)
draw_stones("st-yinyang2", { 8, 6}, {1,1},3)
draw_stones("st-yinyang2", {10, 6}, {1,1},3)
draw_stones("st-yinyang2", {12, 6}, {1,1},3)
draw_stones("st-yinyang2", {14, 6}, {1,1},3)
draw_stones("st-yinyang2", {16, 6}, {1,1},3)
draw_stones("st-yinyang1", { 1,16}, {1,1},3)
draw_stones("st-yinyang1", { 3,16}, {1,1},3)
draw_stones("st-yinyang1", { 5,16}, {1,1},3)
draw_stones("st-yinyang1", { 7,16}, {1,1},3)
draw_stones("st-yinyang1", { 9,16}, {1,1},3)
draw_stones("st-yinyang1", {11,16}, {1,1},3)
draw_stones("st-yinyang1", {13,16}, {1,1},3)
draw_stones("st-yinyang1", {15,16}, {1,1},3)
draw_stones("st-yinyang1", {17,16}, {1,1},2)
draw_stones("st-yinyang2", { 1,17}, {1,1},2)
draw_stones("st-yinyang2", { 2,16}, {1,1},3)
draw_stones("st-yinyang2", { 4,16}, {1,1},3)
draw_stones("st-yinyang2", { 6,16}, {1,1},3)
draw_stones("st-yinyang2", { 8,16}, {1,1},3)
draw_stones("st-yinyang2", {10,16}, {1,1},3)
draw_stones("st-yinyang2", {12,16}, {1,1},3)
draw_stones("st-yinyang2", {14,16}, {1,1},3)
draw_stones("st-yinyang2", {16,16}, {1,1},3)
set_stones("st-yinyang1", {{1,8},{1,18}})
set_stones("st-yinyang2", {{18,6},{18,16}})

function oxyd_base( color, x, y)
       stone = format( "st-%s4", color)
       set_stone( stone, x-1, y-1)
       set_stone( stone, x,   y-1)
       set_stone( stone, x+1, y-1)
       set_stone( stone, x-1, y)
       set_stone( stone, x+1, y)
       set_stone( stone, x-1, y+1)
       set_stone( stone, x,   y+1)
       set_stone( stone, x+1, y+1)
       oxyd( x, y)
end

oxyd_default_flavor = "b"

oxyd_base( "black", 2, 2)
oxyd_base( "black",13, 2)
oxyd_base( "black", 6,22)
oxyd_base( "black",17,22)
oxyd_base( "white", 6, 2)
oxyd_base( "white",17, 2)
oxyd_base( "white", 2,22)
oxyd_base( "white",13,22)

oxyd_shuffle()

set_item("it-yinyang",  3,10)
set_item("it-yinyang", 16,14)

set_actor("ac-blackball",  3.5,10.5)
set_actor("ac-whiteball", 16.5,14.5)












