-- Bright Dots, a level for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above 
-- (Maze)Code partly taken from nat27.lua

Require("levels/lib/natmaze.lua")

-- MAZE CODE
maze_width = 4
maze_height = 6

function cellx_to_worldx( cellx )
    return originx + 2*cellx
end

function celly_to_worldy( celly )
    return originy + 2*celly
end

function maze_floor(x,y)
    set_floor("fl-abyss_fake",x,y)
    set_actor("ac-whiteball-small", x+0.5, y+0.5, {essential=0,controllers=2})
end

function maze_wall(x,y)
    set_floor("fl-abyss",x,y)

end

function render_cell( maze, cellx, celly )
    local x = cellx_to_worldx(cellx)
    local y = celly_to_worldy(celly)
    
    maze_floor(x,y)
    
    if maze:can_go_east(cellx,celly) then
        maze_floor(x+1,y)
    elseif maze:contains_cell(cellx+1,celly) then
    	maze_wall(x+1,y)
    end
    
    if maze:can_go_south(cellx,celly) then
        maze_floor(x,y+1)
    elseif maze:contains_cell(cellx,celly+1) then
        maze_wall(x,y+1)
    end
    
    if maze:contains_cell(cellx+1,celly+1) then
        maze_wall(x+1,y+1)
    end
end

function draw_maze()
    render_maze( new_kruskal_maze(maze_width,maze_height), render_cell )
end
cells = {}

-- World
create_world( 20, 13 )
fill_floor("fl-abyss")
fill_floor("fl-abyss_fake",9,1,2,13)
draw_floor("fl-abyss",{1,1},{0,1},11)
draw_floor("fl-abyss",{18,1},{0,1},11)
set_stone("st-grate1",9,6)
set_stone("st-grate1",10,6)
originx=2
originy=1
draw_maze()
originx=11
originy=1
draw_maze()
draw_border("st-glass2")

--OXYD
oxyd(1,1)
oxyd(1,11)
oxyd(18,1)
oxyd(18,11)
oxyd_default_flavor = "d"
oxyd_shuffle()

set_actor("ac-blackball", 10, 6+0.5, {player=0,essential=1})
oxyd_shuffle()











