-- The Enigmhanoi towers, a level for Enigma
-- Copyright (C) 2006 Alain Busser
-- Licensed under GPL v2.0 or above 
-- Modified fixed by Raoul

levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "a"
draw_border("st-rock1")
fill_floor("fl-tigris")

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="*" then
			set_stone( "st-rock1", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
            elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5, {player=0})
		elseif c=="a" then
			document(i-1,line, "who speaks, does not know (Lao Tse)") 
		elseif c=="b" then
			document(i-1,line, "The wise man laughs (Lao Tse)") 
		elseif c=="c" then
			document(i-1,line, "who knows, does not speak (Lao Tse)") 
		elseif c=="+" then
			set_stone("st-grate1", i-1,line)
		elseif c=="g" then
			doorv(i-1,line, {name="door1"})
		elseif c=="h" then
			doorv(i-1,line, {name="door2"})
		elseif c=="f" then
			set_floor("fl-water",i-1,line)
		elseif c=="l" then
			set_floor("fl-leaves",i-1,line)
     		elseif c == "2" then
         		set_item("it-banana",i-1,line)
		elseif c == "T" then
         		set_stone("st-switch",i-1,line, {action="open", target="door1"})
     		elseif c == "S" then
         		set_stone("st-switch",i-1,line, {action="open", target="door2"})
		end
	end	
end


--               01234567890123456789
renderLine(00 , "                    ")
renderLine(01 , "  *    a b c     *  ")
renderLine(02 , "  *              *  ")
renderLine(03 , "  * ll +++++ lll *  ")
renderLine(04 , "  * ll +   + lzl *  ")
renderLine(05 , "  * ll +   + l l *  ")  
renderLine(06 , "  g ll +++++ lll h  ")
renderLine(07 , "  *              *  ")
renderLine(08 , "  *              *  ")
renderLine(09 , "  f f        fff f  ")
renderLine(10 , "  fff ff fff f f f  ")
renderLine(11 , " of f ff f f fff fo ")
renderLine(12 , "                    ") 
--               01234567890123456789

--shogun sektion:
if difficult then
 set_stone("st-grate1",8,5)
 set_stone("st-grate1",10,5)
 set_stone("st-shogun-sml", 9,5)
 shogundot3(8,4,{action="openclose", target="door1"})
 shogundot3(10,4,{action="openclose", target="door2"})
elseif not difficult then
 set_stone("st-grate1",8,5)
 set_stone("st-grate1",10,4)
 set_stone("st-shogun-sml", 9,5)
 shogundot3(8,4,{action="openclose", target="door1"})
 shogundot3(10,5,{action="openclose", target="door2"})
end


oxyd_shuffle()



			















