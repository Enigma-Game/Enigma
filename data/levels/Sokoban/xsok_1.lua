-- A sokoban clone for Enigma
-- Name    : Sokoban 1
-- Filename: xsok_1.lua
--
-- Taken from Sokoban
-- Copyright: Thinking Rabbit
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/lib/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!!!!#####!!!!!!!!!!",
  "!!!!#   #!!!!!!!!!!",
  "!!!!#o  #!!!!!!!!!!",
  "!!###  o##!!!!!!!!!",
  "!!#  o o #!!!!!!!!!",
  "### # ## #!!!######",
  "#   # ## #####  ..#",
  "# o  o          ..#",
  "##### ### #@##  ..#",
  "!!!!#     #########",
  "!!!!#######!!!!!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

swapStyle = 2
play_sokoban(level,715)

document(xlo+1,ylo+7,"The levels named 'Sokoban' are taken from the original sokoban game.")

