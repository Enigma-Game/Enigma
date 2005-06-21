levelw = 20
levelh = 13

create_world(levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-abyss")
fill_floor("fl-plank", 1,1, 9,6)
fill_floor("fl-plank",10,6, 9,6)

draw_stones( "st-glass", { 2,6}, {2,0}, 4)
draw_stones( "st-glass", {11,6}, {2,0}, 4)

hollow( 3, 1)
hollow( 7, 1)
hollow(12,11)
hollow(16,11)

set_actor("ac-whiteball-small", 3,5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 8,5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",12,8, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",17,8, {player=0, mouseforce=1})

