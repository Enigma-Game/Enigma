-- Paper Chase -- Oxyd Extra -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-05-12
-- 2003-06-21 -- fixed

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
cells={}
use_cells(cells, "X",")")
cells[" "]=cell{floor="fl-rough"}
cells["#"]=cell{stone="st-rock5"}
cells["D"]=cell{stone="st-death_invisible"}
cells["F"]=fakeoxyd
cells["x"]=cell{ cells["X"]; item="it-extralife"}
cells["Y"]=cell{{cells["X"], cells[")"]}}
cells["p"]=cell{item={"it-document", {text="oh..."}}}


level = {
   "####################",
   "#xFXXFXX0XXFXXFXXFX#",
   "# D   p D  D  D p  #",
   "#X0XXFXXFXX0xX0XXFX#",
   "# D  D   p  p D  D #",
   "#XFXXFxXFXXFXXFXXFx#",
   "# D  D  DO D  D  D #",
   "#XFxXFXXFXYFXXFXxFX#",
   "# D  D  D  D  D  D #",
   "#XFXX0XXFXxFXXFXX0X#",
   "#  p D  D  D p   D #",
   "#XFXXFXXFXXFXXFXXFX#",
   "####################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

set_default_parent(cells[" "])
oxyd_default_flavor = "b"
create_world_by_map(level)
oxyd_shuffle()











