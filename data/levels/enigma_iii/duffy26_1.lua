-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Lock Them In

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
		elseif c=="w" then
			set_actor("ac-whiteball-small", i-.5,line+.5, {player=0})
		elseif c=="H" then
		   hollow(i-1,line)
	elseif c=="h" then
		   set_item("it-hill",i-1,line)
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

		elseif c == "a" then
		   set_stone("st-switch",i-1,line,{target="doora",action="openclose"})
		elseif c == "b" then
		   set_stone("st-switch",i-1,line,{target="doorb",action="openclose"})
		elseif c == "c" then
		   set_stone("st-switch",i-1,line,{target="doorc",action="openclose"})
		elseif c == "e" then
		   set_stone("st-switch",i-1,line,{target="doord",action="openclose"})
		elseif c=="A" then
		   set_stone("st-door_b",i-1,line,{name="doora"})
			set_floor("fl-gradient",  i-1,  line, {type=3})
		elseif c=="B" then
		   set_stone("st-door_b",i-1,line,{name="doorb"})
			set_floor("fl-gradient",  i-1,  line, {type=3})
		elseif c=="C" then
		   set_stone("st-door_b",i-1,line,{name="doorc"})
			set_floor("fl-gradient",  i-1,  line, {type=4})
		elseif c=="D" then
		   set_stone("st-door_b",i-1,line,{name="doord"})
			set_floor("fl-gradient",  i-1,  line, {type=4})
end
	end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"########ba##########")
renderLine(01,"#3llllllClH#3lllll1#")
renderLine(02,"#3llllllDlH#d#####u#")
renderLine(03,"#d##########d#3l1#u#")
renderLine(04,"#d#3llllllll7#d#u#u#")
renderLine(05,"#d#d##########d#u#u#")
renderLine(06,"#d#d#3rrwwwwll7#u#u#")
renderLine(07,"#d#d#u##########u#u#")
renderLine(08,"#d#d#u#3rrrrrrrr7#u#")
renderLine(09,"#d#9r7#u##########u#")
renderLine(10,"#d#####u#HrBrrrrrr7#")
renderLine(11,"#9rrrrr7#HrArrrrrr7#")
renderLine(12,"##########ce########")

SendMessage("doora","openclose")
SendMessage("doorb","openclose")
SendMessage("doorc","openclose")
SendMessage("doord","openclose")













