levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-glass")

fill_floor("fl-normal")

draw_floor("fl-gradient", { 2, 1}, {1,0}, 16, {type=1})
draw_floor("fl-gradient", { 2, 3}, {1,0}, 16, {type=2})
draw_floor("fl-gradient", { 2, 5}, {1,0}, 16, {type=2})
draw_floor("fl-gradient", { 2, 7}, {1,0}, 16, {type=1})
draw_floor("fl-gradient", { 2, 9}, {1,0}, 16, {type=1})
draw_floor("fl-gradient", { 2,11}, {1,0}, 16, {type=2})

set_floor("fl-gradient",  1,  1, {type=5})
set_floor("fl-gradient",  1,  2, {type=3})
set_floor("fl-gradient",  1,  3, {type=7})
set_floor("fl-gradient",  1,  5, {type=12})
set_floor("fl-gradient",  1,  6, {type=4})
set_floor("fl-gradient",  1,  7, {type=11})
set_floor("fl-gradient",  1,  9, {type=5})
set_floor("fl-gradient",  1, 10, {type=3})
set_floor("fl-gradient",  1, 11, {type=7})
set_floor("fl-gradient", 18,  1, {type=6})
set_floor("fl-gradient", 18,  2, {type=4})
set_floor("fl-gradient", 18,  3, {type=8})
set_floor("fl-gradient", 18,  5, {type=10})
set_floor("fl-gradient", 18,  6, {type=3})
set_floor("fl-gradient", 18,  7, {type=9})
set_floor("fl-gradient", 18,  9, {type=6})
set_floor("fl-gradient", 18, 10, {type=4})
set_floor("fl-gradient", 18, 11, {type=8})

hollow( 3, 6)
hollow( 7, 6)
hollow(12, 6)
hollow(16, 6)

set_actor("ac-whiteball-small", 5, 2, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",15, 2, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 5,11, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",15,11, {player=0, mouseforce=1})















