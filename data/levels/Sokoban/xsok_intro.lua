-- A sokoban clone for Enigma
-- Name:     Sokoban 1
-- Filename: xsok_01.lua
--
-- (C) 2003 Ralf Westram (amgine@reallysoft.de)
-- GPL 2.0 or above


dofile(enigma.FindDataFile("levels/lib/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "#####!!!!",
  "#   ####!",
  "#    o ##",
  "## oo#  #",
  "!##   @.#",
  "!!####..#",
  "!!!!!####",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,0)

document(xlo+1,ylo+1,
         "Welcome to the world of Sokoban.          "..
         "The following levels were converted from various sokoban level packages with the friendly permission of their authors.          "..
         "All have been play-tested, so - whether you believe or not - they are always solvable."
      )
