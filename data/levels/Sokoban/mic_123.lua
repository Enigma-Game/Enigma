-- A sokoban clone for Enigma
-- Name    : Microban 123
-- Filename: mic_123.lua
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
  "########!!!!!!!",
  "#      #!!!!!!!",
  "# o oo ########",
  "##### @##._.  #",
  "!!!!#o  #_.   #",
  "!!!!#   #. . ##",
  "!!!!#o# ## # #!",
  "!!!!#        #!",
  "!!!!#  ###  ##!",
  "!!!!#  #!####!!",
  "!!!!####!!!!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,916)

