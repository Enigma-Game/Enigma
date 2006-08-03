-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Help Yourself

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

enigma.Brittleness=0

oxyd_default_flavor="c"

fill_floor("fl-wood", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock4", i-1, line)
		elseif c =="X" then
			set_stone( "st-death", i-1, line)
		elseif c =="F" then
			set_stone( "st-fart", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "O" then
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
		elseif c=="S" then
		   set_stone("st-switch",i-1,line,{action="callback",target="funcc1"})
		elseif c=="1" then
			set_item("it-crack3",i-1,line)
		elseif c=="2" then
			set_item("it-crack2",i-1,line)
		elseif c=="3" then
			set_item("it-crack1",i-1,line)
		elseif c=="U" then
 			set_stone("st-oneway", i-1,line, {orientation=enigma.NORTH})
		elseif c=="R" then
 			set_stone("st-oneway", i-1,line, {orientation=enigma.EAST})
		elseif c=="D" then
 			set_stone("st-oneway", i-1,line, {orientation=enigma.SOUTH})
		elseif c=="L" then
 			set_stone("st-oneway", i-1,line, {orientation=enigma.WEST})
	     end
	end	
end

renderLine(00,"####################")
renderLine(01,"#F   O#o    L   R  #")
renderLine(02,"#  z  ####### ###o #")
renderLine(03,"#             L  #2#")
renderLine(04,"#     ### O####o # #")
renderLine(05,"#     S # #O  O#U#D#")
renderLine(06,"#O   O# # #S   #   #")
renderLine(07,"####### # ####2##o #")
renderLine(08,"#O    2   R 2      #")
renderLine(09,"#  S  ########2###2#")
renderLine(10,"#  S  O  S   S #o  #")
renderLine(11,"#O   O#o   S   2  S#")
renderLine(12,"####################")

oxyd_shuffle()

function funcc1()
   oxyd_shuffle()
end

















