-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Clear the Path

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-bluegreen", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock2", i-1, line)
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
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="X" then
			set_stone("st-turnstile",i-1,line)
		elseif c=="U" then
			set_stone("st-turnstile-n",i-1,line)
		elseif c=="R" then
			set_stone("st-turnstile-e",i-1,line)
		elseif c=="D" then
			set_stone("st-turnstile-s",i-1,line)
		elseif c=="L" then
			set_stone("st-turnstile-w",i-1,line)
		elseif c=="u" then
		   puzzle(i-1,line,PUZ_1000)
		elseif c=="r" then
		   puzzle(i-1,line,PUZ_0001)
		elseif c=="d" then
		   puzzle(i-1,line,PUZ_0010)
		elseif c=="l" then
		   puzzle(i-1,line,PUZ_0100)
	     end
	end	
end

renderLine(00,"####################")
renderLine(01,"o U U U U U U U U o#")
renderLine(02,"# X X X X X X X X  #")
renderLine(03,"# U U   U   U   U  #")
renderLine(04,"# X X # X # X # X  #")
renderLine(05,"# Ulr    U  U XR ~ #")
renderLine(06,"ozX LXRU X LXRD o# #")
renderLine(07,"# D  D XRDlr  ## ~ #")
renderLine(08,"# X # X # X # X X  #")
renderLine(09,"# D   D   D   D D  #")
renderLine(10,"# X X X X X X X X  #")
renderLine(11,"o D D D D D D D D o#")
renderLine(12,"####################")

oxyd_shuffle()


















