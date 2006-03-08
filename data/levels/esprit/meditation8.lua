--Siegfried Fennig

levelw = 39
levelh = 13

create_world(levelw, levelh)
draw_border("st-wood_001")
fill_floor("fl-floor_001")

draw_stones("st-wood_001", {4,2}, {1,0},2)
draw_stones("st-wood_001", {5,3}, {1,0},4)
draw_stones("st-wood_001", {14,2}, {1,0},2)
draw_stones("st-wood_001", {11,3}, {1,0},4)
draw_stones("st-wood_001", {4,6}, {1,0},2)
draw_stones("st-wood_001", {2,5}, {1,0},3)
draw_stones("st-wood_001", {2,7}, {1,0},3)
draw_stones("st-wood_001", {5,9}, {1,0},4)
draw_stones("st-wood_001", {4,10}, {1,0},2)
draw_stones("st-wood_001", {8,4}, {0,1},5)
draw_stones("st-wood_001", {11,4}, {0,1},5)
draw_stones("st-wood_001", {11,9}, {1,0},4)
draw_stones("st-wood_001", {14,10}, {1,0},2)
draw_stones("st-wood_001", {14,6}, {1,0},2)
draw_stones("st-wood_001", {15,5}, {1,0},3)
draw_stones("st-wood_001", {15,7}, {1,0},3)
draw_stones("st-wood_001", {19,1}, {0,1},11)

set_stone("st-switch", 3,2, {action="trigger", target="h1"})
set_stone("st-switch", 3,10, {action="trigger", target="h2"})
set_stone("st-switch", 16,10, {action="trigger", target="h3"})
set_stone("st-switch", 16,2, {action="trigger", target="h4"})

set_item( "it-hollow", 7,6, {name="h1"})
set_item( "it-hollow", 9,6, {name="h2"})
set_item( "it-hollow",10,6, {name="h3"})
set_item( "it-hollow",12,6, {name="h4"})

set_stone( "st-timer", 21, 1, {action="callback", target="th1"})

timer = 0

function th1()
    if timer == 0 or timer == 5 then
        SendMessage("h1", "trigger")
        SendMessage("h2", "trigger")
        SendMessage("h3", "trigger")
        SendMessage("h4", "trigger")
    end
    timer = timer + 1
    if timer == 20 then
        timer = 0
    end
end

set_actor("ac-whiteball-small",  6.5, 2.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",  6.5,10.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 13.5, 2.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 13.5,10.5, {player=0, mouseforce=1})










