-- A sokoban clone for Enigma
-- Name    : Mas Sasquatch 12
-- Filename: msas_12.lua
--
-- Taken from Mas Sasquatch
-- Copyright: David W Skinner
-- Email    : sasquatch@bentonrea.com
-- Website  : http://users.bentonrea.com/~sasquatch/sokoban/
-- Released : August 1999
-- With friendly permission from David W Skinner.
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "#!!###!!!!!",
  "!##   ##!!!",
  "!#*.o  #!!!",
  "# .o.o .##!",
  "# o.o.o  #!",
  "#  o.@.o  #",
  "!#  o.o.o #",
  "!##. o.o. #",
  "!!!#  o.*#!",
  "!!!##   ##!",
  "!!!!!###!!#",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

shogunLaser = 1
play_sokoban(level,1316)

