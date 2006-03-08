--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 134
levelh = 13

create_world( levelw, levelh)

draw_border( "st-woven")

draw_floor("fl-abyss", { 1, 4}, {1,0},  levelw-13)
draw_floor("fl-gradient", { 1, 5}, {1,0},  levelw-13, {type=15})
draw_floor("fl-gradient", { 1, 6}, {1,0},  levelw-13, {type=23})
draw_floor("fl-gradient", { 1, 7}, {1,0},  levelw-13, {type=16})
draw_floor("fl-abyss", { 1, 8}, {1,0},  levelw-13)

fill_floor("fl-woven", 1, 1, levelw-14,3)
fill_floor("fl-woven", 1, 9, levelw-14,3)
fill_floor("fl-woven", levelw-13, 1, 12, 11)

fill_floor("fl-water", levelw-11, 4, 5, 5)

draw_stones("st-oneway-w", {levelw-14, 1}, {0,1}, 2)
draw_stones("st-oneway-e", {levelw-14, 3}, {0,1}, 7)
draw_stones("st-oneway-w", {levelw-14,10}, {0,1}, 2)

set_floor("fl-woven",levelw-16, 4)
oneway(levelw-16, 4, SOUTH)
set_floor("fl-woven",levelw-16, 8)
oneway(levelw-16, 8, NORTH)

oxyd(levelw-30, 1)
oxyd(levelw-50,11)
oxyd(levelw-6, 5)
oxyd(levelw-6, 7)

draw_stones("st-woven", {levelw-6,4},{0,2},3)
oxyd_shuffle()


set_actor("ac-blackball", 1.5, 6.5)

-- Call this only after setting the main actor, or the actor probably
-- won't be inside the screen at when the game begins.
display.SetFollowMode(display.FOLLOW_SCROLLING)










