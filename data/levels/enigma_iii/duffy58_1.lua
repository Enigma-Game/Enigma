-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Upstream Journey

levelw=20
levelh=13

create_world( levelw, levelh)

fill_floor("fl-black", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-glass", i-1, line)
		elseif c =="%" then
			set_stone( "st-actorimpulse_invisible", i-1, line)
			abyss(i-1,line)
		elseif c =="$" then
			set_stone( "st-invisible", i-1, line)
			abyss(i-1,line)
		elseif c == "o" then
			oxyd( i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
--			fill_floor("fl-water", i-1,line, 1,1)
		elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="x" then
		        fill_floor("fl-black", i-1,line, 1,1)
		     elseif c=="g" then
			set_stone("st-grate1",i-1,line)
		     elseif c=="N" then
			set_item("it-vortex-open", i-1, line, {targetx=i-.5,targety=line-.5})
		     elseif c=="E" then
			set_item("it-vortex-open", i-1, line, {targetx=i+.5,targety=line+.5})
		     elseif c=="S" then
			set_item("it-vortex-open", i-1, line, {targetx=i-.5,targety=line+1.5})
		     elseif c=="W" then
			set_item("it-vortex-open", i-1, line, {targetx=i-1.5,targety=line+.5})
		     elseif c=="w" then
			set_item("it-vortex-open", i-1, line, {targetx=i-2.5,targety=line+.5})
		     elseif c=="Q" then
			set_item("it-vortex-open", i-1, line)
		     elseif c=="X" then
			set_stone( "st-death", i-1, line)
			abyss(i-1,line)
		     elseif c=="q" then
			set_stone( "st-death", i-1, line)
		end
	end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"$$$$$$$$$$$$$$$$$$$$")
renderLine(01,"$!!%%%%%%%%%%%%%%!!$")
renderLine(02,"$!!%EEEEEEEEEEES%!!$")
renderLine(03,"$oxxN!%%%%%%%%!Sxxo$")
renderLine(04,"$!!%N%!!!!!!!!%S%!!$")
renderLine(05,"$!!%N%!!!!!!!!%S%!!$")
renderLine(06,"$!!%Nxxo!!!!oxxS!!!$")
renderLine(07,"$!!%N%!!!!!!!!%S%!!$")
renderLine(08,"$!!%N%!!!!!!!!%S%!!$")
renderLine(09,"$oxxN!XXXXXXXX!Sxxo$")
renderLine(10,"$!!%NWqwzqqxQqwW%!!$")
renderLine(11,"$!!%%%XXXXXXXX%%%!!$")
renderLine(12,"$$$$$$$$$$$$$$$$$$$$")

oxyd_shuffle()
















