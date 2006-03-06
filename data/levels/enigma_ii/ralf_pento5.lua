-- A level for Enigma
-- Name:        Pentomino V
-- Filename:    ralf_pento5.lua
-- Copyright: 	(C) Dec 2004 Ralf Westram
-- Contact: 	amgine@reallysoft.de
-- License: 	GPL v2.0 or above

if dofile(enigma.FindDataFile("levels/lib/ralf_pento.lua")) == nil then error("can't load 'ralf_pento.lua'"); end

level={
   "##o###o#2##1##o##o##3###",
   "#                      #",
   "o RRRRR                o",
   "# RRRRR                #",
   "S RRRRR    a d         o",
   "# RRRRR                #",
   "o RRRRR                o",
   "o                      #",
   "#                      #",
   "# @@@@@@@@@@@@@@@@@@@@ #",
   "oH@@@@@@@@@@@@@@@@@@@@ o",
   "# @@@@@@@@@@@@@@@@@@@@ #",
   "#######################T",
}

areafloor="fl-leaves"
normalfloor="fl-samba"
rotfloor="fl-bluegreen"
wallstone="st-blue-sand"

cells=pentocells("And last not least: 3*20 = 60")
rs_create_world(level,cells)
early_check()
display.SetFollowMode(display.FOLLOW_SMOOTH)



