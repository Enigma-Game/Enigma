-- A sokoban clone for Enigma
-- Name    : Mas Microban 104
-- Filename: mmic_104.lua
--
-- Taken from Mas Microban
-- Copyright: David W Skinner
-- Email    : sasquatch@bentonrea.com
-- Website  : http://users.bentonrea.com/~sasquatch/sokoban/
-- With friendly permission from David W Skinner.
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!######!!!!!!!",
  "!#____#######!",
  "!#_##_###___#!",
  "##_#_OO_#_..#!",
  "#____O__#..##!",
  "#_#_##O_a__###",
  "#_#__O__#..  #",
  "#____OO_#.   #",
  "###___###    #",
  "!!############",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,1236)

