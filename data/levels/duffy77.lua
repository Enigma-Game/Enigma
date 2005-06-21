-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Seed Puzzle

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
			set_stone( "st-rock5", i-1, line)
		elseif c =="%" then
			set_stone( "st-rock5", i-1, line)
			abyss(i-1,line)
		elseif c == "o" then
		   set_stone("st-oxyd",i-1, line,{color="0"})
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
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="U" then
		        set_stone("st-oneway-n", i-1,line)
		elseif c=="R" then
 			set_stone("st-oneway-e", i-1,line)
		elseif c=="D" then
			set_stone("st-oneway-s", i-1,line)	
		elseif c=="L" then
 			set_stone("st-oneway-w", i-1,line)
		elseif c=="S" then
		   set_item("it-seed",i-1,line)
	     end
	end	
end

renderLine(00,"####################")
renderLine(01,"#o gL          Rg o#")
renderLine(02,"#   # ###UU### #   #")
renderLine(03,"#   # #S  z S# #   #")
renderLine(04,"#   # # #### # #   #")
renderLine(05,"#!!%#U# #oo# #U#%!!#")
renderLine(06,"#!!%! LSR  LSR !%!!#")
renderLine(07,"#!!%#D# #oo# #D#%!!#")
renderLine(08,"#   # # #### # #   #")
renderLine(09,"#   # #S    S# #   #")
renderLine(10,"#   # ###DD### #   #")
renderLine(11,"#o gL          Rg o#")
renderLine(12,"####################")

oxyd_shuffle()