-- April 1st, a level for Enigma
-- Copyright (C) 2006 Alain Busser
-- Licensed under GPL v2.0 or above 

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "a"
fill_floor("fl-woven", 0,0, level_width,level_height)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="w" then
			set_floor("fl-water", i-1, line)
		elseif c=="b" then
		   	set_item("it-burnable_fireproof",i-1,line)
--			set_item("it-burnable",i-1,line)
		elseif c=="A" then
			set_floor("fl-abyss_fake",i-1,line)
		elseif c=="a" then
			set_floor("fl-abyss",i-1,line)
		elseif c=="B" then
			set_floor("fl-bridge-closed",i-1,line,{name="br1"})
			set_item("it-trigger",i-1,line,{invisible=1,action="callback",target="func1"})
		elseif c=="p" then
			set_item("it-coin1",i-1,line)
		elseif c=="q" then
			set_item("it-key_a",i-1,line)
		elseif c=="P" then
			set_stone("st-coinslot",i-1,line,{action="callback", target="func3"}) 
		elseif c=="Q" then
			set_stone("st-key_a",i-1,line,{action="callback", target="func2"})
		elseif c=="d" then
			document(i-1,line,"Dou you REALLY want to do this ???")
		elseif c=="D" then
			document(i-1,line,"Didn't your mother tell you that you should not spend your money in such a careless manner ?")
		elseif c=="e" then
			document(i-1,line,"What is the current month ?")
		elseif c=="E" then
			document(i-1,line,"What is this level's name ?")
		elseif c == "o" then
			oxyd( i-1, line)
  		elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5, {name="ac0"})
		end
	end	
end

month=enigma.date("%B")

                              
function func1()
        SendMessage("br1", "open")
end

function func2()
	set_item("it-burnable_ignited",7,6)
	set_item("it-burnable_ignited",7,7)
	set_item("it-burnable_ignited",15,4)
	set_item("it-burnable_ignited",15,5)
	set_item("it-burnable_ignited",15,6)
	set_item("it-burnable_ignited",15,7)
	set_item("it-burnable_ignited",15,8)
end

function func3()
	for i=1,300 do
		xx=random(20)
		yy=random(13)
		set_floor("fl-water",xx,yy)
	end
end

function apr1()

        if month == "April" then

--               01234567890123456789
renderLine(00 , "wwwwwwwwwwwwwwwwwwww")
renderLine(01 , "wwwww   dQp wwwwwwww")
renderLine(02 , "wwwb          wwwww ")
renderLine(03 , "ww  b          www  ")
renderLine(04 , "w    b          w   ")
renderLine(05 , "  o  b          a   ")  
renderLine(06 , "      b         B  o")
renderLine(07 , "      b   z     A   ")
renderLine(08 , "     b          w   ")
renderLine(09 , "ww  b          www  ")
renderLine(10 , "wwwb          wwwww ")
renderLine(11 , "wwwww   DPq wwwwwwww")
renderLine(12 , "wwwwwwwwwwwwwwwwwwww") 
--               01234567890123456789

	
	else

--               01234567890123456789
renderLine(00 , "wwwwwwwwwwwwwwwwwwww")
renderLine(01 , "wwwww       wwwwwwww")
renderLine(02 , "wwwb          wwwww ")
renderLine(03 , "ww  b    e     www  ")
renderLine(04 , "w    b          w   ")
renderLine(05 , "  o  b          a   ")  
renderLine(06 , "      b   z     a  o")
renderLine(07 , "      b         a   ")
renderLine(08 , "     b          w   ")
renderLine(09 , "ww  b    E     www  ")
renderLine(10 , "wwwb          wwwww ")
renderLine(11 , "wwwww       wwwwwwww")
renderLine(12 , "wwwwwwwwwwwwwwwwwwww") 
--               01234567890123456789

	end
end

apr1()

oxyd_shuffle()

















