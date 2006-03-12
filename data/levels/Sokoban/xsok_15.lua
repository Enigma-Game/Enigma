-- A sokoban clone for Enigma
-- Name    : Sokoban 15
-- Filename: xsok_15.lua
--
-- Taken from Sokoban
-- Copyright: Thinking Rabbit
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/lib/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!!!########!!!!!!",
  "!!!#   #  #!!!!!!",
  "!!!#  o   #!!!!!!",
  "!### #o   ####!!!",
  "!#  o  ##o   #!!!",
  "!#  # @ o # o#!!!",
  "!#  #      o ####",
  "!## ####o##     #",
  "!# o#.....# #   #",
  "!#  o..**. o# ###",
  "##  #.....#   #!!",
  "#   ### #######!!",
  "# oo  #  #!!!!!!!",
  "#  #     #!!!!!!!",
  "######   #!!!!!!!",
  "!!!!!#####!!!!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

reverse_scrolling=1
play_sokoban(level,768)

