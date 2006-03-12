-- A sokoban clone for Enigma
-- Name    : Microban 58
-- Filename: mic_58.lua
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
  "####!!!",
  "#__####",
  "#.*o  #",
  "#_.o# #",
  "## @  #",
  "!#   ##",
  "!#####!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,875)

document(xlo+1,ylo+1,"Excerpt from the 'Microban' set by David W Skinner.")

