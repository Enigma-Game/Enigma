--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

stone="st-rock1"

create_world( levelw, levelh)

draw_border( stone)
fill_floor("fl-water", 5, 5, 10, 3)

draw_floor("fl-gradient", { 2, 1}, {1,0},  16, {type=1})
draw_floor("fl-gradient", { 3, 2}, {1,0},  14, {type=1})
draw_floor("fl-gradient", { 4, 3}, {1,0},  12, {type=1})
draw_floor("fl-gradient", { 5, 4}, {1,0},  10, {type=1})

draw_floor("fl-gradient", { 2,11}, {1,0},  16, {type=2})
draw_floor("fl-gradient", { 3,10}, {1,0},  14, {type=2})
draw_floor("fl-gradient", { 4, 9}, {1,0},  12, {type=2})
draw_floor("fl-gradient", { 5, 8}, {1,0},  10, {type=2})

draw_floor("fl-gradient", { 1, 2}, {0,1},   9, {type=3})
draw_floor("fl-gradient", { 2, 3}, {0,1},   7, {type=3})
draw_floor("fl-gradient", { 3, 4}, {0,1},   5, {type=3})
draw_floor("fl-gradient", { 4, 5}, {0,1},   3, {type=3})

draw_floor("fl-gradient", {18, 2}, {0,1},   9, {type=4})
draw_floor("fl-gradient", {17, 3}, {0,1},   7, {type=4})
draw_floor("fl-gradient", {16, 4}, {0,1},   5, {type=4})
draw_floor("fl-gradient", {15, 5}, {0,1},   3, {type=4})

draw_floor("fl-gradient", { 1, 1}, { 1, 1},   4, {type=5})

draw_floor("fl-gradient", {18, 1}, {-1, 1},   4, {type=6})

draw_floor("fl-gradient", { 1,11}, { 1,-1},   4, {type=7})

draw_floor("fl-gradient", {18,11}, {-1,-1},   4, {type=8})



oxyd( 0, 3)
oxyd( 0, 9)
oxyd(19, 3)
oxyd(19, 9)
oxyd( 3, 0)
oxyd(16, 0)
oxyd( 3,12)
oxyd(16,12)

oxyd_shuffle()


set_actor("ac-blackball", 1.5, 11)
















