levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "a"       -- Default flavor for oxyd stones.

draw_border("st-rock7")
--draw_checkerboard_floor ("fl-light", "fl-black", 0,0,levelw, levelh)
fill_floor("fl-black")

hollow(3,3)
hollow(level_width-4,level_height-4)
hollow(level_width-4, 3)
hollow(3,level_height-4)

set_actor("ac-whiteball-small", 10,4.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 4, 6.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 16,6.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 10,8.5, {player=0, mouseforce=1})















