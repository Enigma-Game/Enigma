-- Black Lava -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-01-08
-- 2003-02-10 -- keeping up to date with latest additions to ant.lua

dofile(enigma.FindDataFile("levels/ant.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells["n"]=cell{floor={face="fl-normal"}}

grada1=cell{{{gradient, 5}}}
grada2=cell{{{gradient, 6}}}
grada3=cell{{{gradient, 7}}}
grada4=cell{{{gradient, 8}}}
gradb1=cell{{{gradient, 9}}}
gradb2=cell{{{gradient, 10}}}
gradb3=cell{{{gradient, 11}}}
gradb4=cell{{{gradient, 12}}}
gradc1=cell{{{gradient, 21}}}
gradc2=cell{{{gradient, 22}}}
gradc3=cell{{{gradient, 23}}}
gradc4=cell{{{gradient, 24}}}

gradaa=cell{{{randomfloor,{grada1, grada2, grada3, grada4}}}}
gradbb=cell{{{randomfloor,{gradb1, gradb2, gradb3, gradb4}}}}
gradcc=cell{{{randomfloor,{gradc1, gradc2, gradc3, gradc4}}}}

cells[" "]=cell{{{randomfloor,{gradaa, gradbb, gradcc}}}}

cells["#"]=cell{stone="st-rock6"}
cells["D"]=cell{stone="st-death"}
cells["0"]=oxyd
cells["O"]=cell{parent=cells["n"],actor={"ac-blackball", {player=0}}}

level = {
   "####################",
   "#O       ##       n#",
   "#        ##        #",
   "0  D  D  DD  D  D  0",
   "#                  #",
   "#                  #",
   "0  D  D      D  D  0",
   "#                  #",
   "#                  #",
   "0  D  D  DD  D  D  0",
   "#        ##        #",
   "#n       ##       n#",
   "####################"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
randomseed(666)
oxyd_default_flavor = "b"
set_default_parent(cells[" "])
create_world_by_map(level)
randomseed(enigma.GetTicks())
oxyd_shuffle()
