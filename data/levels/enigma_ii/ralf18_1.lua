-- A level for Enigma
-- Name:        Pentomino III
-- Filename:    ralf_pento3.lua
-- Copyright: 	(C) Dec 2004 Ralf Westram
-- Contact: 	amgine@reallysoft.de
-- License: 	GPL v2.0 or above

Require("levels/lib/ralf_pento.lua")

level={
   "##o###o#2##1#####o##",
   "#                 H#",
   "o RRRRR            o",
   "# RRRRR            #",
   "S RRRRR            o",
   "# RRRRR            #",
   "o RRRRR            o",
   "o      @@@@@@@@@@@@#",
   "#      @@@@@@@@@@@@#",
   "3  a d @@@@@@@@@@@@#",
   "o      @@@@@@@@@@@@#",
   "#      @@@@@@@@@@@@#",
   "##o###o############T",
}

areafloor="fl-leaves"
normalfloor="fl-samba"
rotfloor="fl-bluegreen"
wallstone="st-blue-sand"

cells=pentocells("Simply 5*12 ..")
rs_create_world(level,cells)
early_check()




