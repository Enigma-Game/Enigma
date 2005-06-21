-- WB 9: Annoying little balls
-- (c) 2003 Jon Sneyers
-- Licensed under GPL v2.0 or above
-- 2003-12-20

dofile(enigma.FindDataFile("levels/ant.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells[" "]=cell{floor="fl-tigris"}
cells[";"]=cell{floor="fl-samba"}
cells[":"]=cell{floor="fl-stone"}
cells["w"]=cell{stone="st-wood"}
cells["i"]=cell{stone="st-stoneimpulse"}

cells["="]=cell{stone="st-rock5"}
cells["#"]=cell{stone="st-rock6"}
cells["."]=cell{floor="fl-abyss"}

cells["H"]=cell{stone="st-chameleon"}

cells["&"]=cell{{{oneway, EAST}}}
cells["%"]=cell{{{oneway, WEST}}}

cells["V"]=cell{parent=cells[" "], stone="st-bolder-s"}
cells[">"]=cell{parent=cells[" "], stone="st-bolder-e"}
cells["<"]=cell{parent=cells[" "], stone="st-bolder-w"}
cells["^"]=cell{parent=cells[" "], stone="st-bolder-n"}

cells["a"]=cell{stone={face="st-door", attr={name="doorA", type="v"}}}
cells["c"]=cell{stone={face="st-door", attr={name="doorC", type="h"}}}
cells["A"]=cell{item= {face="it-trigger", attr={action="open", target="doorA"}}}
cells["B"]=cell{item= {face="it-trigger", attr={action="close", target="doorA"}}}
cells["C"]=cell{item= {face="it-trigger", attr={action="open", target="doorC"}}}
cells["D"]=cell{item= {face="it-trigger", attr={action="close", target="doorC"}}}
cells["O"]=cell{actor={face="ac-blackball", attr={player=0}}}
cells["o"]=cell{actor={face="ac-whiteball-small", attr={player=0}}}
cells["0"]=oxyd

level = {      
   "####################",
   "0                o 0",
   "# ########0####### #",
   "# 0           o  0 #",
   "# # ############ # #",
   "# # 0          0 # #",
   "# 0 #          # 0 #",
   "# # 0     O    0 # #",
   "# # ############ # #",
   "# 0    o         0 #",
   "# ########0####### #",
   "0   o              0",
   "####################"
}
--  01234567890123456789

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor = "b"
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()
