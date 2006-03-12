-- A sokoban clone for Enigma
-- Name    : Sokoban 26
-- Filename: xsok_26.lua
--
-- Taken from Sokoban
-- Copyright: Thinking Rabbit
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/lib/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!#######!!!!!!!",
  "!#  #  #####!!!",
  "##  #  #...###!",
  "#  o#  #...__#!",
  "# o #oo ...__#!",
  "#  o#  #..._.#!",
  "#   # o########",
  "##o       o o #",
  "##  #  oo #   #",
  "!######  ##oo@#",
  "!!!!!!#      ##",
  "!!!!!!########!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,770)

