-- A sokoban clone for Enigma
-- Name    : Sokoban 7
-- Filename: xsok_7.lua
--
-- Taken from Sokoban
-- Copyright: Thinking Rabbit
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!!!!!!!#####!",
  "!#######   ##",
  "## # @## oo #",
  "#    o      #",
  "#  o  ###   #",
  "### #####o###",
  "# o  ### ..#!",
  "# o o o ...#!",
  "#    ###...#!",
  "# oo #!#...#!",
  "#  ###!#####!",
  "####!!!!!!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,721)

