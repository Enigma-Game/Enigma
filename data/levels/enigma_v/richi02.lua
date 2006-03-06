-- Jesusbond, a level for Enigma
-- Copyright (C) 2005 Richi Buetzer
-- Licensed under GPL v2.0 or above

create_world(20, 13)
oxyd_default_flavor="b"

fill_floor("fl-wood")
draw_floor("fl-water", {2, 1}, {1, 0}, 15)
fill_floor("fl-water", 2, 2, 3, 10)
draw_floor("fl-water", {16, 2}, {0, 1}, 10)
set_floor("fl-water", 18, 2)
fill_floor("fl-water", 5, 9, 11, 3)
set_floor("fl-water", 18, 10)

draw_stones("st-greenbrown", {0, 0}, {1, 0}, 14)
draw_stones("st-greenbrown", {15, 0}, {1, 0}, 5)
draw_stones("st-greenbrown", {0, 1}, {0, 1}, 12)
draw_stones("st-glass", {5, 1}, {1, 0}, 11, {type="0"})
draw_stones("st-wood", {17, 1}, {0, 1}, 11)
draw_stones("st-greenbrown", {19, 1}, {0, 1}, 12)
draw_stones("st-glass", {5, 9}, {1, 0}, 11, {type="0"})
draw_stones("st-greenbrown", {1, 12}, {1, 0}, 4)
draw_stones("st-greenbrown", {6, 12}, {1, 0}, 9)
draw_stones("st-greenbrown", {16, 12}, {1, 0}, 3)
set_stone("st-laser", 14, 0, {name="laser9", dir=SOUTH, on="1"})
set_stones("st-wood", {{12, 4}, {12, 6}})
set_stone("st-laser", 5, 12, {name="laser1", dir=NORTH, on="1"})
set_stone("st-laser", 15, 12, {name="laser19", dir=NORTH, on="1"})

set_actor("ac-blackball", 9.5, 5.5, {player="0", mouseforce="1"})

oxyd(18, 11)
oxyd(1, 11)
oxyd(18, 1)
oxyd(1, 1)
oxyd_shuffle()





























