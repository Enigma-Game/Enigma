-- Bolderrun, a level for Enigma
-- Copyright (C) JAHR JuSt
-- Licensed under GPL v2.0 or above
-- Level had been automatically created with Star.LUA Editor v0.32 beta

create_world(20, 13)
oxyd_default_flavor="b"

-- Floor Layer
fill_floor("fl-gravel")

-- Item Layer
set_item("it-wrench", 5, 1)

-- Stone Layer
oxyd(0, 11)
oxyd(0, 1)
draw_stones("st-stone1", {0, 0}, {1, 0}, 10)
draw_stones("st-stone1", {11, 0}, {1, 0}, 9)
draw_stones("st-stone1", {0, 2}, {1, 0}, 11)
draw_stones("st-stone1", {19, 2}, {0, 1}, 11)
draw_stones("st-stone1", {0, 3}, {0, 1}, 8)
draw_stones("st-stone1", {13, 4}, {0, 1}, 4)
draw_stones("st-stone1", {1, 5}, {1, 0}, 2)
draw_stones("st-stone1", {5, 5}, {0, 1}, 2)
draw_stones("st-stone1", {7, 6}, {1, 0}, 5)
draw_stones("st-stone1", {14, 7}, {1, 0}, 4)
draw_stones("st-stone1", {1, 8}, {1, 0}, 2)
draw_stones("st-rotator-right", {16, 8}, {0, 1}, 2, {movable=FALSE})
draw_stones("st-stone1", {5, 9}, {0, 1}, 2)
draw_stones("st-stone1", {7, 9}, {0, 1}, 2)
draw_stones("st-stone1", {9, 9}, {0, 1}, 2)
draw_stones("st-stone1", {1, 10}, {1, 0}, 4)
draw_stones("st-stone1", {10, 10}, {1, 0}, 2)
draw_stones("st-stone1", {16, 10}, {1, 0}, 2)
draw_stones("st-stone1", {18, 11}, {0, 1}, 2)
draw_stones("st-stone1", {0, 12}, {1, 0}, 18)
set_stones("st-rotator-right", {{10, 0}, {19, 1}}, {movable=FALSE})
set_stones("st-grate1", {{9, 1}, {6, 6}})
set_stone("st-bolder", 17, 1, {name="bolder3", direction=WEST})
set_stone("st-bombs", 11, 3)
set_stones("st-stone1", {{14, 4}, {4, 6}, {12, 7}, {17, 8}, {6, 10}, {8, 10}, {13, 10}})
set_stones("st-rotator_move-left", {{8, 5}, {16, 5}}, {movable=TRUE})
set_stone("st-stoneimpulse", 11, 7, {name="stoneimpulse2"})
set_stones("st-rotator-left", {{3, 8}, {12, 10}, {18, 10}}, {movable=FALSE})
set_stones("st-blocker", {{3, 11}, {5, 11}, {7, 11}, {9, 11}, {11, 11}, {13, 11}})

-- Actor Layer
set_actor("ac-blackball", 1.5, 1.5, {player="0", mouseforce="1"})

oxyd_shuffle()

-- Total Count: 50
-- Time: 0 sec 11 msec



























