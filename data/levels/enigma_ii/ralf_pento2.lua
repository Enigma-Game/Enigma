-- A level for Enigma
-- Name:        Pentomino II
-- Filename:    ralf_pento2.lua
-- Copyright: 	(C) Dec 2004 Ralf Westram
-- Contact: 	amgine@reallysoft.de
-- License: 	GPL v2.0 or above

if dofile(enigma.FindDataFile("levels/lib/ralf_pento.lua")) == nil then error("can't load 'ralf_pento.lua'"); end

level={
   "##o###o#2##1#####o##",
   "#                 H#",
   "o RRRRR            o",
   "# RRRRR            #",
   "S RRRRR            #",
   "# RRRRR @@@@@@@@@@ o",
   "o RRRRR @@@@@@@@@@ #",
   "#       @@@@@@@@@@ #",
   "#       @@@@@@@@@@ #",
   "3  a d  @@@@@@@@@@ #",
   "o       @@@@@@@@@@ o",
   "#                  #",
   "##o#####o########o#T",
}

areafloor="fl-leaves"
normalfloor="fl-samba"
rotfloor="fl-bluegreen"
wallstone="st-blue-sand"

cells=pentocells("5*12 = 6*10")
rs_create_world(level,cells)
early_check()



