-- A sokoban clone for Enigma
-- Name    : Sokoban 18
-- Filename: xsok_18.lua
--
-- Taken from Sokoban
-- Copyright: Thinking Rabbit
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!!!!!###########!!!",
  "!!!!!#__.  #   #!!!",
  "!!!!!#_#.    @ #!!!",
  "!#####_##..# ####!!",
  "##  #_..###     ###",
  "# o #...   o #  o #",
  "#    .. ##  ## ## #",
  "####o##o# o #   # #",
  "!!## #    #o oo # #",
  "!!#  o # #  # o## #",
  "!!#               #",
  "!!#  ###########  #",
  "!!####!!!!!!!!!####",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,771)

