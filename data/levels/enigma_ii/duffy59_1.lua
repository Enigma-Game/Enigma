-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Space Meditation

levelw=20
levelh=13

create_world( levelw, levelh)

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c=="w" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {player=0,mouseforce=1})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="A" then
		   set_item("it-hollow",i-1,line)
		elseif c=="B" then
		   set_floor("fl-space",i-1,line)
		   set_item("it-hollow",i-1,line)
	     end
	end	
end

renderLine(00,"####################")
renderLine(01,"#A                A#")
renderLine(02,"#  w            w  #")
renderLine(03,"# w==============w #")
renderLine(04,"#  ==============  #")
renderLine(05,"#  ====B====B====  #")
renderLine(06,"#  ==============  #")
renderLine(07,"#  ====B====B====  #")
renderLine(08,"#  ==============  #")
renderLine(09,"# w==============w #")
renderLine(10,"#  w            w  #")
renderLine(11,"#A                A#")
renderLine(12,"####################")














