-- Bad Flowers
-- A Meditation Level for Enigma
--
-- Copyright (c) 2005 Nat Pryce
-- Licensed under the GPL version 2.


function blackball( x, y )
    return set_actor( "ac-blackball", x, y, {player=0, mouseforce=0, essential=1} )
end

function top( x, y )
    return set_actor( "ac-top", x, y, {force=200, range=20} )
end

function sumo( center_actor_constructor, x, y, radius, rubber_band_strength,
               outer_actor_constructor, outer_actor_count )
    local center = center_actor_constructor(x,y)
    local delta = 360 / outer_actor_count
    local outer_band_length = 2*radius*sin(360/(2*outer_actor_count))

    local first_outer = nil
    local prev_outer = nil
    local angle = 0
    while angle < 360 do
        local outer = place_actor( outer_actor_constructor, x, y, radius, angle )
        AddRubberBand( center, outer, rubber_band_strength, radius, radius )
        if prev_outer == nil then
            first_outer = outer
        else
            AddRubberBand( prev_outer, outer, 
                           rubber_band_strength, outer_band_length )
        end
        
        prev_outer = outer
        angle = angle + delta
    end
    AddRubberBand( prev_outer, first_outer, 
                   rubber_band_strength, outer_band_length )
end

function place_actor( actor_constructor, x, y, radius, angle )
    x = x + radius*cos(angle)
    y = y + radius*sin(angle)
    
    return actor_constructor( x, y )
end


create_world(20,13)
draw_border("st-metal")    
fill_floor("fl-leaves")
set_item("it-hollow", 2, 2)
set_item("it-hollow", level_width-3, 2)
set_item("it-hollow", 2, level_height-3)
set_item("it-hollow", level_width-3, level_height-3)

if options.Difficulty == HARD then
    alt_mouseforce = -1
else
    alt_mouseforce = 1
end

set_actor("ac-whiteball-small", 10, 2, {player=0, mouseforce=alt_mouseforce})
set_actor("ac-whiteball-small", 10, 5, {player=0})
set_actor("ac-whiteball-small", 10, 8, {player=0})
set_actor("ac-whiteball-small", 10, 11, {player=0, mouseforce=alt_mouseforce})


sumo( top, level_width/4, level_height/2, 1.25, 10000, blackball, 10 )
sumo( top, 3*level_width/4, level_height/2, 1.25, 10000, blackball, 10 )

--sumo( top, 5, 4, 1.25, 10000, blackball, 10 )
--sumo( top, 5, 9, 1.25, 10000, blackball, 10 )




























