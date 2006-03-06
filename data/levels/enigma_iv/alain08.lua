-- Wood robbing, a level for Enigma
-- Copyright (C) 2006 Alain Busser
-- Licensed under GPL v2.0 or above 

levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "a"
draw_border("st-bumps")
fill_floor("fl-brick", 0,0, level_width,level_height)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone("st-bombs", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
            elseif c=="c" then
            	set_stone("st-key_a", i-1,line, {action="callback", target="abrir"})
            elseif c =="k" then
			set_item("it-key_a",i-1,line)
  		elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5, {player=0})
		elseif c=="a" then
			set_item("it-trigger",i-1,line,{action="openclose", target="door2"})
		elseif c=="b" then
			set_item("it-whitebomb", i-1, line)
		elseif c=="e" then
			set_item("it-dynamite", i-1,line)
		elseif c=="d" then
			set_item("it-trigger",i-1,line,{action="openclose", target="door1"})
		elseif c=="*" then
			set_stone ( "st-brick", i-1,line)
		elseif c=="+" then
			set_stone("st-grate1", i-1,line)
		elseif c=="w" then
			set_stone("st-wood", i-1,line)
		elseif c=="g" then
			doorh(i-1,line, {name="door1"})
		elseif c=="h" then
			doorh(i-1,line, {name="door2"})
		elseif c=="i" then
			doorv(i-1,line,{name="door3"})
		elseif c=="j" then
			doorv(i-1,line,{name="door4"})
		elseif c=="m" then
			doorv(i-1,line,{name="door5"})
		elseif c=="l" then
			doorv(i-1,line,{name="door6"})
		elseif c=="p" then
			set_item("it-trigger",i-1,line,{action="openclose", target="door3"})
		elseif c=="q" then
			set_item("it-trigger",i-1,line,{action="openclose", target="door4"})
		elseif c=="r" then
			set_item("it-trigger",i-1,line,{action="openclose", target="door5"})
		elseif c=="s" then
			set_item("it-trigger",i-1,line,{action="openclose", target="door6"})
		elseif c=="u" then
			set_item("it-blackbomb",i-1,line)
			set_stone("st-wood",i-1,line)
		elseif c=="B" then
			set_item("it-blackbomb",i-1,line)
		end
	end	
end

function abrir()
	document (10,3,"Gotcha!")
end


--               01234567890123456789
renderLine(00 , "                    ")
renderLine(01 , "        dap  +   ek ")
renderLine(02 , "  www   qrs  ****** ")
renderLine(03 , "  w+uw       +w w  #")
renderLine(04 , "  www        +w w   ")
renderLine(05 , "        z B  +w w c ")  
renderLine(06 , "             +w w   ")
renderLine(07 , "             +w w   ")
renderLine(08 , " **h*************** ")
renderLine(09 , "     i j m l        ")
renderLine(10 , " **************g*** ")
renderLine(11 , " o                o ")
renderLine(12 , "                    ") 
--               01234567890123456789

oxyd_shuffle()












