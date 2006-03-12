-- A sokoban clone for Enigma
-- Name    : Microban 145
-- Filename: mic_145.lua
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
  "!!!#####!!!",
  "!!!# @ #!!!",
  "!!##   ##!!",
  "###.ooo.###",
  "#  o...o  #",
  "#  o.#.o  #",
  "#  o...o  #",
  "###.ooo.###",
  "!!##   ##!!",
  "!!!#   #!!!",
  "!!!#####!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

xsymm = 1
ysymm = 1
psymm = 1
play_sokoban(level,920)

