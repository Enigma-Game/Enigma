--Siegfried Fennig 15.01.2003 Rev. 0

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-rock1")
fill_floor("fl-gray")

hollow( 6, 6)
hollow(13, 6)

set_item("it-seed", 3, 9)

draw_stones("st-grate1", { 3, 3}, {1,0},14)
draw_stones("st-grate1", { 3, 9}, {1,0},14)
draw_stones("st-grate1", { 3, 4}, {0,1}, 5)
draw_stones("st-grate1", {16, 4}, {0,1}, 5)

set_item("it-yinyang",  1, 6)
set_item("it-yinyang", 18, 6)

local ac1=set_actor("ac-blackball", 1.5, 6.5)
local ac2=set_actor("ac-whiteball",18.5, 6.5)
local ac3=set_actor("ac-whiteball-small", 7.5, 6.5, {player=0, mouseforce=0})
local ac4=set_actor("ac-whiteball-small",12.5, 6.5, {player=0, mouseforce=0})

AddRubberBand(ac1, ac3, 16,0)
AddRubberBand(ac3, ac4, 12,0)
AddRubberBand(ac4, ac2, 16,0)