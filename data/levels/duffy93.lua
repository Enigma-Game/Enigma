-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Seed Puzzle 2

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.SetCompatibility("oxyd1")
enigma.ConserveLevel=FALSE

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock6", i-1, line)
		elseif c =="&" then
		   set_stone( "st-glass",i-1,line)
		   set_item("it-blackbomb",i-1,line)
		elseif c == "o" then
		   set_stone("st-oxyd",i-1, line,{color="0"})
		elseif c == "p" then
		   set_stone("st-oxyd",i-1, line,{color="1"})
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
		        set_stone("st-oneway", i-1,line, {orientation=enigma.NORTH})
		elseif c=="R" then
 			set_stone("st-oneway", i-1,line, {orientation=enigma.EAST})
		elseif c=="D" then
			set_stone("st-oneway", i-1,line, {orientation=enigma.SOUTH})	
		elseif c=="L" then
 			set_stone("st-oneway", i-1,line, {orientation=enigma.WEST})
		elseif c=="S" then
		   set_item("it-seed",i-1,line)
		elseif c=="%" then
		   set_stone("st-glass_move",i-1,line)
		elseif c=="B" then
		   set_stone("st-bombs",i-1,line)
		elseif c=="A" then
		   set_item("it-crack3",i-1,line)
		elseif c=="C" then
		   set_item("it-crack3",i-1,line)
                   draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
	     end
	end	
end

renderLine(00,"#########oo#########")
renderLine(01,"#B L  & R  L &  R B#")
renderLine(02,"#### S  ####  S ####")
renderLine(03,"#                  #")
renderLine(04,"# #U#C###UU#####U# #")
renderLine(05,"# # !  S#  #S  ! # #")
renderLine(06,"#%###   L zR   ###%#")
renderLine(07,"# # !  S#  #S  ! # #")
renderLine(08,"# #D#####DD##C##D# #")
renderLine(09,"#                  #")
renderLine(10,"#### S  ####  S ####")
renderLine(11,"#B L  & R  L &  R B#")
renderLine(12,"#########oo#########")