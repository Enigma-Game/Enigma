-- A sokoban clone for Enigma
-- Name    : Microban 62
-- Filename: mic_62.lua
--
-- Taken from Microban
-- Copyright: David W Skinner
-- Email    : sasquatch@bentonrea.com
-- Website  : http://users.bentonrea.com/~sasquatch/sokoban/
-- With friendly permission from David W Skinner.
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!!!##########",
  "####    ##  #",
  "#  ooo....o@#",
  "#      ###  #",
  "#   ####!####",
  "#####!!!!!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

shogunLaser = 1
play_sokoban(level,870)

document(xlo+3,ylo+3,"Stones don't like corners. They prefer to die there..")

