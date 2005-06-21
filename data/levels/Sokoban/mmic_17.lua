-- A sokoban clone for Enigma
-- Name    : Mas Microban 17
-- Filename: mmic_17.lua
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
  "!#####",
  "!#   #",
  "!#o#@#",
  "## o #",
  "#   ##",
  "#.*._#",
  "#____#",
  "######",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,1191)

document(xlo+1,ylo+6,"Excerpt from the 'Mas Microban' set by David W Skinner. David writes: \"Like 'Microban' this set is for beginners and children. Most of the puzzles are small and illustrate a particular concept.\"       I'm not quite sure what kind of children David has.. well nevertheless have fun with these \"childish\" levels. ")

