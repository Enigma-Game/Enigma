-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Washing Machine

levelw=50
levelh=30

create_world( levelw, levelh)

fill_floor("fl-metal", 0,0,levelw,levelh)

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
                elseif c == " " then
			abyss(i-1,line)
		elseif c=="w" then
			set_actor("ac-whiteball-small", i-.5,line+.5, {player=0, mouseforce=1})
		elseif c=="H" then
		   hollow(i-1,line)
		elseif c=="*" then
		   set_stone("st-greenbrown",i-1,line)
		elseif c=="r" then
			set_floor("fl-gradient", i-1,line , {type=23})
		elseif c=="l" then
			set_floor("fl-gradient", i-1,line , {type=24})
		elseif c=="d" then
			set_floor("fl-gradient", i-1,line , {type=21})
		elseif c=="u" then
			set_floor("fl-gradient", i-1,line , {type=22})
		elseif c=="R" then
			set_floor("fl-gradient", i-1,line , {type=23})
set_actor("ac-whiteball-small", i-.5,line+.5, {player=0, mouseforce=1})
		elseif c=="L" then
			set_floor("fl-gradient", i-1,line , {type=24})
			set_actor("ac-whiteball-small", i-.5,line+.5, {player=0, mouseforce=1})
		elseif c=="D" then
			set_floor("fl-gradient", i-1,line , {type=21})
set_actor("ac-whiteball-small", i-.5,line+.5, {player=0, mouseforce=1})
		elseif c=="U" then
			set_floor("fl-gradient", i-1,line , {type=22})
			set_actor("ac-whiteball-small", i-.5,line+.5, {player=0, mouseforce=1})
		elseif c=="x" then
			set_floor("fl-metal", i-1,line)
		elseif c=="X" then
			set_floor("fl-metal", i-1,line)
set_actor("ac-whiteball-small", i-.5,line+.5, {player=0, mouseforce=1})
end
	end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"
 
renderLine(00,"   #############    ")
renderLine(01,"   #RrrrrrddddD#    ")
renderLine(02,"   #rrrrrrddddd#    ")
renderLine(03,"   #rrrrrrddddd#    ") 
renderLine(04,"   #rrrrrrddddd#    ")
renderLine(05,"   #rrrrHrHdddd#    ")   
renderLine(06,"   #uuuuu ddddd#    ")
renderLine(07,"   #uuuuHlHllll#    ")
renderLine(08,"   #uuuuullllll#    ")
renderLine(09,"   #uuuuullllll#    ")
renderLine(10,"   #uuuuullllll#    ")
renderLine(11,"   #UuuuulllllL#    ")
renderLine(12,"   #############    ")

oxyd_shuffle()
