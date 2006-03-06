create_world(20,25)
fill_floor("fl-bluegreen")
draw_border("st-greenbrown")

-- floors
fill_floor("fl-leaves",      1,  1, 18,  3)
fill_floor("fl-water",      14,  1,  5,  5)
fill_floor("fl-leaves",     16,  2,  2,  2)
fill_floor("fl-water",       1, 13, 12, 11)
fill_floor("fl-hay",         1,  5, 14,  7)
draw_floor("fl-gradient", { 15,  5}, {1, 0}, 4, {type=1})
draw_floor("fl-gradient", { 15,  6}, {1, 0}, 4, {type=2})
draw_floor("fl-gradient", { 15,  7}, {1, 0}, 4, {type=2})
draw_floor("fl-gradient", { 15,  8}, {1, 0}, 4, {type=2})
draw_floor("fl-gradient", { 15,  9}, {1, 0}, 4, {type=2})
draw_floor("fl-gradient", { 15, 10}, {1, 0}, 4, {type=2})
draw_floor("fl-gradient", { 15, 11}, {1, 0}, 4, {type=2})
fill_floor("fl-water",       1, 13, 12, 11)
fill_floor("fl-dunes",       4, 17,  5,  5)
fill_floor("fl-sand",       13, 13,  6, 11)
fill_floor("fl-water",      13, 21,  1,  2)
fill_floor("fl-sand",        4, 13,  9,  2)
fill_floor("fl-sand",        4, 15,  2,  2)

-- general walls
draw_stones("st-brick",      {  1,  4}, {1, 0}, 13)
draw_stones("st-greenbrown", {  1, 12}, {1, 0}, 18)
draw_stones("st-greenbrown", { 14,  5}, {0, 1}, 13)
enigma.KillStone(14, 8)
draw_stones("st-greenbrown", { 17, 17}, {1, 0},  2)
draw_stones("st-glass",      { 14, 21}, {1, 0},  5)
draw_stones("st-glass",      { 14, 22}, {0, 1},  2)

-- labirynth
draw_stones("st-greenbrown", {  1,  6}, {1, 0}, 12)
set_stone("st-greenbrown",      2,  7)
set_stone("st-greenbrown",      7,  8)
set_stone("st-greenbrown",      5,  9)
draw_stones("st-greenbrown", {  2,  8}, {1, 0},  4)
draw_stones("st-greenbrown", {  5, 10}, {1, 0},  3)
draw_stones("st-greenbrown", {  7,  9}, {1, 0},  3)
draw_stones("st-greenbrown", { 10,  8}, {0, 1},  3)
draw_stones("st-greenbrown", { 12,  8}, {0, 1},  3)
set_stone("st-greenbrown",     11,  8)
draw_stones("st-greenbrown", {  1, 10}, {1, 0},  3)
draw_stones("st-greenbrown", {  1, 11}, {1, 0},  2)

-- functional walls
set_stone("st-fart",             2,  1)
set_stone("st-bolder-e",         1,  2)
set_stone("st-greenbrown_move",  2,  2)
set_stone("st-greenbrown_move",  7,  2)
set_stone("st-greenbrown_move", 12,  2)
set_stone("st-key_a",            3,  4, {action="open", target="door1"})
doorh(4, 4, {name="door1"})
set_stone("st-invisible",        5,  5)
set_stone("st-greenbrown_hole",  2,  6)
set_stone("st-greenbrown_move",  4, 11)
set_stone("st-greenbrown_hole",  7, 11)
set_stone("st-greenbrown_hole", 16, 12)
set_stone("st-laser",           13, 13, {dir=SOUTH, name="laser1", on=0})
draw_stones("st-death",       { 16, 15}, {1, 0},  2)
set_stone("st-switch",          18, 15, {action="onoff", target="laser1"})
doorv(16, 16, {name="door2"})
set_stone("st-key_b",           16, 17, {action="open", target="door2"})
mirrorp(13, 23, FALSE, FALSE, 2)

-- items
set_item("it-trigger",          3, 11, {action="callback", target="kill_stone"})
set_item("it-key_b",            1,  3)
set_item("it-key_a",            4, 20)
set_item("it-hammer",           8, 20)
set_item("it-spring2",         11,  9)

-- oxyds
oxyd( 17,  2)
oxyd( 18,  7)
oxyd(  8, 10)
oxyd( 18, 14)
oxyd(  6, 19)
oxyd( 18, 19)
oxyd(  4, 23)
oxyd( 15, 23)
oxyd_shuffle()
set_actor("ac-blackball", 14.5, 8.5)

-- documents
document(4,2, "A hammer destroys some black stones.")
document(8,11, "Watch out for a disappearing stone.")

-- functions
function kill_stone()
	enigma.KillStone(2, 7)
end















