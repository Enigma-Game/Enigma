-- A level for Enigma
-- Name:        Swamp Stomp
-- Filename:    ralf11.lua
-- Copyright: 	(C) May 2003 Ralf Westram
-- Contact: 	amgine@reallysoft.de
-- License: 	GPL v2.0 or above

dofile(enigma.FindDataFile("levels/lib/ralf.lua"))

--debug_mode()

--   7  0  1
--   6  +  2
--   5  4  3

level={
   "!!!###o######o###!!!",
   "!!!# 567    123 #!!!",
   "!!!#  B         #!!!",
   "####            ####",
   "#   b%%%%%%%%%%b   #",
   "#5  %%%%%%%%%%%%  7#",
   "o4  %%%%%%%%%%%%  0o",
   "#3  %%%%%%%%%%%%  1#",
   "#   b%%%%%%%%%%b   #",
   "####            ####",
   "!!!#         W  #!!!",
   "!!!# 765    321 #!!!",
   "!!!###o######o###!!!",
}

blocker="st-white4"
--blocker="st-black4" --testing only

cells={}

cells["!"] = cell{floor="fl-space"}
cells[" "] = cell{floor="fl-red"}
cells["%"] = cell{floor="fl-swamp"}
cells["#"] = cell{floor="fl-rough-blue",stone="st-glass"}

if (difficult) then
   cells["b"] = cell{parent=cells["%"], stone=blocker}

   cells["0"] = cell{parent=cells[" "],stone=blocker}
   cells["1"] = cells["0"]
   cells["2"] = cells["0"]
   cells["3"] = cells["0"]
   cells["4"] = cells["0"]
   cells["5"] = cells["0"]
   cells["6"] = cells["0"]
   cells["7"] = cells["0"]
else
   cells[" "] = cell{floor="fl-normal"}
   cells["b"] = cells["%"]

   cells["0"] = cell{parent={{gradient,SLOPE_N       }},stone=blocker}
   cells["1"] = cell{parent={{gradient,SLOPE_SMALL_NE}},stone=blocker}
   cells["2"] = cell{parent={{gradient,SLOPE_E       }},stone=blocker}
   cells["3"] = cell{parent={{gradient,SLOPE_SMALL_SE}},stone=blocker}
   cells["4"] = cell{parent={{gradient,SLOPE_S       }},stone=blocker}
   cells["5"] = cell{parent={{gradient,SLOPE_SMALL_SW}},stone=blocker}
   cells["6"] = cell{parent={{gradient,SLOPE_W       }},stone=blocker}
   cells["7"] = cell{parent={{gradient,SLOPE_SMALL_NW}},stone=blocker}
end

cells["B"] = cell{parent=cells[" "],actor={"ac-blackball", {player=0,essential=0}}}
cells["W"] = cell{parent=cells[" "],actor={"ac-whiteball", {essential=1,controllers=0}}}
cells["o"] = cell{oxyd}

rs_create_world(level,cells)
enigma.AllowTogglePlayer = FALSE
enigma.ConserveLevel = FALSE
oxyd_shuffle()













