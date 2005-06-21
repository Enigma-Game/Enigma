-- WB 8: Tough Skyscraper
-- (c) 2003 Jon Sneyers
-- Licensed under GPL v2.0 or above
-- 2003-12-20

dofile(enigma.FindDataFile("levels/ant.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells[" "]=cell{floor="fl-tigris"}
cells[";"]=cell{floor="fl-samba"}
cells[":"]=cell{floor="fl-stone"}
cells["w"]=cell{stone="st-wood"}
cells["B"]=cell{stone="st-stone_break"}
SetDefaultAttribs("st-rubberband", {strength=40})
cells["R"]=cell{stone="st-rubberband"}
cells["i"]=cell{stone="st-stoneimpulse"}

cells["="]=cell{stone="st-rock5"}
cells["#"]=cell{stone="st-rock6"}
cells["."]=cell{floor="fl-abyss"}

cells["h"]=cell{item="it-hammer"}

cells["H"]=cell{stone="st-chameleon"}

cells["%"]=cell{stone="st-scissors"}

cells["V"]=cell{parent=cells[" "], stone="st-bolder-s"}
cells[">"]=cell{parent=cells[" "], stone="st-bolder-e"}
cells["<"]=cell{parent=cells[" "], stone="st-bolder-w"}
cells["^"]=cell{parent=cells[" "], stone="st-bolder-n"}

cells["t"]=cell{stone={face="st-door", attr={name="doorA", type="h"}}}
cells["T"]=cell{item= {face="it-trigger", attr={action="open", target="doorA"}}}

cells["l"]=cell{{{oneway, SOUTH}}}


cells["O"]=cell{actor={face="ac-blackball", attr={player=0}}}
cells["0"]=oxyd

if difficult == false then
    cells["D"]=cells["#"]
else
    cells["D"]=cell{stone="st-death"}
end


level = {      
   "####################",
   "#                  #",
   "#                  #",
   "#   #############w##",
   "#   #              #",
   "#   #              #",
   "#   #  #############",
   "#   #             T#",
   "#   #######l########",
   "#   #iBBBBB        #",
   "#   #iBBBBB####### #",
   "#   #iBBDBB        #",
   "#   #i0BDBB ########",
   "#   #iBBDBB        #",
   "#   #iBBDBB        #",
   "#   #i0BDBB####### #",
   "#   #iBBDBB        #",
   "#  ##iBBBBB        #",
   "#   #iBBBBB ########",
   "#   #i             #",
   "#   #i             #",
   "#   #i ########### #",
   "#   #i #RRRRRRRRR# #",
   "#   #i #R       R# #",
   "#   #i #R       R# D",
   "#   #i #R  #0#  R# #",
   "#   #i #R  D D  R# #",
   "#   #i #R  D D  RD #",
   "#   #i #R  D D  R# #",
   "#   #i #R  D D  R# #",
   "#   #i #R  D D  R# D",
   "##  #i #         # #",
   "#   #i             #",
   "#   #i             #",
   "#   #i  %%%%%%%%%  #",
   "#   #i             #",
   "#   #i             #",
   "#   #i       DDDD  #",
   "#   #i             #",
   "#   #i             #",
   "#   #i             #",
   "#   #i   DDDDD     #",
   "#   #i             #",
   "#   #i             #",
   "#   #i             #",
   "#  ##i       DDDD  #",
   "#   #i             #",
   "#   #i             #",
   "#   #i  DDDDD      #",
   "#   #i             #",
   "#   #i             #",
   "#   #i             #",
   "#   #i   D###D###D##",
   "#   #w             #",
   "#   #             h#",
   "#   #t##0#DRD#0D0###",
   "#   #              #",
   "#   #            R #",
   "#   %            R #",
   "# O ##0D0D0D0###0R #",
   "##################0#"
}
--  01234567890123456789

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor = "c"
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()
enigma.AddConstantForce(0,50)
display.SetFollowMode(display.FOLLOW_SCROLLING)
