-- Shoals
-- A level for Enigma
--
-- Copyright (c) 2003 Nat Pryce
-- Licensed under the GPL version 2.

Require("levels/lib/natmaze.lua")

maze_width = 19
maze_height = 6

function cellx_to_worldx( cellx )
    return 2*cellx + 1
end

function celly_to_worldy( celly )
    return 2*celly + 1
end

function maze_floor(x,y)
    set_floor("fl-water", x, y)
end

function maze_wall(x,y)
    set_floor("fl-sand", x, y)
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

function cell_is_clear(x,y)
    for i = 1,getn(cells) do
        cell = cells[i]
        if cell[1] == x and cell[2] == y then
            return nil
        end
    end
    return 1
end

function random_safe_tile()
    repeat
        x = random(0,maze_width-2)
        y = random(0,maze_height-2)
    until cell_is_clear(x,y)
    
    tinsert(cells, {x,y})
    return {cellx_to_worldx(x)+1,celly_to_worldy(y)+1}
end


create_world( 39, 13 )
fill_floor("fl-water")
draw_maze()

for n = 1,8 do
    p = random_safe_tile()
    oxyd(p[1],p[2])
end

start = random_safe_tile()

set_item("it-spring1", start[1], start[2])
set_actor("ac-blackball", start[1]+0.5, start[2]+0.5, {player=0})
oxyd_shuffle()



























