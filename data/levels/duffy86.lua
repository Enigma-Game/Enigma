-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Fly High

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)

fill_floor("fl-bluegray", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c =="%" then
			set_stone( "st-greenbrown", i-1, line)
			abyss(i-1,line)
		elseif c =="X" then
			set_stone( "st-death", i-1, line)
			abyss(i-1,line)
		elseif c =="$" then
			set_stone( "st-death", i-1, line)
			abyss(i-1,line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "O" then
		   set_item("it-extralife",i-1,line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-1,line+.5, {player=0})
		   set_item("it-extralife",i-1,line)
		elseif c=="y" then
		   set_actor("ac-whiteball", i-.5,line+.5, {player=1,mouseforce=0})
		elseif c=="J" then
		   set_item("it-springboard",i-1,line)
		elseif c=="w" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {player=0, mouseforce=0})
		elseif c=="W" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {player=0, mouseforce=1})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="q" then
  		   set_item("it-vortex-open", i-1, line)
	     end
	end	
end

renderLine(00,"XXXXXXXXXXXXXXXXXXXX")
renderLine(01,"X!!!!!!!!OO!!!!!!!!X")
renderLine(02,"Xo!qq!qq!qq!qq!qq!oX")
renderLine(03,"X!!!!!!!!!!!!!!!!!!X")
renderLine(04,"X!!q!!X!!!!!!X!!q!!X")
renderLine(05,"X!!!!!X!!qq!!X!!!!!X")
renderLine(06,"X!!o!!X!!Oz!!X!!o!!X")
renderLine(07,"X!!!!!X!!qq!!X!!!!!X")
renderLine(08,"X!!q!!X!!!!!!X!!q!!X")
renderLine(09,"X!!!!!!!!!!!!!!!!!!X")
renderLine(10,"Xo!qq!qq!qq!qq!qq!oX")
renderLine(11,"X!!!!!!!!OO!!!!!!!!X")
renderLine(12,"XXXXXXXXXXXXXXXXXXXX")

oxyd_shuffle()
