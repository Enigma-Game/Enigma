-- The Ditch, a level for Enigma
-- Copyright (C) 2005 Richi Buetzer
-- Licensed under GPL v2.0 or above

create_world(20, 13)
oxyd_default_flavor="b"

fill_floor("fl-plank")
fill_floor("fl-water", 12, 1, 2, 11)
fill_floor("fl-water", 15, 1, 4, 11)

set_item("it-document", 9, 3, {text="Setze dein Leben aufs Spiel..."})

draw_stones("st-greenbrown", {0, 0}, {0, 1}, 13)
draw_stones("st-greenbrown", {1, 0}, {1, 0}, 9)
draw_stones("st-greenbrown", {12, 0}, {1, 0}, 2)
draw_stones("st-greenbrown", {15, 0}, {1, 0}, 5)
draw_stones("st-greenbrown", {4, 1}, {0, 1}, 10)
draw_stones("st-greenbrown", {19, 1}, {0, 1}, 12)
draw_stones("st-greenbrown", {2, 2}, {0, 1}, 11)
draw_stones("st-wood", {9, 3}, {0, 1}, 2)
draw_stones("st-greenbrown", {3, 12}, {1, 0}, 11)
draw_stones("st-greenbrown", {15, 12}, {1, 0}, 4)
set_stone("st-laser", 10, 0, {name="laser1", dir=SOUTH, on="1"})
set_stone("st-laser", 11, 0, {name="laser2", dir=SOUTH, on="1"})
set_stone("st-wood", 14, 11)
set_stone("st-greenbrown", 1, 12)

set_actor("ac-blackball", 1.5, 11.5, {player="0", mouseforce="1"})

oxyd(14, 12)
oxyd(14, 0)
oxyd_shuffle()


























