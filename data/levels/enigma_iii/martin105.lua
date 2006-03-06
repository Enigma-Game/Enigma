--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

-- reused code from ant16.lua

dofile(enigma.FindDataFile("levels/lib/ant.lua"))
dofile(enigma.FindDataFile("levels/lib/natmaze.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
-- i took these from Nat Pryce's natmaze.lua maps
function cell_to_level( cellx, celly )
    return originx + cellx * 2, originy + celly * 2
end

function render_cell(maze, cellx, celly)
    x,y = cell_to_level(cellx, celly)

    _n = maze:can_go_north(cellx,celly)
    _s = maze:can_go_south(cellx,celly)
    _e = maze:can_go_east(cellx,celly)
    _w = maze:can_go_west(cellx,celly)

    if _s then
       cells["|"](x, y+1)
    end

    if _e then
      cells["-"](x+1, y)
    end

    if ( _n and _s and not _e and not _w ) then
       cells["|"](x, y)
    elseif ( not _n and not _s and _e and _w ) then
       cells["-"](x, y)
    else
       cells[" "](x, y)
    end
 end

-- to get the size of maze
originx, originy = nil, nil
mazew, mazeh = nil, nil

function get_limits(x,y)
   originx = originx or x
   originy = originy or y

   if (x>originx) then
      mazew = ceil((x-originx+1)/2)
   end

   if (y>originy) then
      mazeh = ceil((y-originy+1)/2)
   end
end

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
cells={}

cells[" "]=cell{floor="fl-rock"}
cells["."]=cell{floor="fl-abyss"}
cells["|"]=cell{item="it-vstrip"}
cells["-"]=cell{item="it-hstrip"}
cells["#"]=cells["."]

cells["o"]=cell{actor={"ac-blackball", {player=0, mouseforce=1}}}
cells["Z"]=cell{{{get_limits}}}

level = {
   "#0###############0##",
   "#Z.................#",
   "#..................#",
   "#..................#",
   "#..................#",
   "#..................#",
   "#..................#",
   "#..................#",
   "#..................#",
   "#..................#",
   "#..................#",
   "#................Zo#",
   "#0###############0##"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
set_default_parent(cells[" "])
create_world_by_map(level)
render_maze(new_kruskal_maze(mazew, mazeh), render_cell)

if not difficult then -- easy
    set_item( "it-umbrella", 1, 1)
end
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --















