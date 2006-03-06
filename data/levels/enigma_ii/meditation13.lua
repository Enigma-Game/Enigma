levelw = 20
levelh = 13

create_world(levelw, levelh)

fill_floor("fl-metal")

draw_border("st-rock1")

set_item( "it-magnet", 6, 6, {on=1, })

hollow(13, 8)
hollow(14, 6)
hollow(11, 5)
hollow(12, 9)

set_actor("ac-whiteball-small", 6.5, 5.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",13.5, 5.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 9.5, 7.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",10.5, 7.5, {player=0, mouseforce=1})














