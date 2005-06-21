levelw = 20
levelh = 13

create_world(levelw, levelh)
enigma.ConserveLevel=FALSE
draw_border("st-brownie")

fill_floor("fl-sand")

fill_stones( "st-grate1", 1, 5, 5, 7)
fill_stones( "st-grate1", 14, 5, 5, 7)
fill_stones( "st-grate1", 6, 5, 8, 3)
fill_stones( "st-grate1", 9, 8, 2, 4)

draw_stones( "st-death", {1,6}, {1,0}, 6)
draw_stones( "st-death", {13,6}, {1,0}, 6)
draw_stones( "st-death", {1,6}, {1,0}, 6)
draw_stones( "st-death", {4,7}, {0,1}, 4)
draw_stones( "st-death", {15,7}, {0,1}, 4)
fill_stones( "st-death", 9, 5, 2, 2)
fill_stones( "st-death", 9, 9, 2, 1)

hollow( 7, 9)
hollow( 8,11)
hollow(11,11)
hollow(12, 9)

set_actor("ac-whiteball-small", 6.5, 5.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",13.5, 5.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 9.5, 7.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",10.5, 7.5, {player=0, mouseforce=1})

