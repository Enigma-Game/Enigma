-- WB 5: Block its way!
-- (c) 2003 Jon Sneyers
-- Licensed under GPL v2.0 or above
-- 2003-12-20

dofile(enigma.FindDataFile("levels/lib/ant.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells[";"]=cell{floor="fl-tigris"}
cells[":"]=cell{floor="fl-samba"}
cells["/"]=cell{floor="fl-stone"}
cells[" "]=cell{floor="fl-leaves"}
cells["%"]=cell{floor="fl-sand"}
cells["w"]=cell{stone="st-wood"}
cells["W"]=cell{parent=cells[":"], stone={"st-wood"}}
cells["i"]=cell{stone="st-stoneimpulse"}
cells["I"]=cell{stone="st-stoneimpulse-hollow"}

cells["#"]=cell{stone="st-rock6"}
cells["="]=cell{stone="st-rock1"}
cells["R"]=cell{stone="st-rock8"}
cells["B"]=cell{parent=cells[":"],stone="st-black4"}
cells["b"]=cell{parent=cells[":"],stone="st-white4"}
cells["@"]=cell{floor="fl-abyss"}
cells["."]=cell{floor="fl-water"}

cells["H"]=cell{stone="st-chameleon"}

cells["&"]=cell{{{oneway, EAST}}}
cells["l"]=cell{{{oneway, SOUTH}}}
cells["j"]=cell{{{oneway, NORTH}}}

cells["V"]=cell{parent=cells[" "], stone="st-bolder-s"}
cells[">"]=cell{parent=cells[" "], stone="st-bolder-e"}
cells["<"]=cell{parent=cells[" "], stone="st-bolder-w"}
cells["^"]=cell{parent=cells[" "], stone="st-bolder-n"}

cells["t"]=cell{stone={face="st-door", attr={name="doorA", type="v"}}}
cells["T"]=cell{item= {face="it-trigger", attr={action="close", target="doorA"}}}
cells["U"]=cell{item= {face="it-trigger", attr={action="open", target="doorA"}}}
cells["O"]=cell{actor={face="ac-blackball", attr={player=0}}}
cells["o"]=cell{actor={face="ac-whiteball", attr={player=1}}}
cells["0"]=oxyd
cells["y"]=cell{item= {face="it-yinyang"}}
cells["1"]=cell{{{document, "Don't forget to open the door..."}}}
cells["2"]=cell{{{document, "Good Luck!"}}}
cells["3"]=cell{{{document, "Hurry!"}}}
cells["4"]=cell{{{document, "Congratulations! (or try again)"}}}


level = {      
--  012345678901234567890123456789012345678
   "...................====================",
   ".iiiiiiiiiiiiiiii#iV:=::::::::::::::::=",
   ".iU      #######i#i#:=::===========:W:=",
   ".i## O 1 & 2   ii#ii:=::=:::::::::=:::=",
   ".i0#     #####.wI0#i:=::=:::====:==:::=",
   ".i4iiiiiiIIIIIIi##ii:=B==:::=0:=...:::=",
   ".ijI##wwww########i#:=::::::.=:=:==:W:=",
   ".i II#....#IIIIIIii#:=======:=b=:=::::=",
   ".i wiw....wI     w##::B:::::W=:::=::W:=",
   ".i0 i# ww  IIII 3  .::======.=====::===",
   ".i0y t II     I  w#:::=0::::.=0::::==:=",
   ".iiiiiiiiiiiiiiiii##T=======.=======::=",
   "...................===:::::WWW::&:::::0",  -- _________________________
   "...........RRR%%....=:::::::::::==l====",
   "..%%%%%%%..R w %..;;;:::::::::::::::::=",
   "..%  w w%..%  w%R...===W============BB=",
   "..% wow %..%%%%RR...=::W::::::::::::::=",
   "..%  w  %.....RR....=::===========::::=",
   "..%%%%%%%...........=bb=:::::::::==W===",
   "....................=:::::=====:::::::=",
   ".%%RRRRR..%RRRRRRR%.=======0=0=======B=",
   ".% y  0R..%      0%.=:::::::::::::::::=",
   ".RRR  RR..%w  w w %.=WWWWWWWWWWWWWWWWW=",
   "...RR%%...%%%%%%%%%.=:::::::::::::::::=",
   "....................==0==0=======0==0==",
--  012345678901234567890123456789012345678
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()
enigma.ConserveLevel = FALSE
oxyd_default_flavor = "d"

document(21,21,"Almost finished...")

















