-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Drunkard's Walk

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-rough-blue", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c =="X" then
			set_stone( "st-death", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-1,line+.5, {player=0})
		elseif c=="y" then
		   set_actor("ac-whiteball", i-1,line+.5, {player=1})
		elseif c=="w" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {player=0})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="B" then
		   set_item("it-booze",i-1,line)
		elseif c=="H" then
		   set_item("it-hollow",i-1,line)
	     end
	end	
end

renderLine(00,"XXXXXXXXXXXXXXXXXXXX")
renderLine(01,"X                  X")
renderLine(02,"X          X    H  X")
renderLine(03,"X     w            X")
renderLine(04,"X          X  X   XX")
renderLine(05,"X     w         H  X")
renderLine(06,"X BX  w w  X   H H X")
renderLine(07,"X     w         H  X")
renderLine(08,"X          X  X   XX")
renderLine(09,"X     w            X")
renderLine(10,"X          X    H  X")
renderLine(11,"X                  X")
renderLine(12,"XXXXXXXXXXXXXXXXXXXX")

oxyd_shuffle()













