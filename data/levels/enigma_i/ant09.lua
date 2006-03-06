-- Escher's World -- the Enigma Level
-- (c) 2002 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-10 -- keeping up to date with latest additions to ant.lua

dofile(enigma.FindDataFile("levels/lib/ant.lua"))
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
cells["."]=cell{}
cells[" "]=cell{floor="fl-normal"}
cells["v"]=cell{{{gradient, 1}}}
cells["^"]=cell{{{gradient, 2}}}
cells[">"]=cell{{{gradient, 3}}}
cells["<"]=cell{{{gradient, 4}}}
cells["O"]=cell{parent=cells[" "], actor={"ac-blackball",{player=0}}}
cells["0"]=oxyd

level = {
   "....................",
   ".O<<<< <<<<<<<<<<< .",
   ".v....v.........0.^.",
   ".v....v..........0^.",
   ".v....v...........^.",
   ".v....v...........^.",
   ".v.... >>00<< ....^.",
   ".v...........^....^.",
   ".v...........^....^.",
   ".v0..........^....^.",
   ".v.0.........^....^.",
   ". >>>>>>>>>>> >>>> .",
   "...................."
}


-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor = "b"
set_default_parent(abyss)
create_world_by_map(level)
oxyd_shuffle()















