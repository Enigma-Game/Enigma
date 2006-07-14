--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)
draw_border("st-rock5")
fill_floor("fl-space")
fill_floor("fl-tigris", 14,1,6,11)

draw_stones("st-oneway_white-e", {12,1}, {0,1},  2)
draw_stones("st-oneway_white-w", {12,3}, {0,1},  7)
draw_stones("st-oneway_white-e", {12,10}, {0,1}, 2)

draw_floor("fl-gradient", {1,1}, {1,0}, 11, {type=FLAT_FORCE_E})
draw_floor("fl-gradient", {1,11}, {1,0}, 11, {type=FLAT_FORCE_E})
set_floor("fl-gradient", 12,1, {type=SLOPE_S_FORCE_E})
set_floor("fl-gradient", 12,11, {type=SLOPE_N_FORCE_E})

oxyd( 3, 3)
oxyd( 3, 5)
oxyd( 3, 7)
oxyd( 3, 9)

oxyd( 5, 4)
oxyd( 5, 6)
oxyd( 5, 8)

oxyd( 7, 5)
oxyd( 7, 7)

oxyd( 9, 6)

oxyd_shuffle()

set_actor("ac-blackball", 16.5, 4.5)
set_actor("ac-whiteball", 1.5, 1.5, {player=0, mouseforce=0})
















