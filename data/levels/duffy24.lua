-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Turbulence

levelw=60
levelh=30

create_world( levelw, levelh)

fill_floor("fl-bluegreen", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "D" then
			set_stone( "st-death", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "=" then
			fill_floor("fl-bluegreen", i-1,line, 1,1)
		elseif c == "+" then
			fill_floor("fl-space", i-1,line, 1,1)
		elseif c == "a" then
  			set_stone("st-switch", i-1,line, {action="openclose", target="door1"})
		elseif c == "A" then
			set_stone("st-door", i-1, line , {name="door1", type="v"})
		elseif c == "z" then
			set_actor("ac-blackball", i-1+1,line+.5)
		elseif c == "d" then --1-d
			set_floor("fl-gradient",  i-1,  line, {type=1})
		elseif c == "u" then --2-u
			set_floor("fl-gradient",  i-1,  line, {type=2})
		elseif c == "r" then --3-r
			set_floor("fl-gradient",  i-1,  line, {type=3})
		elseif c == "l" then --4-l
			set_floor("fl-gradient",  i-1,  line, {type=4})
		elseif c == "1" then --ur
			set_floor("fl-gradient",  i-1,  line, {type=11})
		elseif c == "3" then --dl
			set_floor("fl-gradient",  i-1,  line, {type=9})
		elseif c == "7" then --dr
			set_floor("fl-gradient",  i-1,  line, {type=12})
		elseif c == "9" then --ul
			set_floor("fl-gradient",  i-1,  line, {type=10})
		end
	end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"!!!!!!!!!!!!!!!!!!!!")
renderLine(01,"!DdulrdulDDulrdulrD!")
renderLine(02,"!uordulrdulrdulrdol!")
renderLine(03,"!rdulrDulrdulDdulrd!")
renderLine(04,"!ulrdolrdulrduordul!")
renderLine(05,"!rdulrdulrdulrdulrd!")
renderLine(06,"!oDrdulrdz=rdulrdDo!")
renderLine(07,"!rdulrdulrdulrdulrd!")
renderLine(08,"!ulrdolrdulrduordul!")
renderLine(09,"!rdulrDulrdulDdulrd!")
renderLine(10,"!uordulrdulrdulrdol!")
renderLine(11,"!DdulrdulDDulrdulrD!")
renderLine(12,"!!!!!!!!!!!!!!!!!!!!")

oxyd_shuffle()