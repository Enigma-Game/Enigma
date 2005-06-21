-- A sokoban clone for Enigma
-- Name    : Sokoban 47
-- Filename: xsok_47.lua
--
-- Taken from Sokoban
-- Copyright: Thinking Rabbit
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!!!!!!!!!####!!!!!",
  "!#########  ##!!!!",
  "##  o      o #####",
  "#   ## ##   ##...#",
  "# #oo o oo#o##...#",
  "# #   @   #   ...#",
  "#  o# ###oo   ...#",
  "# o  oo  o ##....#",
  "###o       #######",
  "!!#  #######!!!!!!",
  "!!####!!!!!!!!!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,773)

