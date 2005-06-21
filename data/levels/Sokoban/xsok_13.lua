-- A sokoban clone for Enigma
-- Name    : Sokoban 13
-- Filename: xsok_13.lua
--
-- Taken from Sokoban
-- Copyright: Thinking Rabbit
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!!!#########!!!!!!!",
  "!!##   ##  #####!!!",
  "###     #  #    ###",
  "#  o #o #  #  ..._#",
  "# # o#@o## # #.#._#",
  "#  # #o  #    ._._#",
  "# o    o # # #.#._#",
  "#   ##  ##o o ._._#",
  "# o #   #  #o#.#._#",
  "## o  o   o  o..._#",
  "!#o ######    ##__#",
  "!#  #!!!!##########",
  "!####!!!!!!!!!!!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,766)

