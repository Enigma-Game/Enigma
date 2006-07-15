-- Siegfried Fennig

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-rock2")
fill_floor("fl-bluegreen", 0,0, levelw,levelh)

draw_stones("st-rock2", {4,1}, {0,1},2)
draw_stones("st-rock2", {4,4}, {0,1},2)
draw_stones("st-rock2", {4,7}, {0,1},5)
draw_stones("st-rock2", {15,1}, {0,1},5)
draw_stones("st-rock2", {15,7}, {0,1},5)
draw_stones("st-rock2", {1,5}, {1,0},3)
set_stones("st-rock2", {{1,7},{3,7},{16,5},{16,7},{18,5},{18,7}})

set_floor("fl-bridge",  4,3, {name="bridge1"})
set_floor("fl-bridge",  2,7, {name="bridge2"})
set_floor("fl-bridge", 17,5, {name="bridge3"})
set_floor("fl-bridge", 17,7, {name="bridge4"})

function sdot1()
    SendMessage("bridge1", "openclose")
    SendMessage("bridge2", "openclose")
end

function sdot2()
    SendMessage("bridge3", "openclose")
    SendMessage("bridge4", "openclose")
end

shogundot1(4,6, {action="callback", target="sdot1"})
shogundot1(15,6, {action="callback", target="sdot2"})

set_stone("st-shogun", 6,6)
set_stone("st-shogun", 13,6)

set_item("it-hollow", 2,2)
set_item("it-hollow", 2,10)
set_item("it-hollow", 17,2)
set_item("it-hollow", 17,10)

set_actor("ac-whiteball-small",  7.5, 3.5, {player=0, mouseforce=1, essential=1})
set_actor("ac-whiteball-small",  13.5,2.5, {player=0, mouseforce=1, essential=1})
set_actor("ac-whiteball-small", 8.5, 9.5, {player=0, mouseforce=1, essential=1})
set_actor("ac-whiteball-small", 11.5,10.5, {player=0, mouseforce=1, essential=1})











