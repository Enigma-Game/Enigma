-- Black Bilball the hobbyte, a level for Enigma
-- Copyright (C) 2006 Alain Busser
-- Licensed under GPL v2.0 or above 

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

levelw = 20
levelh = 13

create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
oxyd_default_flavor = "a"
fill_floor("fl-leaves", 0,0, level_width,level_height)
draw_border("st-rock1")


function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="x" then
			set_stone("st-death_invisible",i-1,line)
		elseif c=="a" then
			set_stone("st-knight", i-1,line)
		elseif c==">" then
			set_stone("st-oneway-e",i-1,line)
		elseif c=="D" then
			document(i-1,line,"Mightie art the Knightes of The Olde Ages who keepe the Elfic Oxyde Stones, and numerous art the Ghostes who lye in the waye to them; many art the adventurers who tryed to steale Aylendil the Sworde but none has ever come back. Hazy is the swampe where the Magick of the Ancients is buryed; the lair of Whiteballs the dragon thu shalt avoyd in feare of awaken him...  ")
		elseif c=="i" then
			set_item("it-glasses", i-1,line)
			set_floor("fl-swamp",i-1,line)
			set_stone("st-chameleon",i-1,line)
		elseif c=="j" then
			set_item("it-sword", i-1,line)
			--set_floor("fl-swamp",i-1,line)
			--set_stone("st-chameleon",i-1,line)
		elseif c=="k" then
			set_stone("st-invisible", i-1,line)
			--set_stone("st-chameleon",i-1,line)
		elseif c=="l" then
			set_stone("st-chameleon",i-1,line)
		elseif c=="*" then
		   	set_item("it-burnable_fireproof",i-1,line)
		elseif c=="w" then
			set_stone("st-whiteballs",i-1,line)
		elseif c=="-" then
			set_floor("fl-swamp",i-1,line)
		elseif c=="+" then
			set_stone("st-rock4",i-1,line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c=="t" then
			set_item("it-trigger",i-1,line,{invisible=1,action="callback",target="func1"})
		elseif c=="O" then
			set_item("it-ring",i-1,line)
  		elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5, {name="ac0"})
		end
	end	
end


function func1()
	set_item("it-burnable_ignited",9,11)
	set_item("it-burnable_ignited",9,10)

end


--               01234567890123456789
renderLine(00 , "                    ")
renderLine(01 , " oa              ao ")
renderLine(02 , " aa              aa ")
renderLine(03 , " xxxxxxxxxxxxxx xxx ")
renderLine(04 , "                    ")
renderLine(05 , " x xxxxxxxxxxxxxxxx ")  
renderLine(06 , " ****************** ")
renderLine(07 , "   z  D     kkklkkk ")
renderLine(08 , " ----       kllllll ")
renderLine(09 , " ----    t  klkkkkk ")
renderLine(10 , " -O--   + + klkl> j ")
renderLine(11 , " i---   + + klllk   ")
renderLine(12 , "         w          ") 
--               01234567890123456789

oxyd_shuffle()
















