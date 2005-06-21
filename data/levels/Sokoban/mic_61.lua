-- A sokoban clone for Enigma
-- Name    : Microban 61
-- Filename: mic_61.lua
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
  "########!",
  "#      #!",
  "# #### #!",
  "# #...@#!",
  "# ###o###",
  "# #     #",
  "#  oo o #",
  "####   ##",
  "!!!#.###!",
  "!!!###!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,869)

document(xlo+1,ylo+5,"Some triggers refuse to work, if not ALL stones are in the right position.")

