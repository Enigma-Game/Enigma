-- A sokoban clone for Enigma
-- Name    : Sokoban 82
-- Filename: xsok_82.lua
--
-- Taken from Sokoban
-- Copyright: Thinking Rabbit
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!!!!######!!!!!!!",
  "!####    #!!!!!!!",
  "!#    ## #!!!!!!!",
  "!# o     #!!!!!!!",
  "### #### ########",
  "#  o   o ##  ...#",
  "#   oo oo    ...#",
  "#    o  o##  ...#",
  "##@## ## ##  ...#",
  "!###  o  ########",
  "!#   oo  #!!!!!!!",
  "!#    #  #!!!!!!!",
  "!#########!!!!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,772)

