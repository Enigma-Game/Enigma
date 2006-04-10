-- Castle Maze, a level for Enigma
-- Copyright (C) 2005 Lukas Schueller
-- Licensed under GPL v2.0 or above
-- Thanks to Nat Pryce for his natmaze.lua script
------------------------------------------------
-- Floors
floors = {
  "fl-rough",
  "fl-rough-red",
  "fl-rough-blue",
  "fl-normal",
  "fl-leaves",
  "fl-ice",
  "fl-bluegray",
  "fl-samba",
  "fl-sahara",
  "fl-sand",
  "fl-himalaya",
  "fl-sand",
  "fl-hay",
  "fl-white",
  "fl-lightgray",
  "fl-tigris",
  "fl-woven",
  "fl-plank",
  "fl-marble",
  "fl-space"
}
--------------------------------------------------
-- The Maze
Require("levels/lib/natmaze.lua")
maze = new_kruskal_maze(8,8)
startx, starty = random(maze.width-2), random(maze.height-2)
offsetx = 8.5
offsety = 5
cell_side_x = 18
cell_side_y = 11
cell_gap = 1
function cell_to_level( cellx, celly )
    local x = offsetx + cellx * cell_side_x + (cellx-1) * cell_gap-6
    local y = offsety + celly * cell_side_y + (celly-1) * cell_gap-3
    return x, y
end
function draw_cell( cellx, celly )
    local x,y = cell_to_level( cellx, celly )
    draw_border( "st-greenbrown", x-1, y-1, cell_side_x+2, cell_side_y+2 )
    fill_floor(floors[random(getn(floors))],x+2,y+2,14,7)
    fill_floor(floors[random(getn(floors)-1)],x+4,y+4,10,3)
end
function draw_cells()
    for celly = 0,maze.height-1 do
        for cellx = 0,maze.width-1 do
            draw_cell( cellx, celly )
        end
    end
end
function draw_links_for_cell( cellx, celly )
    local x,y = cell_to_level( cellx, celly )
    x=x+offsetx
    y=y+offsety
    if(maze.can_go_north(maze,cellx,celly)) then
      set_item("it-wormhole",x-1,y-offsety,{targetx=x,targety=y-2*offsety-1.5,range=1})
    end
    if(maze.can_go_south(maze,cellx,celly)) then
      set_item("it-wormhole",x-1,y+offsety,{targetx=x,targety=y+2*offsety+2.5,range=1})
    end
    if(maze.can_go_west(maze,cellx,celly)) then
      set_item("it-wormhole",x-offsetx,y,{targetx=x-2*offsetx-2,targety=y+0.5,range=1})
    end
    if(maze.can_go_east(maze,cellx,celly)) then
      set_item("it-wormhole",x+offsetx,y,{targetx=x+2*offsetx+2,targety=y+0.5,range=1})
    end
end
function draw_links()
    for celly = 0,maze.height-1 do
        for cellx = 0,maze.width-1 do
            draw_links_for_cell( cellx, celly )
        end
    end
end
function set_oxyd( cellx, celly )
    local x,y = cell_to_level( cellx, celly )
    oxyd(x,y)
end
create_world( maze.width*cell_side_x + (maze.width-1)*cell_gap + 2*offsetx + 2,
              maze.height*cell_side_y + (maze.height-1)*cell_gap + 2*offsety )
fill_floor( "fl-wood")
draw_cells()
draw_links()
set_oxyd( 0, 0)
set_oxyd( 0, maze.height-1)
set_oxyd( maze.width-1, 0)
set_oxyd( maze.width-1, maze.height-1)
actorx, actory = cell_to_level( startx, starty )
set_actor( "ac-blackball", actorx + offsetx, actory + offsety+0.5 )























