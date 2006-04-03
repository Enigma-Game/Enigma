-- Meditation -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-01-11
-- 2003-02-09 -- fixed originx/originy handling

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

cells[" "]=cell{floor="fl-bluegray"}
cells["~"]=cell{floor="fl-water"}
cells["w"]=cell{floor="fl-samba"}
cells["#"]=cell{stone="st-rock1"}

cells["+"]=cell{item={"it-magnet", {on=TRUE}}}

cells["o"]=cell{parent=cells["+"],actor={"ac-whiteball-small", {player=0, mouseforce=1, essential=1}}}
cells["O"]=cell{item="it-hollow"}
cells["Z"]=cell{{{get_limits}}}

level = {
   "####################",
   "#   Z~~~~~~~~~~~~  #",
   "# o ~~~~~~~~~~~~~ O#",
   "#   ~~~~~~~~~~~~~  #",
   "#   ~~~~~~~~~~~~~  #",
   "#   ~~~~~~~~~~~~~  #",
   "# o ~~~~~~~~~~~~~ O#",
   "#   ~~~~~~~~~~~~~  #",
   "#   ~~~~~~~~~~~~~  #",
   "#   ~~~~~~~~~~~~~  #",
   "# o ~~~~~~~~~~~~~ O#",
   "#   ~~~~~~~~~~~~Z  #",
   "####################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
set_default_parent(cells[" "])
create_world_by_map(level)
render_maze(new_kruskal_maze(mazew, mazeh), render_cell)
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --













