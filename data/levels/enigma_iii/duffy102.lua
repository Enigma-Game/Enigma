-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Guardian Angel

rooms_wide=3
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.SlopeForce=10

fill_floor("fl-hay", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c =="&" then
			set_stone( "st-death", i-1, line)
			set_floor("fl-space",i-1,line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == ":" then
			oxyd( i-1, line)
			set_floor("fl-gradient",  i-1,  line, {type=1})
		elseif c == ";" then
			oxyd( i-1, line)
			set_floor("fl-gradient",  i-1,  line, {type=2})
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
		elseif c == "L" then
			set_stone( "st-brownie", i-1, line)
			set_floor("fl-gradient",  i-1,  line, {type=1})
		elseif c == "M" then
			set_stone( "st-brownie", i-1, line)
			set_floor("fl-gradient",  i-1,  line, {type=2})
		elseif c == "N" then
			set_stone( "st-brownie", i-1, line)
			set_floor("fl-space",i-1,line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-.5,line+.5, {essential=1,controllers=0,player=1})
--			yy1( "black",  i-1, line)
			set_floor("fl-gradient",  i-1,  line, {type=23})
		elseif c=="y" then
		   set_actor("ac-whiteball", i-1,line+.5, {player=1})
		elseif c=="w" then
		   set_actor("ac-whiteball", i-1,line+.5,{player=1,controllers=0})
		elseif c=="k" then
		   set_actor("ac-killerball", i-1,line+1,{player=0,controllers=1})
			set_floor("fl-gradient",  i-1,  line, {type=1})
		elseif c=="A" then
		   set_actor("ac-killerball", i-1,line+1,{player=1,controllers=0})
			set_floor("fl-gradient",  i-1,  line, {type=1})
		elseif c=="B" then
		   set_actor("ac-killerball", i-1,line+1,{player=1,controllers=0})
			set_floor("fl-gradient",  i-1,  line, {type=2})
		elseif c=="C" then
		   set_actor("ac-whiteball", i-1,line+.5,{player=1,controllers=0})
		   set_floor("fl-space",  i-1,  line)
		elseif c=="F" then
		   set_actor("ac-whiteball", i-.5,line+.5, {player=1})
			set_floor("fl-gradient",  i-1,  line, {type=1})
		elseif c=="G" then
		   set_actor("ac-whiteball", i-.5,line+.5, {player=1})
			set_floor("fl-gradient",  i-1,  line, {type=2})
		elseif c=="H" then
		   set_actor("ac-whiteball", i-.5,line+.5, {player=1})
		   set_floor("fl-space",  i-1,  line)
 		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c == "d" then --1-d
			set_floor("fl-gradient",  i-1,  line, {type=1})
		elseif c == "u" then --2-u
			set_floor("fl-gradient",  i-1,  line, {type=2})
		elseif c == "r" then --3-r
			set_floor("fl-gradient",  i-1,  line, {type=3})
		elseif c == "l" then --4-l
			set_floor("fl-gradient",  i-1,  line, {type=4})
		elseif c == "1" then --ur
			set_floor("fl-gradient",  i-1,  line, {type=11})
		elseif c == "3" then --dl
			set_floor("fl-gradient",  i-1,  line, {type=9})
		elseif c == "7" then --dr
			set_floor("fl-gradient",  i-1,  line, {type=12})
		elseif c == "9" then --ul
			set_floor("fl-gradient",  i-1,  line, {type=10})
		elseif c=="R" then
			set_floor("fl-gradient",  i-1,  line, {type=23})
		elseif c=="D" then
			set_floor("fl-gradient",  i-1,  line, {type=1})
			set_stone( "st-greenbrown", i-1, line)
		elseif c=="U" then
			set_floor("fl-gradient",  i-1,  line, {type=2})
			set_stone( "st-greenbrown", i-1, line)
		elseif c=="6" then
			yy1( "black",  i-1, line)
			set_floor("fl-space",i-1,line)
--			set_floor("fl-gradient",  i-1,  line, {type=23})
		elseif c=="2" then
			yy1( "black",  i-1, line)
		   set_floor("fl-space",  i-1,  line)
	     end
	end	
     end

function yy1( color, x, y)
        stone = format( "st-%s4", color)
        set_stone( stone, x, y)
end

renderLine(00,"DDDDDDDDDDDDDDDDDDDD")
renderLine(01,":ddddddddddddddddddD")
renderLine(02,":dddkddddddFdddddddD")
renderLine(03,":dddddAddddLdddddddD")
renderLine(04,":ddddddddddddddddddD")
renderLine(05,":ddddddddddddddddddD")
renderLine(06,"oz========H====N=C6o")
renderLine(07,";uuuuuuuuuuuuuuuuuuU")
renderLine(08,";uuuuuuuuuuuuuuuuuuU")
renderLine(09,";uuuMuuuuuuuuBuuuuuU")
renderLine(10,";uuuGuuuuuuuuuuuuuuU")
renderLine(11,";uuuuuuuuuuuuuuuuuuU")
renderLine(12,"UUUUUUUUUUUUUUUUUUUU")

oxyd_shuffle()

display.SetFollowMode(display.FOLLOW_SCROLLING)















