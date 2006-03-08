levelw = 20
levelh = 13

create_world(levelw, levelh)

draw_border("st-rock3")

fill_floor("fl-marble")

state_h1 = 0
function trigger_h1()
    if state_h1 == 0 then
        state_h1 = 1
	SendMessage("h1", "trigger")
    else
        state_h1 = 0
    end
end

state_h2 = 0
function trigger_h2()
    if state_h2 == 0 then
        state_h2 = 1
        SendMessage("h2", "trigger")
    else
        state_h2 = 0
    end
end

state_h3 = 0
function trigger_h3()
    if state_h3 == 0 then
        state_h3 = 1
        SendMessage("h3", "trigger")
    else
        state_h3 = 0
    end
end

state_h4 = 0
function trigger_h4()
    if state_h4 == 0 then
        state_h4 = 1
        SendMessage("h4", "trigger")
    else
        state_h4 = 0
    end
end

draw_stones( "st-rock3", { 5,7}, {0,1}, 5)
draw_stones( "st-rock3", { 7,7}, {0,1}, 5)
draw_stones( "st-rock3", { 9,7}, {0,1}, 5)
draw_stones( "st-rock3", {11,7}, {0,1}, 5)
draw_stones( "st-rock3", {13,7}, {0,1}, 5)


set_item( "it-hollow", 6,11, {name="h1"})
set_item( "it-hollow", 8,11, {name="h2"})
set_item( "it-hollow",10,11, {name="h3"})
set_item( "it-hollow",12,11, {name="h4"})

set_item("it-trigger", 6, 8, {invisible=1, action="callback", target="trigger_h1"})
set_item("it-trigger", 8, 8, {invisible=1, action="callback", target="trigger_h2"})
set_item("it-trigger",10, 8, {invisible=1, action="callback", target="trigger_h3"})
set_item("it-trigger",12, 8, {invisible=1, action="callback", target="trigger_h4"})


set_actor("ac-whiteball-small", 2.7,5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",17.3,5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 3.7,5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",16.3,5, {player=0, mouseforce=1})

enigma.AddConstantForce(0,17)











