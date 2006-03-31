-- Oxyd 68, a OxydClone for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above 
-- Twoplayer nr 86

-- GENERAL --
levelw = 20
levelh = 13
create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
oxyd_default_flavor = "a"
Require("levels/lib/libpuzzle.lua")

-- DESIGN --
boden="fl-abyss"
wand="st-brick"

fill_floor(boden, 0,0, level_width,level_height)
fill_floor("fl-metal", 0,1, level_width,level_height-2)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone(wand, i-1, line)
		elseif c =="w" then
			set_stone("st-break_acwhite", i-1, line)
		elseif c =="s" then
			set_stone("st-break_acblack", i-1, line)
		elseif c =="H" then
			set_stone("st-rock2_hole", i-1, line)
  		elseif c=="o" then
			oxyd(i-1,line)
		end
	end	
end

renderLine(00 , "                    ")
renderLine(01 , "####################")
renderLine(02 , "#        ##        #")
renderLine(03 , "#        ##        #")
renderLine(04 , "#                  #")
renderLine(05 , "#w       HH       s#")  
renderLine(06 , "ow       HH       so")
renderLine(07 , "#w                s#")
renderLine(08 , "#        ##        #")
renderLine(09 , "#        ##        #")
renderLine(10 , "#        ##        #")
renderLine(11 , "####################") 
renderLine(12,  "                    ")

oxyd_shuffle()

--Ring
puzzle({{1,1,1,1},{1,0,0,1},{1,0,0,1},{1,1,1,1}},8,4,"2")

-- Special
set_item("it-document",4,3,{text="Es ist ein Ring!"})
set_item("it-hammer",10,5)

--Actors
set_item("it-yinyang", 4,6)
set_item("it-yinyang", 15,6)
set_actor("ac-blackball", 4.5, 6.5, {player=0})
set_actor("ac-whiteball",  15.5, 6.5, {player=1})
