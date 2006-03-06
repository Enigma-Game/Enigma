-- WB 7: Easy Skyscraper
-- (c) 2003 Jon Sneyers
-- Licensed under GPL v2.0 or above
-- 2003-12-20

dofile(enigma.FindDataFile("levels/lib/ant.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells[" "]=cell{floor="fl-tigris"}
cells[";"]=cell{floor="fl-samba"}
cells[":"]=cell{floor="fl-stone"}
cells["w"]=cell{stone="st-wood"}
cells["B"]=cell{stone="st-stone_break"}
SetDefaultAttribs("st-rubberband", {strength=20})
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

cells["O"]=cell{actor={face="ac-blackball", attr={player=0}}}
cells["0"]=oxyd

if difficult == false then
    cells["D"]=cells["#"]
else
    cells["D"]=cell{stone="st-death"}
end


level = {      
   "####################",
   "#h                 #",
   "R##R##R##R##R##R## R",
   "R                  #",
   "R R###R##R##R##R#R##",
   "R R                #",
   "R R  #0#      #0#  #",
   "R R  DDD      DDD  #",
   "R R         R      #",
   "R R    R           #",
   "R R            R   #",
   "#                  #",
   "###############R   #",
   "#    R        D    #",
   "#             D R  #",
   "#  R ######   D    #",
   "#    #0       D  R #",
   "#  R #0       D    #",
   "#    ########## R  #",
   "#   R       R      #",
   "#       R          #",
   "#            R     #",
   "#    R         R   #",
   "#                  #",
   "# R       R      R #",
   "#     R            #",
   "#             R    #",
   "#   R              #",
   "#          R       #",
   "#      R           #",
   "#             R    #",
   "#   R              #",
   "#         %        #",
   "#                  #",
   "#                  #",
   "#  #################",
   "#                  #",
   "##########         #",
   "#             ##BB##",
   "#             DBBB##",
   "#             DBBB##",
   "#        ######BBB##",
   "#        DBBBBBBBB##",
   "#        DBBBBBBBB##",
   "#   #######BB#######",
   "#   DBBBBBBBBBBBBBB#",
   "#   DBB0BB0BB0BB0BB#",
   "# O DBBBBBBBBBBBBBB#",
   "####################"
}
--  01234567890123456789

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor = "c"
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()
enigma.AddConstantForce(0,50)
display.SetFollowMode(display.FOLLOW_SCROLLING)















