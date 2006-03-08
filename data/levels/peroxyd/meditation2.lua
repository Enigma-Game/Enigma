levelw = 20
levelh = 13

create_world(levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-water")
fill_floor("fl-gray",   2,2, level_width-4,level_height-4)
fill_floor("fl-sand",  3,3, level_width-6,level_height-6)
fill_floor("fl-gray",   7,4, level_width-14,level_height-8)
fill_floor("fl-water", 8,5, level_width-16,level_height-10)

hollow(3,3)
hollow(level_width-4,level_height-4)
hollow(level_width-4, 3)
hollow(3,level_height-4)

set_actor("ac-whiteball-small", 6.7,5.7, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",13.3,5.7, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 6.7,7.3, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",13.3,7.3, {player=0, mouseforce=1})











