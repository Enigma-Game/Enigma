-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Path in the Stars

levelw=20
levelh=13

create_world( levelw, levelh)

fill_floor("fl-bluegreen", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "=" then
			fill_floor("fl-bluegreen", i-1,line, 1,1)
		elseif c == "+" then
			fill_floor("fl-space", i-1,line, 1,1)
		elseif c == "z" then
			set_actor("ac-blackball", i-1+.5,line+.5, {player=0})
		end
	end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"!!!!!!!!!!!!!!!!!!!!")
renderLine(01,"!!!!=+++++++=!!!=+=!")
renderLine(02,"!!!!+!!!!!!!+!!!+!+!")
renderLine(03,"!=+++++++=!!=+!!+!=!")
renderLine(04,"!+!!+!!!!+!!!++!+!o!")
renderLine(05,"!=++=!=+++++=!+++!!!")
renderLine(06,"!!!!!!+!!+!!+!!=++=!")
renderLine(07,"!!oz=+++++++=!!!+!+!")
renderLine(08,"!!!!!!+!!+!!!!!!+!+!")
renderLine(09,"!=+++++++=!=++++++=!")
renderLine(10,"!+!!!!+!!!!+!!!!+!!!")
renderLine(11,"!=++++=!!!!=++++=!!!")
renderLine(12,"!!!!!!!!!!!!!!!!!!!!")

oxyd_shuffle()














