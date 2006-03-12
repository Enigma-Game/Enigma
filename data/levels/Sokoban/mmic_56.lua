-- A sokoban clone for Enigma
-- Name    : Mas Microban 56
-- Filename: mmic_56.lua
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
  "!!!#####",
  "!!##   #",
  "### .# #",
  "#  o*o@#",
  "#   .# #",
  "### #  #",
  "!!#   ##",
  "!!#####!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

swapStyle = 2
play_sokoban(level,1194)

