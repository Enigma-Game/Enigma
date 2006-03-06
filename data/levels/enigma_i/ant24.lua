-- Meditation -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-08
-- 2003-02-09 -- enigma.ConserveLevel=FALSE added

dofile(enigma.FindDataFile("levels/lib/ant.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells[" "]=cell{floor="fl-bluegray"}
cells["_"]=cell{floor="fl-sahara"}
cells["#"]=cell{stone="st-rock1"}

puzzles = {};
cells["*"]=cell{{{add_multicell, puzzles}}}

cells["O"]=cell{item="it-hollow"}
cells["@"]=cell{{{wormhole, 2.5, 2.5, {range=1, strength=10}}}}
cells["o"]=cell{actor={"ac-whiteball-small", {player=0, mouseforce=1}}}
cells["6"]=cell{{cells["o"], cells["_"]}}

level = {
   "####################",
   "#*****        *****#",
   "#*6  *        *  o*#",
   "#*   *O O     *   *#",
   "#*   *        *   *#",
   "#*****O O  @  *****#",
   "#                  #",
   "#*****        *****#",
   "#*   *  @  @  *   *#",
   "#*   *        *   *#",
   "#*o  *        *  o*#",
   "#*****        *****#",
   "####################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
set_default_parent(cells[" "])
create_world_by_map(level)
render_puzzles(puzzles);
enigma.ConserveLevel=FALSE













