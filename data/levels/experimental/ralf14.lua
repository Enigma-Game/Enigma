-- A level for Enigma
-- Name:        Escape?
-- Filename:    ralf14.lua
-- Copyright: 	(C) Jul 2003 Ralf Westram
-- Contact: 	amgine@reallysoft.de
-- License: 	GPL v2.0 or above

dofile(enigma.FindDataFile("levels/lib/ralf.lua"))

--debug_mode()

level={
   "####################",
   "#osssssssssssssssso#",
   "#s________________s#",
   "#s____%%%%%%%%____s#",
   "#s____%  a   %____s#",
   "#s____%lxiixr%____s#",
   "#s____% pggp %____s#",
   "#s____% xiix %____s#",
   "#s____%      %____s#",
   "#s____%%%%%%%%____s#",
   "#s________________s#",
   "#osssssssssssssssso#",
   "####################",
}

cells={}

cells[" "] = cell{floor="fl-normal"}
cells["_"] = cell{floor="fl-leaves"}
cells["#"] = cell{floor="fl-water"}
cells["s"] = cell{floor="fl-sand"}
cells["%"] = cell{parent=cells[" "],stone="st-rock1"}
cells["o"] = cell{parent={cells["s"],oxyd}}

cells["x"] = cell{parent=cells[" "],stone="st-swap"}
cells["p"] = cell{parent=cells[" "],stone="st-pull"}
cells["l"] = cell{parent=cells[" "],stone="st-rotator_move-left"}
cells["r"] = cell{parent=cells[" "],stone="st-rotator_move-right"}
cells["g"] = cell{parent=cells[" "],stone="st-grate1"}
cells["i"] = cell{parent=cells[" "],stone="st-stoneimpulse_movable"}

cells["a"] = cell{parent=cells[" "],actor={"ac-blackball", {player=0}}}

rs_create_world(level,cells)
oxyd_shuffle()

enigma.ConserveLevel=FALSE












