-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Break Down the Wall

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-rock", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock1", i-1, line)
		elseif c =="B" then
			set_stone( "st-black1", i-1, line)
		elseif c =="W" then
			set_stone( "st-white1", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-.5,line+.5, {player=0})
		elseif c=="y" then
		   set_actor("ac-whiteball", i-1,line+.5, {player=1})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c == "G" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
			set_item("it-hollow",i-1,line)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="r" then
 			set_stone("st-oneway_black", i-1,line, {orientation=enigma.EAST})
		elseif c=="s" then
		   set_item("it-seed",i-1,line)
	     end
	end	
end

renderLine(00,"####################")
renderLine(01,"#o     GrrrrG     o#")
renderLine(02,"#    s G rrrG      #")
renderLine(03,"#   s  GrrrrG      #")
renderLine(04,"#o   s GrrrrG     o#")
renderLine(05,"#   s  G rrrG      #")
renderLine(06,"#  z   GrrrrG      #")
renderLine(07,"#   s  G rrrG      #")
renderLine(08,"#o   s GrrrrG     o#")
renderLine(09,"#   s  GrrrrG      #")
renderLine(10,"#    s G rrrG      #")
renderLine(11,"#o     GrrrrG     o#")
renderLine(12,"####################")
















