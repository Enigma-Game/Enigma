-- A sokoban clone for Enigma
-- Name    : Sokoban 46
-- Filename: xsok_46.lua
--
-- Taken from Sokoban
-- Copyright: Thinking Rabbit
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "######!!!!!!!!!!",
  "#    #!!!!!!!!!!",
  "# ## ###!!!#####",
  "# #  o ##### ..#",
  "# #o   o     ..#",
  "# #   ## ##  ..#",
  "# #o o# o ## ..#",
  "# #  o     # ..#",
  "#  o### o  # ..#",
  "### o  o o # ..#",
  "!#    o # ## ###",
  "!# o ##@#    #!!",
  "!##  #########!!",
  "!!####!!!!!!!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,772)

