-- A sokoban clone for Enigma
-- Name    : Microban 113
-- Filename: mic_113.lua
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
  "!!!#####!!!!!!",
  "!!##   #!!!!!!",
  "###  # #!!!!!!",
  "#    . #!!!!!!",
  "#  ##_#####!!!",
  "#  ._._#__##!!",
  "#  #_a_O___###",
  "#####._#__O__#",
  "!!!!####__O__#",
  "!!!!!!!##_O_##",
  "!!!!!!!!#__##!",
  "!!!!!!!!#__#!!",
  "!!!!!!!!####!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,915)

