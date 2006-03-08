--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

create_world( 20, 25)

stone = "st-rock3"

fill_floor( "fl-stone", 0, 0, 20, 25)

draw_border( stone)

draw_stones( stone, { 2, 8}, {1,0},  7)
draw_stones( stone, {11, 8}, {1,0},  7)
draw_stones( stone, { 2,10}, {1,0},  7)
draw_stones( stone, {11,10}, {1,0},  7)
draw_stones( stone, { 2,12}, {1,0},  7)
draw_stones( stone, {11,12}, {1,0},  7)
draw_stones( stone, { 2,14}, {1,0},  7)
draw_stones( stone, {11,14}, {1,0},  7)
draw_stones( stone, { 2,16}, {1,0},  7)
draw_stones( stone, {11,16}, {1,0},  7)
draw_stones( stone, { 2,18}, {1,0},  7)
draw_stones( stone, {11,18}, {1,0},  7)
draw_stones( stone, { 2,22}, {1,0},  7)
draw_stones( stone, {11,22}, {1,0},  7)
draw_stones( stone, { 2,11}, {0,1}, 11)
draw_stones( stone, {17,11}, {0,1}, 11)

draw_stones( "st-stoneimpulse", { 1, 1}, {0,1}, 23)
draw_stones( "st-stoneimpulse", {18, 1}, {0,1}, 23)
draw_stones( "st-stoneimpulse", { 2, 1}, {1,0},  7)
draw_stones( "st-stoneimpulse", {11, 1}, {1,0},  7)
draw_stones( "st-stoneimpulse", { 2, 9}, {1,0},  7)
draw_stones( "st-stoneimpulse", {11, 9}, {1,0},  7)
draw_stones( "st-stoneimpulse", { 2,13}, {1,0},  7)
draw_stones( "st-stoneimpulse", {11,13}, {1,0},  7)
draw_stones( "st-stoneimpulse", { 2,17}, {1,0},  7)
draw_stones( "st-stoneimpulse", {11,17}, {1,0},  7)
draw_stones( "st-stoneimpulse", { 2,23}, {1,0},  7)
draw_stones( "st-stoneimpulse", {11,23}, {1,0},  7)

set_stone( "st-grate1",10, 8)
set_stone( "st-grate1", 9,10)
set_stone( "st-grate1",10,12)
set_stone( "st-grate1", 9,14)
set_stone( "st-grate1",10,16)
set_stone( "st-grate1", 9,18)

set_stone( "st-wood", 9, 1)
set_stone( "st-wood", 9, 9)
set_stone( "st-wood", 9,13)
set_stone( "st-wood", 9,17)
set_stone( "st-wood", 9,23)

oneway( 9, 8, SOUTH)
oneway(10,10, NORTH)
oneway( 9,12, SOUTH)
oneway(10,14, NORTH)
oneway( 9,16, SOUTH)
oneway(10,18, NORTH)

set_stone( "st-death", 3,20)
set_stone( "st-death",16,20)

oxyd( 9, 0)
oxyd(10, 0)
oxyd( 3,11)
oxyd(16,11)
oxyd( 3,15)
oxyd(16,15)
oxyd( 9,24)
oxyd(10,24)

oxyd_shuffle()

set_actor("ac-blackball", 11.5, 4.5)










