--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

dofile(enigma.FindDataFile("levels/lib/ant.lua"))

create_world(20, 13)
fill_floor("fl-abyss")

level =  {
 "                    ",
 " ##..--++++++--..## ",
 " 0#---.      .---#0 ",
 " ##....++++++....## ",
 " #O....|    |....## ",
 " ##----|    |----## ",
 " 0#....      ....#0 ",
 " ##----|    |----## ",
 " ##....|    |....## ",
 " ##....++++++....## ",
 " 0#---.      .---#0 ",
 " ##..--++++++--..## ",
 "                    "}

cells={}
cells[" "]=abyss
cells["#"]=cell{floor="fl-stwood"}
cells["+"]=cell{item="it-crack3",floor="fl-wood"}
if difficult then
  cells["-"]=cell{parent=cells["+"]}
else
  cells["-"]=cell{parent=cells["#"]}
end
if difficult then
  cells["."]=cell{parent=cells[" "]}
else
  cells["."]=cell{parent=cells["#"]}
end

if difficult then
  cells["|"]=cell{parent=cells["+"]}
else
  cells["|"]=cell{parent=cells[" "]}
end


oxyd_default_flavor = "a"

set_default_parent(cells["#"])
create_world_by_map(level,cells)

oxyd_shuffle()

enigma.ConserveLevel = FALSE















