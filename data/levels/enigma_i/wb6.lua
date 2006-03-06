-- WB 6: Spiral Race
-- (c) 2003 Jon Sneyers
-- Licensed under GPL v2.0 or above
-- 2003-12-20

dofile(enigma.FindDataFile("levels/lib/ant.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells[" "]=cell{floor="fl-tigris"}
cells[";"]=cell{floor="fl-samba"}
cells[":"]=cell{floor="fl-stone"}
cells["w"]=cell{stone="st-wood"}
cells["i"]=cell{stone="st-stoneimpulse"}

cells["="]=cell{stone="st-rock5"}
cells["G"]=cell{stone="st-grate1"}
cells["#"]=cell{stone="st-rock6"}
cells["."]=cell{floor="fl-abyss"}

cells["H"]=cell{stone="st-chameleon"}

cells["&"]=cell{{{oneway, EAST}}}
cells["%"]=cell{{{oneway, WEST}}}

cells["V"]=cell{parent=cells[" "], stone="st-bolder-s"}
cells[">"]=cell{parent=cells[" "], stone="st-bolder-e"}
cells["<"]=cell{parent=cells[" "], stone="st-bolder-w"}
cells["^"]=cell{parent=cells[" "], stone="st-bolder-n"}

cells["O"]=cell{actor={face="ac-blackball", attr={player=0}}}
cells["0"]=oxyd

level = {      
   "iiiiiiiiiiiiiiiiiiii",
   "i                  i",
   "i iiiiiiiiiiiiiiii i",
   "i i              i i",
   "i i iiiiiiiiiiii i i",
   "i i i#00000#wiii i i",
   "i i i#     G %   i i",
   "i i i#00000##iiiii i",
   "i i iiiiiiiiii     i",
   "i i            iii i",
   "i iiiiiiiiiiiiii iii",
   "i               w Oi",
   "iiiiiiiiiiiiiiiiiii#",
}
--  01234567890123456789

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor = "c"
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()















