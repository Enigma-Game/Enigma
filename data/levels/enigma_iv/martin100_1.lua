--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

world.DefineSimpleStoneMovable("fake_stone_move", "shatter", 0)
display.DefineAlias("fake_stone_move", "st-death")
world.DefineSimpleStone("fake_stone", "shatter", 0, 0)
display.DefineAlias("fake_stone", "st-death")

Require("levels/lib/ant.lua")

cells={}

cells[" "]=cell{floor="fl-abyss_fake"}
cells["."]=cell{floor="fl-swamp"}
cells["#"]=cell{stone="fake_stone"}
cells["+"]=cell{stone="fake_stone_move"}
cells["X"]=cell{stone="my_oxyd"}
cells["%"]=cell{stone="st-death_invisible"}
cells["h"]=cell{parent=cells["."],item="it-hammer"}
cells["d"]=cell{item="it-umbrella"}
cells["x"]=cell{actor={"ac-rotor", {range=25, force=40}}}
cells["o"]=cell{parent=cells["."],actor={"ac-blackball", {player=0, mouseforce=1}}}

level = {
   "####################",
   "#......          # #",
   "#..o...          # 0",
   "#....h.          # #",
   "##%####++++++++++++#",
   "#d%                #",
   "#%%                #",
   "#                  #",
   "#                  #",
   "#                  #",
   "#                  #",
   "#                  #",
   "#0##################"
}


set_default_parent(cells[" "])

create_world_by_map(level,cells)
















