-- Gopher It
-- A level for Enigma
-- Copyright (c) 2002 Nat Pryce
-- License: GPL v2.0 or above

dofile( enigma.FindDataFile("levels/lib/natmaze.lua") )

originx = 1
originy = 1

function cell_to_level( cellx, celly )
    return %originx + cellx * 2, %originy + celly * 2
end

function render_cell( maze, cellx, celly )
    x,y = cell_to_level( cellx, celly )
        
    set_stone( "st-marble", x+1, y+1 )
    
    if not maze:can_go_south(cellx,celly) then
        set_stone( "st-marble", x, y+1 )
    elseif random() < 0.025 then
        set_stone( "st-marble_hole", x, y+1 )
    end
    
    if not maze:can_go_east(cellx,celly) then
        set_stone( "st-marble", x+1, y )
    elseif random() < 0.025 then
        set_stone( "st-marble_hole", x+1, y )
    end
end


do
    local maze = new_kruskal_maze(38,24)
    
    create_world( maze.width*2 + 2, maze.height*2 + 2 )
    draw_border( "st-marble" )
    fill_floor( "fl-rough")
    render_maze( maze, render_cell )
    
    oxyd(1,0)
    oxyd(2*maze.width-1,2*maze.height)
    oxyd(1,2*maze.height)
    oxyd(2*maze.width-1,0)
    oxyd_shuffle()
    
    local actorx, actory = cell_to_level( random(maze.width)-1, 
                                          random(maze.height)-1 )
    set_actor( "ac-blackball", actorx + 0.5, actory + 0.5,
               { player=0 } )
end
display.SetFollowMode(display.FOLLOW_SCROLLING)
















