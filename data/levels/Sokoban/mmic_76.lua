-- A sokoban clone for Enigma
-- Name    : Mas Microban 76
-- Filename: mmic_76.lua
--
-- Taken from Mas Microban
-- Copyright: David W Skinner
-- Email    : sasquatch@bentonrea.com
-- Website  : http://users.bentonrea.com/~sasquatch/sokoban/
-- With friendly permission from David W Skinner.
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!######!!",
  "!#  . ###",
  "## o.   #",
  "#  o*oo #",
  "#..*@*..#",
  "# oo*o  #",
  "#   .o ##",
  "### .  #!",
  "!!######!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

psymm = 1
play_sokoban(level,1196)

