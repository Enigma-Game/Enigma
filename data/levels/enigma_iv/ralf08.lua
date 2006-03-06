-- A level for Enigma
-- Name:        - No Meditation -
-- Filename:    ralf08.lua
-- Copyright: 	(C) May 2003 Ralf Westram
-- Contact: 	amgine@reallysoft.de
-- License: 	GPL v2.0 or above

dofile(enigma.FindDataFile("levels/lib/ralf.lua"))

--debug_mode()

level={
   "####################",
   "#__________________#",
   "#________H___ _____#",
   "#_____ _____ . ____#",
   "#____ . _____ _____#",
   "#_____ _______H____#",
   "#__________________#",
   "#____H_______ _____#",
   "#_____ _____ . ____#",
   "#____ . _____ _____#",
   "#_____ ___H________#",
   "#__________________#",
   "####################",
}

cells={}

cells[" "] = cell{floor="fl-red"}
cells["#"] = cell{parent=cells[" "],stone="st-glass1"}
cells["_"] = cell{parent=cells[" "],stone="st-pull"}
cells["H"] = cell{parent=cells[" "],item="it-hollow"}
cells["."] = cell{parent=cells[" "],actor={"ac-whiteball-small", {player=0, mouseforce=1}}}

rs_create_world(level,cells)
oxyd_shuffle()
enigma.ConserveLevel=FALSE













