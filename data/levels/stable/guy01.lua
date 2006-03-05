-- Guitar / CDEmG, a level for Enigma
-- Copyright (C) 2005/07/14 Guy Busser
-- Licensed under GPL v2.0 or above 

dofile(enigma.FindDataFile("levels/lib/ant.lua"))
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
cells["."]=cell{floor="fl-leaves"}
cells["+"]=cell{{cells["."], {document, "C D Em G"}}}
cells["<"]=cell{floor="fl-wood"}
cells["-"]=cell{floor="fl-light"}
cells["T"]=cell{floor="fl-woven"}
cells[">"]=cell{floor="fl-black"}
cells["P"]=cell{floor="fl-woven",stone="st-wood"}
cells["Q"]=cell{floor="fl-leaves",stone="st-grate1"}
cells["&"]=cell{floor="fl-leaves",stone={"st-oneway",{orientation=WEST}}}
cells["!"]=cell{floor="fl-leaves",stone={"st-oneway",{orientation=EAST}}}
cells["S"]=cell{floor="fl-water"}
cells["U"]=cell{floor="fl-leaves",stone="st-wood.growing"}

cells["V"]=cell{floor="fl-wood",item="it-tinyhollow"}
cells["W"]=cell{floor="fl-leaves",item="it-seed"}
cells["w"]=cell{floor="fl-leaves",stone="st-grate1"}
cells["x"]=cell{floor="fl-leaves",stone="st-wood"}
cells["Z"]=cell{floor="fl-light",stone="st-grate1"}
cells["O"]=cell{parent=cells["<"],actor={"ac-blackball",{player=0}}}
cells["r"]=cell{stone="st-rock4"}
cells["a"]=cell{floor="fl-leaves",item={face="it-trigger"}}

cells["A"]=cell{floor="fl-leaves",stone={face="st-door", attr={name="doorA", type="h"}}}
cells["B"]=cell{floor="fl-leaves",stone={face="st-door", attr={name="doorB", type="h"}}}
cells["C"]=cell{floor="fl-leaves",stone={face="st-door", attr={name="doorC", type="h"}}}
cells["D"]=cell{floor="fl-leaves",stone={face="st-door", attr={name="doorD", type="h"}}}
cells["E"]=cell{floor="fl-leaves",stone={face="st-door", attr={name="doorE", type="h"}}}
cells["F"]=cell{floor="fl-leaves",stone={face="st-door", attr={name="doorF", type="h"}}}
cells["G"]=cell{floor="fl-leaves",stone={face="st-door", attr={name="doorG", type="h"}}}
cells["H"]=cell{floor="fl-leaves",stone={face="st-door", attr={name="doorH", type="v"}}}
cells["I"]=cell{floor="fl-leaves",stone={face="st-door", attr={name="doorI", type="h"}}}

cells["1"]=cell{floor="fl-leaves",item={face="it-trigger", attr={action="openclose", target="doorA"}}}
cells["2"]=cell{floor="fl-leaves",item={face="it-trigger", attr={action="openclose", target="doorB"}}}
cells["3"]=cell{floor="fl-leaves",item={face="it-trigger", attr={action="openclose", target="doorC"}}}
cells["4"]=cell{floor="fl-leaves",item={face="it-trigger", attr={action="openclose", target="doorD"}}}
cells["5"]=cell{floor="fl-leaves",item={face="it-trigger", attr={action="openclose", target="doorE"}}}
cells["6"]=cell{floor="fl-leaves",item={face="it-trigger", attr={action="openclose", target="doorF"}}}
cells["7"]=cell{floor="fl-leaves",item={face="it-trigger", attr={action="openclose", target="doorG"}}}
cells["8"]=cell{floor="fl-leaves",item={face="it-trigger", attr={action="openclose", target="doorH"}}}
cells["9"]=cell{floor="fl-leaves",item={face="it-trigger", attr={action="openclose", target="doorI"}}}

cells["0"]=oxyd
level = {
   
"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr",
"r..................r.xSSSSSSSSSSSSSS..r",
"r..........<<......r..SSSSSSS...SSSSx.r",
"r.........<<<......rxWSSSSSSS.S..SSS..r",
"r........<<<<......rx.SSSSSSS.SS.SSSx.r",
"r........<<<<......r.xSSSSSSS.SS.SSS.xr",
"r.......<<<<<......rx.SSSSSSS.S..SSS..r",
"r.......<<O<.......rx.SSSSSSS...SSSSxxr",
"r........TTT.......r..SSSSS...SSSSSS..r",
"r........<<<.......r.xSSSS..S.S..SSS..r",
"r........<<<.......rx.SSSS.SS.SS.SSS.xr",
"r........<V<.......rx.SSSS..S.S..SSS..r",
"r........<<<.......r..SSSSS.....SSSSxxr",
"r........<V<.......r..SSSSSSS.SSSSSS..r",
"r........<<<.......r..SSSSSSS.SSSSSS..r",
"r........<V<.......r..SSSSSWS.SSSSSSS.r",
"r........<<<.......r..SSSSS...SSSSSSS.r",
"r........<V<.......r..SSSSSSSSSSSSSSS.r",
"r....>...<<<.......r..................r",
"r...>>...<<<.......r......aaaa9a......r",
"r..>>>...V<V...>...r....+.a567a8......r",
"r..>>>>..<<<..>>>..r......12aa34......r",
"r...>>>>><<<>>>->>.r......aaaaaa......r",
"r...>>>>-------->>.r..................r",
"r....>>>------->>..r...rrrrrrr.rrrr...r",
"r....>>>-0P0--->...r......!..r...0r...r",
"r....>>>-Z-Z--->...r...rErrrFrrr......r",
"r...>>>--0P0--->>..r...r.&.r...rrrr...r",
"r...>>>--Z-Z--->>..r...r.r.r.0...Wr...r",
"r..>>>>--0P0---->>.r...rAr.....rrBr...r",
"r..>>>>>--------->.r...r.rrrrrrrr.r...r",
"r..>>>>>>TTT>>>>->.r...rDr...H..rIr...r",
"r...>>>>>>>>>>>>>>.r...r.rGrrrr.r.r...r",
"r....>>>>>>>>>>>...r...r0r.r0.r0r0r...r",
"r......>>>>>>>...SSw...rrrCrr.rrrrr..xr",
"r................SS...................r",
"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor = "c"
set_default_parent(abyss)
create_world_by_map(level)
oxyd_shuffle()


























