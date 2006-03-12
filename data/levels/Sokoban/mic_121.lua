-- A sokoban clone for Enigma
-- Name    : Microban 121
-- Filename: mic_121.lua
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
  "!######!!!!",
  "!#_.__#!!!!",
  "##O.#_#!!!!",
  "#__*__#!!!!",
  "#_..###!!!!",
  "##o #x#####",
  "## ##x#   #",
  "#  #### # #",
  "#   @ o o #",
  "##  #     #",
  "!##########",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,914)

