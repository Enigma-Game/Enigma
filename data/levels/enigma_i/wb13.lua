-- WB 13: (Counter)Clockwise
-- (c) 2003 Jon Sneyers
-- Licensed under GPL v2.0 or above
-- 2003-12-20

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells[";"]=cell{floor="fl-tigris"}
cells["6"]=cell{floor="fl-samba"}
cells["/"]=cell{floor="fl-stone"}
cells["9"]=cell{floor="fl-leaves"}
cells[" "]=cell{floor="fl-sand"}
cells[":"]=cell{floor="fl-rough"}
cells["5"]=cell{floor="fl-swamp"}
cells["w"]=cell{stone="st-wood"}
cells["W"]=cell{parent=cells[":"], stone={"st-wood"}}
cells["x"]=cell{stone="st-stoneimpulse"}
cells["X"]=cell{stone="st-stoneimpulse-hollow"}

cells["#"]=cell{stone="st-grate1"}
cells["="]=cell{stone="st-rock1"}
cells["R"]=cell{stone="st-rock7"}
cells["r"]=cell{parent=cells[":"],stone="st-stone_break"}
cells["p"]=cell{item="it-hammer"}
cells["Q"]=cell{item="it-blackbomb"}
cells["+"]=cell{item="it-spring2"}

cells["."]=cell{floor="fl-abyss"}
cells["@"]=cell{floor="fl-water"}
cells["S"]=cell{floor="fl-space"}

cells["H"]=cell{stone="st-chameleon"}

cells["&"]=cell{{{oneway, EAST}}}
cells["%"]=cell{{{oneway, WEST}}}
cells["l"]=cell{{{oneway, SOUTH}}}
cells["j"]=cell{{{oneway, NORTH}}}
cells["V"]=cell{parent=cells[" "], stone="st-bolder-s"}
cells[">"]=cell{parent=cells[" "], stone="st-bolder-e"}
cells["<"]=cell{parent=cells[" "], stone="st-bolder-w"}
cells["^"]=cell{parent=cells[" "], stone="st-bolder-n"}

cells["a"]=cell{stone={face="st-door", attr={name="doorA", type="v"}}}
cells["b"]=cell{stone={face="st-door", attr={name="doorB", type="v"}}}
cells["c"]=cell{stone={face="st-door", attr={name="doorC", type="v"}}}
cells["d"]=cell{stone={face="st-door", attr={name="doorD", type="h"}}}
cells["e"]=cell{stone={face="st-door", attr={name="doorE", type="h"}}}
cells["f"]=cell{stone={face="st-door", attr={name="doorF", type="h"}}}
cells["g"]=cell{stone={face="st-door", attr={name="doorG", type="h"}}}
cells["h"]=cell{stone={face="st-door", attr={name="doorH", type="v"}}}
cells["i"]=cell{stone={face="st-door", attr={name="doorI", type="h"}}}
cells["k"]=cell{stone={face="st-door", attr={name="doorK", type="v"}}}
cells["m"]=cell{stone={face="st-door", attr={name="doorM", type="v"}}}
cells["n"]=cell{stone={face="st-door", attr={name="doorN", type="v"}}}
cells["A"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorA"}}}
cells["B"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorB"}}}
cells["C"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorC"}}}
cells["D"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorD"}}}
cells["E"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorE"}}}
cells["F"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorF"}}}
cells["G"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorG"}}}
cells["H"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorH"}}}
cells["I"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorI"}}}
cells["K"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorK"}}}
cells["M"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorM"}}}
cells["N"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorN"}}}

cells["z"]=cell{stone={face="st-door", attr={name="doorZ", type="h"}}}
cells["Z"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorZ"}}}
cells["y"]=cell{stone={face="st-door", attr={name="doorY", type="h"}}}
cells["Y"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorY"}}}

cells["O"]=cell{actor={face="ac-blackball", attr={player=0}}}
cells["0"]=oxyd


level = {      
--  0123456789012345678|012345678901234567|012345678901234567|012345678901234567|0123456789012345678
   "================================================================================================",
   "=                  %                  %                  %                  %                  =",
   "=                  %                  %         O        %                  %                  =",
   "=llllllll========================0===j====================l====0=======================jjjjjjjj=",
   "=        =:::::::::::::::::::&::::::::::::::::::&::::::::::::::::::&:::::::::::::::::==        =",
   "=        0:::::::::::::::::::&::::::::::::::::::&::::::::::::::::::&::::::::::::::::::0        =",
   "=        00::::::::::::::::::===============0=======0===============:::::::::::::::::00        =",
   "=        0:::::::::::::::::::%::::::::::::::::::%::::::::::::::::::%::::::::::::::::::0        =",
   "=        =:::::::::::::::::::%::::::::::::::::::%::::::::::::::::::%:::::::::::::::::==        =",
   "=llllllll========================0=============================0=======================jjjjjjjj=",
   "=                  &                  &                  &                  &                  =",
   "=                  &                  &                  &                  &                  =",
   "================================================================================================",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()
oxyd_default_flavor = "b"
















