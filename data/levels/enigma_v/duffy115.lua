-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Randomizer

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)

fill_floor("fl-ice_001", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock5", i-1, line)
		elseif c =="F" then
			set_stone( "st-fart", i-1, line)
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
		elseif c=="R" then
		   set_actor("ac-top", i-.5,line+.5, {force=3,range=100,player=1})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="S" then
		   set_stone("st-switch",i-1,line,{action="callback",target="funcc1"})
	     end
	end	
end

renderLine(00,"####################")
renderLine(01,"#F    o  SS  o    F#")
renderLine(02,"# S              S #")
renderLine(03,"#  o            o  #")
renderLine(04,"#                  #")
renderLine(05,"#      o    o      #")
renderLine(06,"#oS R          z So#")
renderLine(07,"#      o    o      #")
renderLine(08,"#                  #")
renderLine(09,"#  o            o  #")
renderLine(10,"# S              S #")
renderLine(11,"#F    o  SS  o    F#")
renderLine(12,"####################")

oxyd_shuffle()

function funcc1()
   oxyd_shuffle()
end















