-- NoName, a level for Enigma
-- Copyright (C) 2005 Richi Buetzer
-- Licensed under GPL v2.0 or above

create_world(20, 13)
oxyd_default_flavor="b"

fill_floor("fl-water")
draw_floor("fl-stwood", {0, 0}, {0, 1}, 7)
draw_floor("fl-stwood", {1, 6}, {1, 0}, 3)
draw_floor("fl-wood", {2, 2}, {1, 0}, 3)
draw_floor("fl-wood", {15, 2}, {1, 0}, 3)
draw_floor("fl-wood", {2, 3}, {0, 1}, 2)
draw_floor("fl-wood", {4, 3}, {1, 0}, 2)
draw_floor("fl-wood", {7, 3}, {1, 0}, 6)
draw_floor("fl-wood", {14, 3}, {1, 0}, 2)
draw_floor("fl-wood", {17, 3}, {-1, 1}, 4)
draw_floor("fl-wood", {3, 4}, {0, 1}, 2)
draw_floor("fl-wood", {5, 4}, {1, 0}, 3)
fill_floor("fl-wood", 9, 4, 2, 6)
draw_floor("fl-wood", {12, 4}, {1, 0}, 3)
draw_floor("fl-wood", {17, 4}, {-1, 1}, 3)
draw_floor("fl-wood", {4, 6}, {1, 0}, 5)
draw_floor("fl-wood", {11, 6}, {1, 0}, 3)
draw_floor("fl-wood", {3, 7}, {0, 1}, 2)
draw_floor("fl-wood", {15, 7}, {1, 1}, 3)
draw_floor("fl-wood", {16, 7}, {1, 1},2)
draw_floor("fl-wood", {2, 8}, {0, 1}, 3)
draw_floor("fl-wood", {5, 8}, {1, 0}, 3)
draw_floor("fl-wood", {12, 8}, {1, 0}, 3)
draw_floor("fl-wood", {4, 9}, {1, 0}, 2)
draw_floor("fl-wood", {7, 9}, {1, 0}, 2)
draw_floor("fl-wood", {11, 9}, {1, 0}, 2)
draw_floor("fl-wood", {14, 9}, {1, 0}, 2)
draw_floor("fl-wood", {3, 10}, {1, 0}, 2)
draw_floor("fl-wood", {15, 10}, {1, 0}, 3)

draw_stones("st-actorimpulse", {4, 1}, {1, 1}, 3)
draw_stones("st-actorimpulse", {15, 1}, {-1, 1}, 3)
draw_stones("st-actorimpulse", {3, 3}, {1, 1}, 3)
draw_stones("st-actorimpulse", {16, 3}, {-1, 1}, 3)
draw_stones("st-actorimpulse", {1, 4}, {1, 1}, 2)
draw_stones("st-actorimpulse", {8, 4}, {-1, 1}, 2)
draw_stones("st-actorimpulse", {11, 4}, {1, 1}, 2)
draw_stones("st-actorimpulse", {18, 4}, {-1, 1}, 2)
draw_stones("st-actorimpulse", {2, 7}, {-1, 1}, 2)
draw_stones("st-actorimpulse", {5, 7}, {-1, 1}, 3)
draw_stones("st-actorimpulse", {7, 7}, {1, 1}, 2)
draw_stones("st-actorimpulse", {12, 7}, {-1, 1}, 2)
draw_stones("st-actorimpulse", {14, 7}, {1, 1}, 3)
draw_stones("st-actorimpulse", {17, 7}, {1, 1}, 2)
draw_stones("st-actorimpulse", {6, 9}, {-1, 1}, 3)
draw_stones("st-actorimpulse", {13, 9}, {1, 1}, 3)
set_stones("st-actorimpulse", {{7, 2}, {12, 2}, {7, 10}, {12, 10}})

set_actor("ac-blackball", 0.5, 0.5, {player="0", mouseforce="1"})

oxyd(17, 10)
oxyd(2, 10)
oxyd(17, 2)
oxyd(2, 2)
oxyd_shuffle()





























