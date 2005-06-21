-- A level for Enigma
-- Name:        Run like hell
-- Filename:    ralf09.lua
-- Copyright: 	(C) May 2003 Ralf Westram
-- Contact: 	amgine@reallysoft.de
-- License: 	GPL v2.0 or above

dofile(enigma.FindDataFile("levels/ralf.lua"))

--debug_mode()

level={
   "####o#o#o###o#o#o###",
   "#  PvPvPvPvPvPvPvP #",
   "# PvPvPvPvPvPvPvP  #",
   "#  PvPvPvPvPvPvPvP #",
   "# PvPvPvPvPvPvPvP  #",
   "#  PvPvPvPvPvPvPvP #",
   "# PvPvPvPvPvPvPvP  #",
   "#  PvPvPvPvPvPvPvP #",
   "# PvPvPvPvPvPvPvP  #",
   "#  PvPvPvPvPvPvPvP #",
   "# PvPvPvPvPvPvPvP  #",
   "#a P P P P P P P Pd#",
   "####################",
}

cells={}

cells["!"] = cell{floor="fl-abyss"}
cells[" "] = cell{floor="fl-red"}

cells["#"] = cell{parent=cells[" "],stone="st-rock3"}
cells["P"] = cell{parent=cells[" "],stone="st-pull"}
cells["@"] = cell{parent=cells[" "],stone="st-brownie"}

cells["^"] = cell{parent=cells[" "],stone="st-bolder-n"}
cells["v"] = cell{parent=cells[" "],stone="st-bolder-s"}
cells[">"] = cell{parent=cells[" "],stone="st-bolder-e"}
cells["<"] = cell{parent=cells[" "],stone="st-bolder-w"}

cells["a"] = cell{parent=cells[" "],actor={"ac-blackball", {player=0}}}
cells["d"] = cell{{cells[" "],{document, "Calm down!"}}}

cells["o"] = cell{oxyd}


rs_create_world(level,cells)
oxyd_shuffle()
