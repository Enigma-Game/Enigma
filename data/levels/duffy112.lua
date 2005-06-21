-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Versailles

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock1", i-1, line)
		elseif c =="%" then
			set_stone( "st-rock1", i-1, line)
			set_floor("fl-sand",i-1,line)
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
			set_floor("fl-sand",i-1,line)
--			set_item("it-seed",i-1,line)
		elseif c=="y" then
		   set_actor("ac-whiteball", i-1,line+.5, {player=1})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="S" then
			set_item("it-seed",i-1,line)
			set_floor("fl-sand",i-1,line)
		elseif c=="C" then
			set_stone("st-grate1",i-1,line)
			set_floor("fl-sand",i-1,line)
			set_item("it-crack3",i-1,line)
		elseif c=="c" then
			set_item("it-crack3",i-1,line)
		elseif c=="s" then
			set_floor("fl-sand",i-1,line)
		elseif c=="U" then
			set_stone("st-oneway_black", i-1,line, {orientation=enigma.NORTH})
		elseif c=="R" then
			set_stone("st-oneway_black", i-1,line, {orientation=enigma.EAST})
		elseif c=="D" then
			set_stone("st-oneway_black", i-1,line, {orientation=enigma.SOUTH})
		elseif c=="L" then
			set_stone("st-oneway_black", i-1,line, {orientation=enigma.WEST})
		elseif c=="r" then
			set_stone("st-oneway_black", i-1,line, {orientation=enigma.EAST})
			set_floor("fl-sand",i-1,line)
		elseif c=="l" then
			set_stone("st-oneway_black", i-1,line, {orientation=enigma.WEST})
			set_floor("fl-sand",i-1,line)
	     end
	end	
end

renderLine(00,"####################")
renderLine(01,"o  LggclSszSrcggR  o")
renderLine(02,"# gLgg %%SS%% ggRg #")
renderLine(03,"#UU# g ~%%%%~ g #UU#")
renderLine(04,"#  L g ~~~~~~ g R  #")
renderLine(05,"#DD# g ~~~~~~ g #DD#")
renderLine(06,"o  L g ~~~~~~ g R  o")
renderLine(07,"#UU# g ~~~~~~ g #UU#")
renderLine(08,"#  L g ~~~~~~ g R  #")
renderLine(09,"#DD# g ~%%%%~ g #DD#")
renderLine(10,"# gLgg %%SS%% ggRg #")
renderLine(11,"o  LggcrSSSSlcggR  o")
renderLine(12,"####################")
