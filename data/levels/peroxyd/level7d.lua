--
-- A level for Enigma
--
-- Copyright (c) 2002 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "d"

fill_floor("fl-water")
fill_floor("fl-gray", 1,1, level_width-2,level_height-2)

oxyd (0,3)
oxyd (0,9)
oxyd (19,3)
oxyd (19,9)

function yy1( color, x, y)
        stone = format( "st-%s4", color)
        set_stone( stone, x, y)
        set_stone( stone, x, y-1)
        set_stone( stone, x, y+1)
        set_stone( stone, x-1, y-1)
        set_stone( stone, x-1, y+1)
end

yy1( "white",  1, 3)
yy1( "black",  1, 9)

function yy2( color, x, y)
        stone = format( "st-%s4", color)
        set_stone( stone, x, y)
        set_stone( stone, x, y-1)
        set_stone( stone, x, y+1)
        set_stone( stone, x+1, y-1)
        set_stone( stone, x+1, y+1)
end

yy2( "white",  18, 9)
yy2( "black",  18, 3)

function yy3( color, x, y)
        stone = format( "st-%s4", color)
        set_stone( stone, x, y)
        set_stone( stone, x, y+1)
        set_stone( stone, x, y+2)
        set_stone( stone, x, y+3)
        set_stone( stone, x, y+4)
        set_stone( stone, x-1, y)
        set_stone( stone, x-2, y)
        set_stone( stone, x-1, y+4)
        set_stone( stone, x-2, y+4)
end

yy3( "white",  2, 7)
yy3( "black",  2, 1)

function yy4( color, x, y)
        stone = format( "st-%s4", color)
        set_stone( stone, x, y)
        set_stone( stone, x, y+1)
        set_stone( stone, x, y+2)
        set_stone( stone, x, y+3)
        set_stone( stone, x, y+4)
        set_stone( stone, x+1, y)
        set_stone( stone, x+2, y)
        set_stone( stone, x+1, y+4)
        set_stone( stone, x+2, y+4)
end

yy4( "white",  17, 1)
yy4( "black",  17, 7)

draw_stones("st-yinyang1", { 5,5}, {0,1},3)
draw_stones("st-yinyang1", { 6,4}, {0,1},5)
draw_stones("st-yinyang1", { 7,4}, {1,0},6)
draw_stones("st-yinyang1", { 7,8}, {1,0},6)
draw_stones("st-yinyang1", {13,4}, {0,1},5)
draw_stones("st-yinyang1", {14,5}, {0,1},3)
draw_stones("st-swap", { 7,5}, {0,1},3)
draw_stones("st-swap", {12,5}, {0,1},3)
draw_stones("st-swap", { 8,5}, {1,0},4)
draw_stones("st-swap", { 8,7}, {1,0},4)
draw_stones("st-grate2", { 3,4}, {0,1}, 5)
draw_stones("st-grate2", { 4,3}, {0,1}, 7)
draw_stones("st-grate2", {15,3}, {0,1}, 7)
draw_stones("st-grate2", {16,4}, {0,1}, 5)
draw_stones("st-grate2", { 5,3}, {1,0},10)
draw_stones("st-grate2", { 5,9}, {1,0},10)
set_stones("st-grate2", {{5,4},{14,4},{5,8},{14,8}})

set_item("it-yinyang",  9,6)
set_item("it-yinyang", 10,6)

set_actor("ac-blackball",  9.5, 6.5)
set_actor("ac-whiteball", 10.5, 6.5)










