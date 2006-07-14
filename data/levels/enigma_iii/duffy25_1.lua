-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Teamwork

levelw=100
levelh=20

create_world( levelw, levelh)
enigma.TwoPlayerGame=TRUE

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
                elseif c == "!" then
--			abyss(i-1,line)
			fill_floor("fl-water", i-1,line, 1,1)
		elseif c == "s" then
			set_stone("st-stoneimpulse", i-1,line)
			fill_floor("fl-water", i-1,line, 1,1)
		elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5)
--			set_item("it-yinyang", i-1, line+.5)
		elseif c=="y" then
			set_actor("ac-whiteball", i-.5,line+.5)
--			set_item("it-yinyang", i-1, line+.5)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
			fill_floor("fl-leaves", i-1,line, 1,1)
		elseif c==":" then
			fill_floor("fl-space", i-1,line, 1,1)
		elseif c=="=" then
			fill_floor("fl-leaves", i-1,line, 1,1)
		elseif c=="S" then
			set_stone( "st-swap", i-1, line)
		     elseif c=="*" then
			set_stone( "st-greenbrown", i-1, line)
				fill_floor("fl-water", i-1,line, 1,1)
		elseif c == "R" then
			puzzle(i-1,line,PUZ_0100)
		elseif c == "L" then
			puzzle(i-1,line,PUZ_0001)
		elseif c == "Q" then
			puzzle(i-1,line,PUZ_0011)
		elseif c == "U" then
			puzzle(i-1,line,PUZ_1100)
		elseif c=="g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="A" then
			doorv(i-1,line,{name="door1"})
		elseif c=="B" then
			doorv(i-1,line,{name="door2"})
		elseif c=="E" then
			doorv(i-1,line,{name="door3"})
		elseif c=="F" then
			doorv(i-1,line,{name="door4"})
		elseif c=="I" then
			fill_floor("fl-inverse", i-1,line, 1,1)
		elseif c=="D" then
			set_stone("st-floppy", i-1, line, {action="openclose", target="door2"})
		elseif c=="d" then
			set_item("it-floppy", i-1, line)
		elseif c=="C" then
			set_stone("st-key_a", i-1, line, {action="openclose", target="door1"})
		elseif c=="c" then		
			set_item("it-key_a", i-1 ,line)
		elseif c == "b" then
			yy1( "black",  i-1, line)
		elseif c == "w" then
			yy1( "white",  i-1, line)
		elseif c == "X" then
            		draw_stones("st-yinyang1", { i-1,line}, {0,1},1)
		elseif c=="P" then
			set_item("it-brush",i-1,line)
		elseif c=="p" then
			draw_stones("st-stonebrush", {i-1,line}, {1,0},1)
	        elseif c=="e" then
			set_stone("st-switch", i-1,line, {action="openclose",target="door3"})
	        elseif c=="f" then
			set_stone("st-switch", i-1,line, {action="openclose",target="door4"})
		elseif c=="l" then
			oneway(i-1,line, enigma.WEST)
		end
	end	
end

function yy1( color, x, y)
        stone = format( "st-%s4", color)
        set_stone( stone, x, y)
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"##################################################sssss###")
renderLine(01,"#  bbXbXbXb XXXXXdX#  D#              #!!!!!!!!!!!s!!!s !#")
renderLine(02,"#  #bwbwbw# XbXw XX# ###   + + + +    #!!!!!!!!!!!sss   !#")
renderLine(03,"#  #bwbwbw# XX XXbX#X  #          #bbb#!!!!      !!!* !! #")
renderLine(04,"#z #bwbwbw# XwXb XX#w# #  #!!!!!!##wwb#!!!! !!!! !!!  !! #")
renderLine(05,"#  #bwbwbw# XX XXwX#X#X##w##!#!#!##RQ #   ! !*        !! #")
renderLine(06,"#  #bwbwbw# XbXw X     AB #!X!X!X## UL  +   !!! !!!!! !! #")
renderLine(07,"#  #bwbwbw# XX XXbX#X#X##b#!#!#!###   #   ! !!! !!!!!*!! #")
renderLine(08,"#y #bwbwbw# XwXb XX#b# #  #!!!!!!##bbw#!!!! !!!    *!!!! #")
renderLine(09,"#  #bwbwbw# XX XXwX#X  #          #www#!!*        !!!    #")
renderLine(10,"#  #bwbwbw# XbXw XX# ###    + + +     #!!!!!!!!*! !!!#b#w#")
renderLine(11,"#  wXwXwXww XX XXcX#  C#              #!!!!!!!!!!*!!!#o#o#")
renderLine(12,"##########################################################")

oxyd_shuffle()

















