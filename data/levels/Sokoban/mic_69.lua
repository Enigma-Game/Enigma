-- A sokoban clone for Enigma
-- Name    : Microban 69
-- Filename: mic_69.lua
--
-- Taken from Microban
-- Copyright: David W Skinner
-- Email    : sasquatch@bentonrea.com
-- Website  : http://users.bentonrea.com/~sasquatch/sokoban/
-- With friendly permission from David W Skinner.
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/lib/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "####!!####!",
  "#  ####__#!",
  "#  #__#__#!",
  "#  #____O##",
  "#  ._.#O__#",
  "#@ ##_#_O_#",
  "#   ._#___#",
  "###########",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,877)

