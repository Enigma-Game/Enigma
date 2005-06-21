-- Siegfried Fennig 26.12.2002 Rev. 0

levelw = 59
levelh = 38

create_world(levelw, levelh)
fill_floor("fl-abyss", 0,0, levelw,levelh)
fill_floor("fl-space",19,12, 20,13)
fill_floor("fl-inverse",24,15, 10,7)
fill_floor("fl-inverse",21,18, 3,1)
fill_floor("fl-inverse",34,18, 3,1)

fill_floor("fl-inverse",22,16, 1,1)
fill_floor("fl-inverse",22,20, 1,1)
fill_floor("fl-inverse",35,16, 1,1)
fill_floor("fl-inverse",35,20, 1,1)

draw_floor("fl-gradient", {21,14}, {1,0},16, {type=1})
draw_floor("fl-gradient", {20,15}, {0,1}, 7, {type=3})
draw_floor("fl-gradient", {21,22}, {1,0},16, {type=2})
draw_floor("fl-gradient", {37,15}, {0,1}, 7, {type=4})
draw_floor("fl-gradient", {20,14}, {1,0}, 1, {type=5})
draw_floor("fl-gradient", {37,14}, {1,0}, 1, {type=6})
draw_floor("fl-gradient", {20,22}, {1,0}, 1, {type=7})
draw_floor("fl-gradient", {37,22}, {1,0}, 1, {type=8})

draw_stones("st-invisible", {18,11}, {1,0},22)
draw_stones("st-invisible", {18,25}, {1,0},22)
draw_stones("st-invisible", {18,12}, {0,1},13)
draw_stones("st-invisible", {39,12}, {0,1},13)

set_item("it-hollow", 22,16)
set_item("it-hollow", 22,20)
set_item("it-hollow", 35,16)
set_item("it-hollow", 35,20)

shogundot3 (35,13)
shogundot2 (21,15)
shogundot1 (19,19)
shogundot1 (34,20)
shogundot3 (23,23)

set_actor("ac-whiteball-small", 28,   18.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 28.5, 18.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 29,   18.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 29.5, 18.5, {player=0, mouseforce=1})

