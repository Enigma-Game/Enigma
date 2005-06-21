-- A level for Enigma
-- Name:        Pentomino IV
-- Filename:    ralf_pento4.lua
-- Copyright: 	(C) Dec 2004 Ralf Westram
-- Contact: 	amgine@reallysoft.de
-- License: 	GPL v2.0 or above

if dofile(enigma.FindDataFile("levels/ralf_pento.lua")) == nil then error("can't load 'ralf_pento.lua'"); end

level={
   "##o###o#2##1##o##o##",
   "#                 H#",
   "o RRRRR            o",
   "# RRRRR            #",
   "S RRRRR    a d     o",
   "# RRRRR            #",
   "o RRRRR            o",
   "o                  #",
   "#   @@@@@@@@@@@@@@@#",
   "3   @@@@@@@@@@@@@@@#",
   "o   @@@@@@@@@@@@@@@#",
   "#   @@@@@@@@@@@@@@@#",
   "##o################T",
}

areafloor="fl-leaves"
normalfloor="fl-samba"
rotfloor="fl-bluegreen"
wallstone="st-blue-sand"

cells=pentocells("4*15 = 60")
rs_create_world(level,cells)
early_check()
