-- A sokoban clone for Enigma
-- Name    : Sokoban 24
-- Filename: xsok_24.lua
--
-- Taken from Sokoban
-- Copyright: Thinking Rabbit
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "###############!!!!",
  "#..........  .####!",
  "#..........oo.#  #!",
  "###########o #   ##",
  "#      o  o     o #",
  "## ####   #  o #  #",
  "#      #   ##  # ##",
  "#  o#  # ##  ### ##",
  "# o #o###    ### ##",
  "###  o #  #  ### ##",
  "###    o ## #  # ##",
  "!# o  #  o  o o   #",
  "!#  o  o#ooo  #   #",
  "!#  #  o      #####",
  "!# @##  #  #  #!!!!",
  "!##############!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,768)

