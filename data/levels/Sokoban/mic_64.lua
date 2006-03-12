-- A sokoban clone for Enigma
-- Name    : Microban 64
-- Filename: mic_64.lua
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
  "!######!!!",
  "##    #!!!",
  "#   o #!!!",
  "#  oo #!!!",
  "### .#####",
  "!!##.#_a_#",
  "!!!#.__O_#",
  "!!!#._####",
  "!!!####!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

improve=1
play_sokoban(level,872)

