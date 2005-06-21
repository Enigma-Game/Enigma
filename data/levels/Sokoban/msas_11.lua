-- A sokoban clone for Enigma
-- Name    : Mas Sasquatch 11
-- Filename: msas_11.lua
--
-- Taken from Mas Sasquatch
-- Copyright: David W Skinner
-- Email    : sasquatch@bentonrea.com
-- Website  : http://users.bentonrea.com/~sasquatch/sokoban/
-- Released : August 1999
-- With friendly permission from David W Skinner.
--
-- Converted to enigma format by Ralf Westram (amgine@reallysoft.de)


dofile(enigma.FindDataFile("levels/ralf_sokoban.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
  "!!!!####!!!!!!!!!!!!",
  "!!!!#  ####!!!!!!!!!",
  "!!!!# o   #########!",
  "!!!!# .#    o ##  #!",
  "!!!!# o# .## o    ##",
  "!#### .###   #oo   #",
  "##  ## #  .. # oo  #",
  "#  o      ...#   o #",
  "# o  #####... #   ##",
  "#  o#   #  .**@####!",
  "###   # #    # #!!!!",
  "!!#####  ####  #!#!!",
  "!!!!!!##      ##!!!!",
  "!!!!!!!########!!!!!",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

play_sokoban(level,1315)

document(xlo+5,ylo+3,"Excerpt from the 'Mas Sasquatch' set by David W Skinner. ")

