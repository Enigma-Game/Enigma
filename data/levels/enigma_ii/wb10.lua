-- WB 10: Meditation
-- (c) 2003 Jon Sneyers
-- Licensed under GPL v2.0 or above
-- 2003-12-20

dofile(enigma.FindDataFile("levels/lib/ant.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells[" "]=cell{floor="fl-wood"}
cells["_"]=cell{floor="fl-normal"}

cells["="]=cell{stone="st-rock5"}
cells["#"]=cell{stone="st-rock6"}
cells["."]=cell{floor="fl-abyss"}

cells["H"]=cell{stone="st-chameleon"}

cells["&"]=cell{{{oneway, EAST}}}
cells["%"]=cell{{{oneway, WEST}}}

cells["o"]=cell{actor={face="ac-whiteball-small", attr={player=0}}}
cells["O"]=hollow
cells["0"]=oxyd

level = {      
   "####################",
   "#O   %        &   O#",
   "######  O  O  ######",
   "#O  %          &  O#",
   "#####  o    o  #####",
   "#  %    oooo    &  #",
   "#O %    o##o    & O#",
   "#  %    oooo    &  #",
   "#####  o    o  #####",
   "#O  %          &  O#",
   "######  O  O  ######",
   "#O   %        &   O#",
   "####################"
}
--  01234567890123456789

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor = "b"
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()













