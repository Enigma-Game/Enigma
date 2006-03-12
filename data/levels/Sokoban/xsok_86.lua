-- A sokoban clone for Enigma
-- Name    : Sokoban 86
-- Filename: xsok_86.lua
--
-- Taken from Sokoban
-- Copyright: Thinking Rabbit
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/lib/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!!!!!###########!!!",
  "!!!!!#__.  #   #!!!",
  "!!!!!#_#.    @ #!!!",
  "!#####_##..# ####!!",
  "##  #...###     ###",
  "# o #...   o #  o #",
  "#    .. ##  ## ## #",
  "####o##o# o #   # #",
  "!!## #    #o oo # #",
  "!!#  o #o#  # o## #",
  "!!#               #",
  "!!#  ###########  #",
  "!!####!!!!!!!!!####",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

swapStyle = 2
play_sokoban(level,776)

document(xlo+14,ylo+10,"Same like Sokoban 18 with one extra box by Michael Bischoff")

