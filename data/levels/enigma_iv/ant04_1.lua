-- The Tomb -- the Enigma Level
-- (c) 2002 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-10 -- keeping up to date with latest additions to ant.lua

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
cells["."]=abyss
cells[" "]=cell{floor="fl-sand"}
cells["#"]=cell{stone="st-greenbrown"}
cells["x"]=cell{stone="st-grate1"}
cells["@"]=cell{stone="st-wood"}
cells["O"]=cell{actor={face="ac-blackball", attr={player=0}}}
cells["+"]=cell{{{puzzle, PUZ_0000}}}
cells["-"]=cell{{{puzzle, PUZ_0101}}}
cells[","]=cell{{{puzzle, PUZ_0001}}}
cells["'"]=cell{{{puzzle, PUZ_0100}}}
cells["A"]=cell{stone={face="st-door", attr={name="doorA", type="v"}}}
cells["B"]=cell{stone={face="st-door", attr={name="doorB", type="h"}}}
cells["C"]=cell{stone={face="st-door", attr={name="doorC", type="h"}}}
cells["D"]=cell{stone={face="st-door", attr={name="doorD", type="v"}}}
cells["E"]=cell{stone={face="st-door", attr={name="doorE", type="h"}}}
cells["F"]=cell{stone={face="st-door", attr={name="doorF", type="v"}}}
cells["a"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorA"}}}
cells["b"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorB"}}}
cells["c"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorC"}}}
cells["d"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorD"}}}
cells["e"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorE"}}}
cells["f"]=cell{item= {face="it-trigger", attr={action="openclose", target="doorF"}}}
cells["0"]=oxyd

level = {
   ".............................",
   ".............................",
   "...##########.....###########",
   "####xx#a##xx#.....# @  @ ..0#",
   "#   x     @ #######   @@ ..0#",
   "# O # #x# bx A    ## @@  ..0#",
   "#e  x c  @##B# ##  # @   ..0#",
   "#@##xx### #  #-+##C#E##@ ..0#",
   "#  #### '- - .   +d,  #  ..0#",
   "##    D      ################",
   ".#############...............",
   ".............................",
   "............................."
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()
display.SetFollowMode(display.FOLLOW_SCROLLING)
















