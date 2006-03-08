levelw = 20
levelh = 13

create_world(levelw, levelh)
stone="st-woven"

draw_border( stone)
fill_floor("fl-stone")
draw_stones( stone, { 1,6}, {1,0}, 7)
draw_stones( stone, {12,6}, {1,0}, 7)
draw_stones( stone, { 5,1}, {0,1}, 4)
draw_stones( stone, {14,1}, {0,1}, 4)
draw_stones( stone, { 5,8}, {0,1}, 4)
draw_stones( stone, {14,8}, {0,1}, 4)
draw_stones( stone, { 7,2}, {0,1}, 9)
draw_stones( stone, {12,2}, {0,1}, 9)

hollow(2,2)
hollow(level_width-3,level_height-3)
hollow(level_width-3, 2)
hollow(2,level_height-3)

set_actor("ac-whiteball-small", 8.5,4.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",11.5,4.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 8.5,8.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",11.5,8.5, {player=0, mouseforce=1})











