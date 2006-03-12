-- A sokoban clone for Enigma
-- Name    : Sokoban 83
-- Filename: xsok_83.lua
--
-- Taken from Sokoban
-- Copyright: Thinking Rabbit
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/lib/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "#######!#########",
  "#     #!#   ##  #",
  "# ### #!#   o   #",
  "# # o ###   o   #",
  "#   oo      ##o #",
  "#    ####   ##  #",
  "#@############ ##",
  "###..    #####o #",
  "!!#..    ####   #",
  "!!#..       oo  #",
  "!!#..    #### o #",
  "!!#..    #!!#   #",
  "!!########!!#####",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,773)

