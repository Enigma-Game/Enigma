-- Draggers -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-01-24
-- 2003-02-08 -- changed document text, as fl-metal looks the same as fl-normal
-- 2003-02-10 -- keeping up to date with latest additions to ant.lua

Require("levels/lib/ant.lua")
enigma.ConserveLevel=FALSE                  -- \dh\

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells[" "]=cell{floor="fl-metal"}
cells[","]=cell{floor="fl-normal"}
cells["."]=cell{floor="fl-abyss"}
cells[":"]=cell{parent=cells[","], stone="st-chameleon", item="it-seed"}
cells["+"]=cell{{{document, "This level is symmetric..."}}}

cells["#"]=cell{stone="st-rock1"}
cells["X"]=cell{stone="st-grate1"}
cells["0"]=oxyd

bolders = {}
actors = {}
cells["^"]=cell{{{add_multistone, "st-bolder-n", bolders}}}
cells["O"]=cell{{{add_multiactor, "ac-blackball", actors, {player=0}, 2}}}

level = {
   "####################",
   "# X.....0  0.....X #",
   "# X..............X #",
   "# X......  ......X #",
   "# X......  ......X #",
   "# X......  ......X #",
   "# X......  ......X #",
   "# X...        ...X #",
   "# X...0      0...X #",
   "# X......  ......X #",
   "# X......  ......X #",
   "# X......  ......X #",
   "# #......  ......# #",
   "# X......  ......X #",
   "# X......  ......X #",
   "# X......  ......X #",
   "# X......  ......X #",
   "# X......  ......X #",
   "# X....      ....X #",
   "# X....      ....X #",
   "# X....  ..  ....X #",
   "# X..0   ..   0..X #",
   "# X..    ..    ..X #",
   "# X....  ..  ....X #",
   "# #....  ..  ....# #",
   "# X....  ..  ....X #",
   "# X....  ..  ....X #",
   "# X              X #",
   "# X              X #",
   "# X  ..........  X #",
   "# X  ..........  X #",
   "# X  ..........  X #",
   "# X              X #",
   "# X              X #",
   "# X......  ......X #",
   "# X......  ......X #",
   "# #......  ......# #",
   "# X......  ......X #",
   "# X......  ......X #",
   "# X......  ......X #",
   "# X......  ......X #",
   "# X......  ......X #",
   "# X.....0  0.....X #",
   "# X              X #",
   "# X              X #",
   "# X  +        :  X #",
   "# X              X #",
   "#^X       O      X^#",
   "####################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor = "d"
set_default_parent(cells[" "])
create_world_by_map(level)
add_rubber_bands(actors, bolders, 5, 0)
enigma.ConserveLevel = FALSE


