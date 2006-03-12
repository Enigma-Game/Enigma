-- A sokoban clone for Enigma
-- Name    : Sokoban 12
-- Filename: xsok_12.lua
--
-- Taken from Sokoban
-- Copyright: Thinking Rabbit
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/lib/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "################!",
  "#              #!",
  "# # ######     #!",
  "# #  o o o o#  #!",
  "# #   o@o   ## ##",
  "# #  o o o###...#",
  "# #   o o  ##...#",
  "# ###ooo o ##...#",
  "#     # ## ##...#",
  "#####   ## ##...#",
  "!!!!#####     ###",
  "!!!!!!!!#     #!!",
  "!!!!!!!!#######!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,765)

