-- A sokoban clone for Enigma
-- Name    : Mas Microban 124
-- Filename: mmic_124.lua
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
  "#########",
  "#___#___#",
  "#_O_*_O_#",
  "#..#_#..#",
  "# o # o #",
  "# .o@o. #",
  "# o # o #",
  "#..#_#..#",
  "#_O_*_O_#",
  "#___#___#",
  "#########",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

xsymm = 1
ysymm = 1
boxlikewall=1
play_sokoban(level,1238)

