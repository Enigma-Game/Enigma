-- A sokoban clone for Enigma
-- Name    : Mas Microban 107
-- Filename: mmic_107.lua
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
  "!!####!!!",
  "!##  ####",
  "!#  oo  #",
  "!# #  # #",
  "!# o o# #",
  "!# #  # #",
  "!#@#  # #",
  "## #o#  #",
  "#_.....##",
  "#___####!",
  "#####!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,1239)

