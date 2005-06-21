-- A sokoban clone for Enigma
-- Name    : Sokoban 77
-- Filename: xsok_77.lua
--
-- Taken from Sokoban
-- Copyright: Thinking Rabbit
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "######!!#####!",
  "#    #!!#   #!",
  "# o  #### o #!",
  "# o      o  #!",
  "#  ###@###o #!",
  "########## ###",
  "#..   ##     #",
  "#..   ##o    #",
  "#..   ## o   #",
  "#..   ## o   #",
  "#..     o o  #",
  "###  #########",
  "!!####!!!!!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

swapStyle = 2
play_sokoban(level,776)

