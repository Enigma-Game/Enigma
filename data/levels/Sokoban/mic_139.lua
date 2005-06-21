-- A sokoban clone for Enigma
-- Name    : Microban 139
-- Filename: mic_139.lua
--
-- Taken from Microban
-- Copyright: David W Skinner
-- Email    : sasquatch@bentonrea.com
-- Website  : http://users.bentonrea.com/~sasquatch/sokoban/
-- With friendly permission from David W Skinner.
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!####!!!!!!!!!!!",
  "##  ####!!!!!!!!",
  "#   ...#!!!!!!!!",
  "#   ...#!!!!!!!!",
  "#   # ##!!!!!!!!",
  "#   #@ ####!####",
  "##### o   ###  #",
  "!!!!#  ##o o   #",
  "!!!###     oo  #",
  "!!!# o  ##   ###",
  "!!!#    ######!!",
  "!!!######!!!!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

shogunLaser = 1
play_sokoban(level,923)

