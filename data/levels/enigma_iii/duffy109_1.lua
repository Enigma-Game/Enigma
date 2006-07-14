-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: One-Way Streets

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-bluegray", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-blue-sand", i-1, line)
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
		   set_item("it-seed",i-1,line)
		elseif c=="y" then
		   set_actor("ac-whiteball", i-1,line+.5, {player=1})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="U" then
			set_stone("st-oneway_black", i-1,line, {orientation=enigma.NORTH})
		elseif c=="R" then
			set_stone("st-oneway_black", i-1,line, {orientation=enigma.EAST})
		elseif c=="D" then
			set_stone("st-oneway_black", i-1,line, {orientation=enigma.SOUTH})
		elseif c=="L" then
			set_stone("st-oneway_black", i-1,line, {orientation=enigma.WEST})
		elseif c=="S" then
		   set_item("it-seed",i-1,line)
	     end
	end	
end

renderLine(00,"####################")
renderLine(01,"#o#####S####S#####o#")
renderLine(02,"#  L  L R  L R  R  #")
renderLine(03,"#DD#DD# #DD# #DD#DD#")
renderLine(04,"#  #  # #  # #  #  #")
renderLine(05,"#DD#UU# #UU# #UU#DD#")
renderLine(06,"o  R  L LSzR R  L  o")
renderLine(07,"#UU#DD# #DD# #DD#UU#")
renderLine(08,"#  #  # #  # #  #  #")
renderLine(09,"#UU#UU# #UU# #UU#UU#")
renderLine(10,"#  L  L R  L R  R  #")
renderLine(11,"#o#####S####S#####o#")
renderLine(12,"####################")
















