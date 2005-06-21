-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Action at a Distance

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.ElectricForce=50

fill_floor("fl-floor_001", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock6", i-1, line)
		elseif c =="P" then
			set_stone( "st-chargeplus", i-1, line)
		elseif c =="M" then
			set_stone( "st-chargeminus", i-1, line)
		elseif c =="N" then
			set_stone( "st-chargezero", i-1, line)
		elseif c =="W" then
			set_stone( "st-white1", i-1, line)
		elseif c =="r" then
			set_floor("fl-rough",i-1,line)
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
		   set_item("it-yinyang",i-1,line)
			set_floor("fl-rough",i-1,line)
		elseif c=="y" then
		   set_actor("ac-whiteball", i-.5,line+.5, {player=1})
		   set_item("it-yinyang",i-1,line)
			set_floor("fl-rough",i-1,line)
		elseif c=="w" then
		   set_actor("ac-whiteball", i-.5,line+.5, {controllers=0,mouseforce=0,player=1})
			set_floor("fl-rough",i-1,line)
		elseif c=="Z" then
		   set_actor("ac-blackball", i-.5,line+.5, {controllers=0,mouseforce=0,player=1})
			set_floor("fl-rough",i-1,line)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="R" then
			set_stone("st-oneway_white", i-1,line, {orientation=enigma.EAST})
		elseif c=="L" then
			set_stone("st-oneway_black", i-1,line, {orientation=enigma.WEST})
	     end
	end	
end

renderLine(00,"####################")
renderLine(01,"#o                o#")
renderLine(02,"#                  #")
renderLine(03,"#      o    o      #")
renderLine(04,"#o                o#")
renderLine(05,"#PPP#          #MMM#")
renderLine(06,"NzrwR          LZryN")
renderLine(07,"#MMM#          #PPP#")
renderLine(08,"#o                o#")
renderLine(09,"#      o    o      #")
renderLine(10,"#                  #")
renderLine(11,"#o                o#")
renderLine(12,"####################")

oxyd_shuffle()
