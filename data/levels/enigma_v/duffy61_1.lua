-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Mount Etna

levelw=20
levelh=13

create_world( levelw, levelh)

fill_floor("fl-concrete", 0,0,levelw,levelh)
enigma.ConserveLevel=FALSE

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "&" then
		   set_floor("fl-water",i-1,line)
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-1,line+.5)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="x" then
		   set_stone("st-volcano_active",i-1,line)
		elseif c=="F" then
		   set_item("it-flagblack",i-1,line)
		elseif c=="A" then
		   set_item("it-soother",i-1,line)
	     end
	end	
end

renderLine(00,"#oo##############oo#")
renderLine(01,"o                  o")
renderLine(02,"o                  o")
renderLine(03,"#                  #")
renderLine(04,"#                  #")
renderLine(05,"#                  #")
renderLine(06,"#        xx        #")
renderLine(07,"#                  #")
renderLine(08,"#                  #")
renderLine(09,"#         z        #")
renderLine(10,"o                  o")
renderLine(11,"o                  o")
renderLine(12,"#oo##############oo#")

oxyd_shuffle()

















