-- A sokoban clone for Enigma
-- Name    : Sokoban 34
-- Filename: xsok_34.lua
--
-- Taken from Sokoban
-- Copyright: Thinking Rabbit
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/lib/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!!!####!!!!!!!!",
  "####  #######!!",
  "#....  o    #!!",
  "#.#.. # #   ###",
  "#.... #  # oo #",
  "#.#.. #o #    #",
  "#  ###  o###o##",
  "#    o o  #  #!",
  "# o#  o o    #!",
  "##  ## o ##o #!",
  "!##@ # #### ##!",
  "!!## o  #   #!!",
  "!!!##     ###!!",
  "!!!!#######!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,769)

