-- Grow Your Own
-- A level for Enigma
-- Copyright (c) 2002 Nat Pryce
-- License: GPL v2.0 or above

dofile( enigma.FindDataFile("levels/lib/natmaze.lua") )


maze = new_kruskal_maze(38,24)   
originx = 1
originy = 1
visited = {}

function cell_to_level( cellx, celly )
    return originx + cellx * 2, %originy + celly * 2
end

function show_neighbours( cellx, celly )
    local x, y = cell_to_level( cellx, celly )
    
    if maze:can_go_west( cellx, celly ) then
        set_floor( "fl-gray", x-1, y )
        show_cell( cellx-1, celly )
    end
    if maze:can_go_east( cellx, celly ) then
        set_floor( "fl-gray", x+1, y )
        show_cell( cellx+1, celly )
    end
    if maze:can_go_north( cellx, celly ) then
        set_floor( "fl-gray", x, y-1 )
        show_cell( cellx, celly-1 )
    end
    if maze:can_go_south( cellx, celly ) then
        set_floor( "fl-gray", x, y+1 )
        show_cell( cellx, celly+1 )
    end
end

function show_cell( cellx, celly )
    cell_id = maze:coords_to_cell(cellx,celly)
    if visited[cell_id] then
        return
    end
    
    local x, y = cell_to_level( cellx, celly )
    function_name = "trigger_"..cell_id
    
    function trigger_callback( on )
        if on == 1 then
            show_neighbours( %cellx, %celly )
        end
    end
    setglobal( function_name, trigger_callback )
    
    set_floor( "fl-gray", x, y )
    set_item( "it-trigger", x, y, {action="callback",target=function_name} )
    
    visited[cell_id] = 1
end


create_world( maze.width*2 + 2, maze.height*2 + 2 )
fill_floor( "fl-abyss")

oxyd(1,0)
oxyd(2*maze.width-1,2*maze.height)
oxyd(1,2*maze.height)
oxyd(2*maze.width-1,0)

local startx, starty = random(maze.width)-1, random(maze.height)-1
local actorx, actory = cell_to_level( startx, starty )
set_floor( "fl-gray", actorx, actory )
show_neighbours( startx, starty )

set_actor( "ac-blackball", actorx + 0.5, actory + 0.5 )
display.SetFollowMode(display.FOLLOW_SCROLLING)


















