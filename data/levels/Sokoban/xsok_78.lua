-- A sokoban clone for Enigma
-- Name    : Sokoban 78
-- Filename: xsok_78.lua
--
-- Taken from Sokoban
-- Copyright: Thinking Rabbit
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/lib/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!!!!!!!###########!",
  "!!!!!!!#         #!",
  "!!!!!!!#    o o  #!",
  "######!# o ##### #!",
  "#    ##### o  ##o#!",
  "#       o o      #!",
  "#          ## ## #!",
  "#    ##@##### ## #!",
  "#    ####!!!# ## ##",
  "#....#!!!!!!# o   #",
  "#....#!!!!!!#     #",
  "######!!!!!!#######",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,777)

