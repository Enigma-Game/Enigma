-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Monster in the Dark

levelw=20
levelh=13

create_world( levelw, levelh)

fill_floor("fl-abyss_fake", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-invisible", i-1, line)
		elseif c =="X" then
			set_stone( "st-death", i-1, line)
		elseif c =="%" then
			set_stone( "st-actorimpulse", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		   set_stone("st-chameleon",i-1,line)
		elseif c=="z" then
	set_actor("ac-blackball", i-.5,line+.5)
		   set_stone("st-chameleon",i-1,line)
		elseif c=="r" then
		   set_actor("ac-rotor", i-.5,line+.5, {player=1, mouseforce=0, range=30, force=5})
		   set_stone("st-chameleon",i-1,line)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c==" " then
		   set_stone("st-chameleon",i-1,line)
		elseif c=="O" then
		   set_item("it-extralife",i-1,line)
		   set_stone("st-chameleon",i-1,line)
	     end
	end	
end

renderLine(00,"##o#####o##o#####o##")
renderLine(01,"#                  #")
renderLine(02,"o                  o")
renderLine(03,"#  #  o#    #o  #  #")
renderLine(04,"#                  #")
renderLine(05,"#                  #")
renderLine(06,"#  r     ##     z  #")
renderLine(07,"#                  #")
renderLine(08,"#                  #")
renderLine(09,"#  #  o#    #o  #  #")
renderLine(10,"o                  o")
renderLine(11,"#                  #")
renderLine(12,"##o#####o##o#####o##")

oxyd_shuffle()















