-- A sokoban clone for Enigma
-- Name    : Mas Microban 47
-- Filename: mmic_47.lua
--
-- Taken from Mas Microban
-- Copyright: David W Skinner
-- Email    : sasquatch@bentonrea.com
-- Website  : http://users.bentonrea.com/~sasquatch/sokoban/
-- With friendly permission from David W Skinner.
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/lib/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "######!!!",
  "#    ###!",
  "# o @  #!",
  "#  ### #!",
  "# oo o #!",
  "#     ###",
  "### ##__#",
  "#_....__#",
  "#_______#",
  "#__######",
  "####!!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,1194)

