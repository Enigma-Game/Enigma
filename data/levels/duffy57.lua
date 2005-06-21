-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Fool the Watchdog

levelw=20
levelh=13

create_world( levelw, levelh)

fill_floor("fl-mortar", 0,0,levelw,levelh)

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
		   set_actor("ac-whiteball-small", i-.5,line+.5, {player=0,mouseforce=0})
		elseif c=="W" then
		   set_actor("ac-whiteball-small", i-1,line+.5, {player=0,mouseforce=0})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="i" then
			set_floor("fl-ice_001",i-1,line)
		elseif c=="h" then
		   set_item("it-hollow",i-1,line)
		   set_floor("fl-ice_001",i-1,line)
		elseif c=="H" then
		   set_item("it-hollow",i-1,line)
		elseif c=="R" then
		   set_actor("ac-rotor", i-1,line+.5, {attacknearest=0, player=1, mouseforce=0, range=40, force=60})
		elseif c=="z" then
                    set_actor("ac-blackball", i-.5,line+.5)
	     end
	end	
end

renderLine(00,"####################")
renderLine(01,"#iiii          iiii#")
renderLine(02,"#ihii H  ww  H iihi#")
renderLine(03,"#ii==          ==ii#")
renderLine(04,"#ii==  w    w  ==ii#")
renderLine(05,"#                  #")
renderLine(06,"#zH ww  H RH  ww H #")
renderLine(07,"#                  #")
renderLine(08,"#ii==  w    w  ==ii#")
renderLine(09,"#ii==          ==ii#")
renderLine(10,"#ihii H  ww  H iihi#")
renderLine(11,"#iiii          iiii#")
renderLine(12,"####################")
