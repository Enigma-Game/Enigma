--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 25

create_world(levelw, levelh)
draw_border("st-rock2")
fill_floor("fl-leaves")

oxyd_default_flavor = "d"

oxyd ( 4,14)
oxyd ( 6,14)
oxyd ( 8,14)
oxyd (10,14)
oxyd (12,14)
oxyd (14,14)
oxyd (16,14)
oxyd (18,14)

oxyd_shuffle()

draw_stones("st-rock2", { 2,10},{1,0}, 2)
draw_stones("st-rock2", { 4, 3},{0,1}, 3)
draw_stones("st-rock2", { 4, 7},{0,1}, 4)
draw_stones("st-rock2", { 5, 7},{1,0}, 3)
draw_stones("st-rock2", { 6, 5},{1,0}, 3)
draw_stones("st-rock2", { 7, 2},{0,1}, 2)
draw_stones("st-rock2", { 9, 2},{1,0}, 2)
draw_stones("st-rock2", { 9, 4},{0,1}, 2)
draw_stones("st-rock2", {11, 4},{0,1}, 2)
draw_stones("st-rock2", {11, 7},{0,1}, 4)
draw_stones("st-rock2", {12, 3},{0,1}, 2)
draw_stones("st-rock2", {12,10},{1,0}, 3)
draw_stones("st-rock2", {14, 3},{1,0}, 2)
draw_stones("st-rock2", {15, 6},{0,1}, 5)
draw_stones("st-rock2", { 2,12},{1,0}, 7)
draw_stones("st-rock2", {10,12},{1,0}, 9)
draw_stones("st-rock2", { 2,13},{0,1}, 8)
set_stones("st-rock2", {{15,4},{18,5}})
set_stones("st-block", {{12,6},{14,5},{17,5},{18,10}})

set_stone("st-shogun-s", 5, 4)
set_stone("st-shogun-s", 6, 8)
set_stone("st-shogun-s", 8, 2)
set_stone("st-shogun-s", 9, 7)
set_stone("st-shogun-s",10, 5)
set_stone("st-shogun-s",11, 6)
set_stone("st-shogun-s",15, 2)
set_stone("st-shogun-s",15, 5)
set_stone("st-shogun-s",17, 6)

shogundot1( 1, 1, {action="openclose", target="door1"})
shogundot1( 2, 1, {action="openclose", target="door2"})
shogundot1( 3, 1, {action="openclose", target="door3"})
shogundot1( 1, 2, {action="openclose", target="door4"})
shogundot1( 2, 2, {action="openclose", target="door5"})
shogundot1( 3, 2, {action="openclose", target="door6"})
shogundot1( 1, 3, {action="openclose", target="door7"})
shogundot1( 2, 3, {action="openclose", target="door8"})
shogundot1( 3, 3, {action="openclose", target="door9"})
shogundot1( 1, 4, {action="openclose", target="door10"})
shogundot1( 2, 4, {action="openclose", target="door11"})
shogundot1( 3, 4, {action="openclose", target="door12"})

set_stone("st-door", 1,12, {name="door1", type="h"})
set_stone("st-door", 1,13, {name="door2", type="h"})
set_stone("st-door", 1,14, {name="door3", type="h"})
set_stone("st-door", 1,15, {name="door4", type="h"})
set_stone("st-door", 1,16, {name="door5", type="h"})
set_stone("st-door", 1,17, {name="door6", type="h"})
set_stone("st-door", 1,18, {name="door7", type="h"})
set_stone("st-door", 1,19, {name="door8", type="h"})
set_stone("st-door", 1,20, {name="door9", type="h"})
set_stone("st-door", 5, 5, {name="door10", type="h"})
set_stone("st-door",10, 4, {name="door11", type="h"})
set_stone("st-door",13, 3, {name="door12", type="h"})

set_attrib(laser( 9,12, TRUE, NORTH), "name", "laser1")

set_actor("ac-blackball",9.5,6.5)
