-- A sokoban clone for Enigma
-- Name    : Microban 138
-- Filename: mic_138.lua
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
  "!!####!!!!!!",
  "###  #!!!!!!",
  "#    ###!!!!",
  "# # . .#!!!!",
  "# @ ...####!",
  "# # # #   ##",
  "#   # oo   #",
  "#####  o o #",
  "!!!!##o # ##",
  "!!!!!#    #!",
  "!!!!!######!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

shogunLaser = 1
play_sokoban(level,922)

