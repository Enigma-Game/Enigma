-- Laser Room II -- Oxyd Magnum -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-03-10

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
use_cells(cells, ".", "X")

cells["="]=doorh
cells[" "]=cell{floor="fl-bluegreen"}
cells["#"]=cell{stone="st-woven"}
cells["*"]=cell{stone="st-break_acwhite"}
cells[":"]=cell{{{mirrorp, FALSE, TRUE, 1}}}
cells["|"]=cell{{{mirrorp, FALSE, FALSE, 1}}}
cells["-"]=cell{{{mirrorp, FALSE, TRUE, 3}}}
cells["L"]=cell{{{laser, TRUE, EAST}}}
cells["o"]=cell{item= "it-extralife"}
cells["c"]=cell{item= {"it-coin4"}}
cells["d"]=cell{{{document, ""}}}
cells["x"]=cell{{cells["."], cells["X"]}}

level = {
   "####################",
   "#   X.0.X.0.XB     #",
   "#   X...X...X      #",
   "#   XXXXXXXXX      #",
   "#  L  :   |    B   #",
   "#                  #",
   "#    . .           #",
   "#    .*.     O #   #",
   "#    .o.           #",
   "#    .*. xxx       #",
   "#    .d. x0x       #",
   "#    .*. x0x       #",
   "#   #.c. xxx###=####",
   "#    ...           #",
   "#     B            #",
   "#                  #",
   "#     -    B    -  #",
   "#                  #",
   "#                  #",
   "#              xxx #",
   "#  #  -   |  B x0x #",
   "#   XXXXX      xxx #",
   "#   X...X     #    #",
   "#   X.0.X          #",
   "####################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

set_default_parent(cells[" "])
oxyd_default_flavor = "c"
create_world_by_map(level)
oxyd_shuffle()











