-- Stay in the Spotlight
-- A level for Enigma
-- 
-- Copyright (c) 2005 Nat Pryce
-- Licensed under the GPL, v2.0 or greater

enigma.ConserveLevel=FALSE
randomseed( enigma.GetTicks() )


function point( x, y )
    return {x=x,y=y}
end

function random_point()
    return point( 2 + random(level_width-6), 2 + random(level_height-6) )
end

function delta( from, to )
    if from > to then
        return 1
    elseif from < to then
        return -1
    else
        return 0
    end
end


create_world(39,25)

draw_border( "st-stone1", 1, 1, level_width-2, level_height-2 )

oxyd_default_flavor = "a"
oxyd( 0, 7 )
oxyd( 38, 7 )
oxyd( 0, 17 )
oxyd( 38, 17 )
oxyd( 7, 0 )
oxyd( 13, 0 )
oxyd( 19, 0 )
oxyd( 25, 0 )
oxyd( 31, 0 )
oxyd( 7, 24 )
oxyd( 13, 24 )
oxyd( 19, 24 )
oxyd( 25, 24 )
oxyd( 31, 24 )

set_stone( "st-grate1", 1, 7 )
set_stone( "st-grate1", 37, 7 )
set_stone( "st-grate1", 1, 17 )
set_stone( "st-grate1", 37, 17 )
set_stone( "st-grate1", 7, 1 )
set_stone( "st-grate1", 13, 1 )
set_stone( "st-grate1", 19, 1 )
set_stone( "st-grate1", 25, 1 )
set_stone( "st-grate1", 31, 1 )
set_stone( "st-grate1", 7, 23 )
set_stone( "st-grate1", 13, 23 )
set_stone( "st-grate1", 19, 23 )
set_stone( "st-grate1", 25, 23 )
set_stone( "st-grate1", 31, 23 )

set_actor("ac-blackball", 8, 8 )
oxyd_shuffle()


timer = set_stone( "st-timer", 19, 12, {action="callback", target="tick", interval=0.5} )
timer_action = null
timer_counter = 0

spotlight_destination = random_point()
spotlight_position = point(7,7)
spotlight_moving = 0


function schedule( time, action )
    timer_counter = time
    timer_action = action
end

function tick()
    timer_counter = timer_counter - 1
    if timer_counter == 0 then
        timer_action()
    elseif spotlight_moving then
        move_spotlight()
    end
end

function draw_spotlight()
    x = spotlight_position.x
    y = spotlight_position.y
    
    set_floor( "fl-leavesc1", x, y )
    set_floor( "fl-leavesc3", x+1, y )
    set_floor( "fl-leavesc2", x, y+1 )
    set_floor( "fl-leavesc4", x+1, y+1 )
end

function clear_spotlight()
    fill_floor( "fl-leavesb", spotlight_position.x, spotlight_position.y, 2, 2 )	
end

function move_spotlight()
    clear_spotlight()

    dx = delta( spotlight_destination.x, spotlight_position.x )
    dy = delta( spotlight_destination.y, spotlight_position.y )

    if dx == 0 and dy == 0 then
	spotlight_destination = random_point()
    else
        prefer_move_x = random() >= 0.5

        if dx ~= 0 and (dy == 0 or prefer_move_x) then
	    spotlight_position.x = spotlight_position.x + dx
        else
	    spotlight_position.y = spotlight_position.y + dy    
        end
    end

    draw_spotlight()
end

function flood_world()
    fill_floor("fl-water")
    fill_floor( "fl-leaves", spotlight_position.x, spotlight_position.y, 2, 2 )	
    spotlight_moving = false
    schedule( 5, drain_world )
end

function drain_world()
    fill_floor("fl-leavesb")
    draw_spotlight()
    spotlight_moving = 1
    
    schedule( 30, flood_world )
end


flood_world()
















