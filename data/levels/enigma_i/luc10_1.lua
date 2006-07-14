-- The Big Swamp, a level for Enigma
-- Copyright (C) 2005 Lukas Schueller
-- Licensed under GPL v2.0 or above
-- Thanks to Petr Machata for his ant.lua script and Nat Pryce for his natmaze.lua script
-----------------------------------------------------------------
-- I took this from Petr Machata's Meditation level

Require("levels/lib/ant.lua")
Require("levels/lib/natmaze.lua")

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
cells={}
normal = cell{floor="fl-swamp"}
wood = cell{floor="fl-samba"}
cells["w"]=cell{{{randomfloor, {normal, normal, normal, normal, wood}}}}
cells[" "]=cell{floor="fl-water"}
cells["Z"]=cell{{{get_limits}}}
level = {
  " 0                                    0 ",
  " Z O                                    ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                        ",
  "                                      Z ",
  " 0                                    0 "
}
set_default_parent(cells[" "])
create_world_by_map(level)
render_maze(new_kruskal_maze(mazew, mazeh), render_cell)
oxyd_shuffle()





















