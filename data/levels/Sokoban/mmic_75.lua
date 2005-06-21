-- A sokoban clone for Enigma
-- Name    : Mas Microban 75
-- Filename: mmic_75.lua
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
  "#########",
  "#   #   #",
  "# .o.o. #",
  "# o.o.o #",
  "##.o@o.##",
  "# o.o.o #",
  "# .o.o. #",
  "#   #   #",
  "#########",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

xsymm = 1
ysymm = 1
psymm = 1
play_sokoban(level,1195)

