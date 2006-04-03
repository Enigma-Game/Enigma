--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

-- reused code from ant16.lua
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above

Require("levels/lib/ant.lua")
Require("levels/lib/natmaze.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
-- i took these from Nat Pryce's natmaze.lua maps
function cell_to_level( cellx, celly )
    return originx + cellx * 2, originy + celly * 2
end

function render_cell(maze, cellx, celly)
    x,y = cell_to_level(cellx, celly)

    cells["w"](x, y)

    if maze:can_go_south(cellx,celly) then
       cells["w"](x, y+1)
    end

    if maze:can_go_east(cellx,celly) then
       cells["w"](x+1, y)
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

if (not difficult) then 
	cells[" "]=cell{floor="fl-bluegray"}
else
	cells[" "]=cell{floor="fl-bluegray",stone="st-grate2"}
end
cells["."]=cell{floor="fl-water"}
cells["w"]=cell{stone="st-knight",floor="fl-bluegray"}
cells["#"]=cell{stone="st-plain"}

cells["o"]=cell{actor={"ac-blackball", {player=0, mouseforce=1}}}
cells["|"]=cell{item="it-sword"}
cells["Z"]=cell{{{get_limits}}}

level = {
   "#0################0#",
   "#   Z............. #",
   "#  ............... #",
   "#  ............... #",
   "#  ............... #",
   "#  ............... #",
   "# o............... #",
   "#  ............... #",
   "#  ............... #",
   "#  ............... #",
   "#  ............... #",
   "# |.............Z  #",
   "#0################0#"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
set_default_parent(cells[" "])
create_world_by_map(level)
render_maze(new_kruskal_maze(mazew, mazeh), render_cell)
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --















