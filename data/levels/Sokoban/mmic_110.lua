-- A sokoban clone for Enigma
-- Name    : Mas Microban 110
-- Filename: mmic_110.lua
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
  "!!####!!######!!",
  "!!#  #!!#    ##!",
  "### @####     #!",
  "#_.   ## o #  #!",
  "#_.   # o  # ###",
  "##.# # o  #    #",
  "#     o  #     #",
  "# .###  ##   ###",
  "#  #!#####  ##!!",
  "####!!!!!####!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,1233)

