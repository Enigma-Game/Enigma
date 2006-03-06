-- Knock Knock!
-- A level for Enigma
-- Copyright (c) 2002 Nat Pryce
-- License: GPL v2.0 or above

function noop()
    -- do nothing
end

function trigger( x, y )
    set_item( "it-trigger", x, y, {target="callback",action="idle"}  )
end

-- An island with a sandy beach

create_world(20,13)
fill_floor( "fl-water")
fill_floor( "fl-sand", 1, 1, level_width-2, level_height-2 )
fill_floor( "fl-leaves", 2, 2, level_width-4, level_height-4 )
fill_floor( "fl-gray", 10,3, 7, 7 )


-- A castle on the east side of the island

function toggle_door( toggle_on )
    if toggle_on == 1 then
        SendMessage( "door", "close" )
    else
        SendMessage( "door", "open" )
    end
end

draw_stones( "st-greenbrown", {11,3},  {1,0}, 5 )
draw_stones( "st-greenbrown", {11,9}, {1,0}, 5 )
set_stone( "st-greenbrown", 10, 4 )
set_stone( "st-greenbrown", 10, 5 )
doorv( 10, 6, {name="door"} )
set_stone( "st-greenbrown", 10, 7 )
set_stone( "st-greenbrown", 10, 8 )
draw_stones( "st-greenbrown", {16,4}, {0,1}, 5 )
y = random(5,7)
set_item( "it-trigger", 16, y, {action="callback",target="toggle_door"} )
set_stone( "st-greenbrown_move", 16, y )

oxyd_default_flavor = "a"       -- Default flavor for oxyd stones.
oxyd(10,3)
oxyd(11,4)
oxyd(16,3)
oxyd(15,4)
oxyd(10,9)
oxyd(11,8)
oxyd(16,9)
oxyd(15,8)


-- The puzzle to open the front door.

trigger(4,4)
trigger(5,5)
trigger(7,7)
trigger(8,8)
trigger(8,4)
trigger(7,5)
trigger(5,7)
trigger(4,8)

set_stone( "st-brownie", 3, 6 )
set_stone( "st-brownie", 6, 3 )
set_stone( "st-brownie", 6, 9 )
set_stone( "st-brownie", 9, 6 )


set_actor("ac-blackball", 6.5, 6.5)
















