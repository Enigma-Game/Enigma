-- A sokoban clone for Enigma
-- Name    : Mas Microban 99
-- Filename: mmic_99.lua
--
-- Taken from Mas Microban
-- Copyright: David W Skinner
-- Email    : sasquatch@bentonrea.com
-- Website  : http://users.bentonrea.com/~sasquatch/sokoban/
-- With friendly permission from David W Skinner.
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/lib/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!!#####!!!!!",
  "!!#   #!!!!!",
  "!!#   #!!!!!",
  "!!#  ###!!!!",
  "###. #x#####",
  "#__. ###___#",
  "#__. #_O___#",
  "#_#.##_O_O_#",
  "#__._a__OO##",
  "##__###___#!",
  "!####!#####!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,1201)

