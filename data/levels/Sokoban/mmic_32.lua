-- A sokoban clone for Enigma
-- Name    : Mas Microban 32
-- Filename: mmic_32.lua
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
  "########",
  "#      #",
  "#    # #",
  "###..O.#",
  "!!#_#O##",
  "!!#__a_#",
  "!!##_O_#",
  "!!!#_#_#",
  "!!!#___#",
  "!!!#####",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,1188)

