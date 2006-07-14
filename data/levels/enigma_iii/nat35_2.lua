-- Space Walk
-- A level for Enigma
-- Copyright (c) 2002 Nat Pryce
-- License: GPL v2.0 or above

Require("levels/lib/natmaze.lua")


function links_x( maze, cellx, celly )
    return maze:can_go_east(cellx,celly) and maze:can_go_west(cellx,celly)
end

function links_y( maze, cellx, celly )
    return maze:can_go_north(cellx,celly) and maze:can_go_south(cellx,celly) 
end

function leads_x( maze, cellx, celly )
    return maze:can_go_east(cellx,celly) or maze:can_go_west(cellx,celly)
end

function leads_y( maze, cellx, celly )
    return maze:can_go_north(cellx,celly) or maze:can_go_south(cellx,celly) 
end

function cell_is_straight( maze, cellx, celly )
    return links_x(maze,cellx,celly) and not leads_y(maze,cellx,celly)
        or links_y(maze,cellx,celly) and not leads_x(maze,cellx,celly)
end



originx = 1
originy = 1

function cell_to_level( cellx, celly )
    return originx + cellx * 2, originy + celly * 2
end

function render_cell( maze, cellx, celly )
    local x, y = cell_to_level( cellx, celly )
    
    if cell_is_straight( maze, cellx, celly ) then
        set_floor( "fl-space", x, y )
    else
        set_floor( "fl-gray", x, y )
        set_stone( "st-grate1", x, y )
    end
     
    if maze:can_go_south(cellx,celly) then
        set_floor( "fl-space", x, y+1 )
    end
    
    if maze:can_go_east(cellx,celly) then
        set_floor( "fl-space", x+1, y )
    end
end

do
    local maze = new_kruskal_maze(38,24)   
    
    create_world( maze.width*2 + 2, maze.height*2 + 2 )
    fill_floor( "fl-abyss")
    render_maze( maze, render_cell )
    
    oxyd(1,0)
    oxyd(2*maze.width-1,2*maze.height)
    oxyd(1,2*maze.height)
    oxyd(2*maze.width-1,0)

    -- ant 2003-02-13: actor should not appear at space cell
    local actorx, actory = 0,0
    repeat
       actorx, actory = random(maze.width)-1, random(maze.height)-1
    until (not(cell_is_straight(maze, actorx, actory)))

    actorx, actory = cell_to_level(actorx, actory)
    set_actor( "ac-blackball", actorx + 0.5, actory + 0.5,
               { player=0 } )
end
display.SetFollowMode(display.FOLLOW_SCROLLING)
















