levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "a"       -- Default flavor for oxyd stones.

draw_border("st-rock1")
fill_floor("fl-gray")

hollow(7,3)
hollow(level_width-8,level_height-4)
hollow(level_width-8, 3)
hollow(7,level_height-4)

set_actor("ac-whiteball-small", 1.5,1.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 1.5,11.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 18.5,1.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 18.5,11.5, {player=0, mouseforce=1})











