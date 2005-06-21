-- WB 1: Hurry!
-- (c) 2003 Jon Sneyers
-- Licensed under GPL v2.0 or above
-- 2003-12-20

dofile(enigma.FindDataFile("levels/ant.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells[" "]=cell{{{randomfloor, cell{floor="fl-tigris"}, 3, cell{floor="fl-samba"}, 1, cell{floor="fl-stone"}, 20}}}
cells["w"]=cell{stone="st-wood"}
cells["i"]=cell{stone="st-stoneimpulse"}
cells["X"]=cell{stone="st-stoneimpulse-hollow"}

cells["="]=cell{stone="st-rock5"}
cells["#"]=cell{stone="st-rock6"}
cells["."]=cell{floor="fl-abyss"}

cells["&"]=cell{{{oneway, EAST}}}
cells["%"]=cell{{{oneway, WEST}}}

cells["V"]=cell{parent=cells[" "], stone="st-bolder-s"}
cells[">"]=cell{parent=cells[" "], stone="st-bolder-e"}
cells["<"]=cell{parent=cells[" "], stone="st-bolder-w"}
cells["^"]=cell{parent=cells[" "], stone="st-bolder-n"}

cells["a"]=cell{stone={face="st-door", attr={name="doorA", type="v"}}}
cells["A"]=cell{item= {face="it-trigger", attr={action="close", target="doorA"}}}
cells["B"]=cell{item= {face="it-trigger", attr={action="open", target="doorA"}}}
cells["O"]=cell{actor={face="ac-blackball", attr={player=0}}}
cells["0"]=oxyd

level = {
   "#iii###0#0#0#0###00#",
   "#i#ii &        a   0",
   "#i##i B#0#0#0####00#",
   "#iO&Xw#iiiiiiiiiiiii",
   "#i###iii######### Vi",
   "#iiiii### Vi# Vi# ##",
   "##### Vi# # # # # # ",
   "#iii  #             ",
   "#i# A               ",
   "#>#               #<",
   "#                  i",
   "# # # # # # # # # # ",
   "### ^i# ^i# ^i# ^i# ",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor = "c"
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()
