-- Shipwreck Maze
-- A level for Enigma
-- Copyright (c) 2002 Nat Pryce
-- License: GPL v2.0 or above

dofile( enigma.FindDataFile("levels/lib/natmaze.lua") )
tight_maze( new_kruskal_maze(38,24), "fl-wood", "fl-water", nil )
oxyd_shuffle()
display.SetFollowMode(display.FOLLOW_SCROLLING)
















