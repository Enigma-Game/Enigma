-- Ice Breaker -- Oxyd Extra -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-05-11

Require("levels/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
cells[" "]=cell{floor="fl-ice_001"}
cells["#"]=cell{stone="st-rock5"}
cells["I"]=cell{stone="st-actorimpulse"}
cells["-"]=abyss
cells["."]=cell{item= {"it-crack1"}}
cells[","]=cell{item= {"it-crack2"}}
cells[":"]=cell{item= {"it-crack3"}}

level = {
   "####################",
   "#      I    I      #",
   "# D .     ,      D #",
   "#        .         #",
   "#   II       .II . #",
   "#        II ,,:,,  #",
   "#I. 0I   00 ..I0. I#",
   "#,,   .O II        #",
   "#,:.II   .    II   #",
   "#.:.           .   #",
   "#.D.     .       D #",
   "#      I    I      #",
   "####################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

set_default_parent(cells[" "])
oxyd_default_flavor = "c"
create_world_by_map(level)
oxyd_shuffle()
