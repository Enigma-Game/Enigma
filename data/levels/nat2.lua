-- Duck and Cover
-- A level for Enigma
-- Copyright (c) 2002 Nat Pryce
-- License: GPL v2.0 or above

create_world( 20, 13 )
oxyd_default_flavor = "c"       -- Default flavor for oxyd stones.


fill_floor( "fl-rough")
draw_border( "st-glass")    
draw_stones( "st-glass", {2,1}, {0,1}, level_height-2 )
draw_stones( "st-glass", {level_width-3,1}, {0,1}, level_height-2 )

lasers = {}

function next_laser_name()
    return "laser" .. getn(lasers)
end

function create_laser( x, y, dir )
    name = next_laser_name()
    set_stone( "st-laser", x, y, {on=TRUE, dir=dir, name=name} )
    tinsert( lasers, name )
end

for y = 1,level_height-2 do
    create_laser( 1, y, EAST )
    create_laser( level_width-2, y, WEST )
end

set_stone( "st-brownie", 8, 6 )
set_stone( "st-brownie", 11, 6 )
set_stone( "st-brownie", 9, 4 )
set_stone( "st-brownie", 10, 4 )
set_stone( "st-brownie", 9, 8 )
set_stone( "st-brownie", 10, 8 )


function toggle_laser( name )
    SendMessage(name, "onoff")
end
function timer_handler()
    for i = 1, getn(lasers) do
        toggle_laser( lasers[i] )
    end
end
set_stone( "st-timer", 0, 6,
           {action="callback", target="timer_handler", interval=4} )


for x = 4, 8, 2 do
    oxyd( x, 0 )
    oxyd( level_width-(x+1), 0 )
    oxyd( x, level_height-1 )
    oxyd( level_width-(x+1), level_height-1 )
end
oxyd_shuffle()

set_actor("ac-blackball", level_width/2.0, level_height/2.0)

