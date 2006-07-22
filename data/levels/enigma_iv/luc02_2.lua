-- Antimaze, a level for Enigma
-- Copyright (C) 2005 Lukas Schueller
-- Licensed under GPL v2.0 or above 

-- May 06: Reduced Size -- Andreas

Require("levels/lib/natmaze.lua")

mfloors = {"fl-leaves","fl-wood","fl-rough-blue","fl-rough-red","fl-sand","fl-rough","fl-normal"}

originx = 1
originy = 1

function cell_to_level( cellx, celly )
  return originx + cellx * 2, originy + celly * 2
end

function fill_randomfloor(names, x0,y0, w,h)
    if x0 == nil then x0 = 0 end
    if y0 == nil then y0 = 0 end
    if w  == nil then w  = level_width end
    if h  == nil then h  = level_height end
    for y=y0,y0+h-1 do
	for x=x0,x0+w-1 do
            set_floor(names[random(getn(names))], x, y) 
        end
    end
end

stone_gothrough = "st-rock1_hole"
stone_block = "st-invisible"
--stone_gothrough = "st-grate1"
--stone_block = "st-rock1"

function render_cell( maze, cellx, celly )
  x,y = cell_to_level( cellx, celly )
  set_stone("st-rock1",x+1,y+1)
  if not maze:can_go_south(cellx,celly) then
    set_stone(stone_block,x,y+1)
  else
    set_stone(stone_gothrough,x,y+1)
  end
  if not maze:can_go_east(cellx,celly) then
    set_stone(stone_block,x+1,y)
  else
    set_stone(stone_gothrough,x+1,y)
  end
end

--local maze = new_kruskal_maze(38,24)
local maze = new_kruskal_maze(19,18)
--CreateWorld(maze.width*2+2,maze.height*2+2)
CreateWorld(maze.width*2+1, maze.height*2+1)
fill_randomfloor(mfloors)
render_maze(maze,render_cell)
draw_border("st-rock1")
oxyd(1,0)
oxyd(2*maze.width-1,2*maze.height)
oxyd(1,2*maze.height)
oxyd(2*maze.width-1,0)
oxyd_shuffle()
local actorx, actory = cell_to_level(random(maze.width)-1,random(maze.height)-1)
set_actor("ac-blackball", actorx + 0.5, actory + 0.5,{player=0})
--display.SetFollowMode(display.FOLLOW_SMOOTH)






























