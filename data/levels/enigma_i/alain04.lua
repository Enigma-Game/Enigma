-- A meditation level for Enigma
-- Copyright (C) 2006 Alain Busser
-- Licensed under GPL v2.0 or above 

levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "a"
draw_border("st-rock1")
fill_floor("fl-leaves", 0,0, level_width,level_height)
enigma.BumperForce=600
function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-glass1", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
            elseif c == "a" then
			set_item("it-wormhole",i-1,line, {targetx=" 13.5",targety="10+random(10)/10",strength=0,range=0})          
		elseif c == "b" then
			set_item("it-wormhole",i-1,line, {targetx=" 4.5",targety="3+random(10)/10",strength=0,range=0})          
            elseif c == "c" then
			set_item("it-wormhole",i-1,line, {targetx=" 16.5",targety="4+random(10)/10",strength=0,range=0})          
		elseif c == "d" then
			set_item("it-wormhole",i-1,line, {targetx=" 6.5",targety="10+random(10)/10",strength=0,range=0})          
            elseif c == "e" then
			set_item("it-wormhole",i-1,line, {targetx=" 13.5",targety="3+random(10)/10",strength=0,range=0})          
		elseif c == "f" then
			set_item("it-wormhole",i-1,line, {targetx=" 4.5",targety="10+random(10)/10",strength=0,range=0})          
            elseif c == "g" then
			set_item("it-wormhole",i-1,line, {targetx=" 16.5",targety="9+random(10)/10",strength=0,range=0})          
		elseif c == "h" then
			set_item("it-wormhole",i-1,line, {targetx=" 6.5",targety="3+random(10)/10",strength=0,range=0})          
		elseif c == "i" then
			set_item("it-wormhole",i-1,line, {targetx=" 13.5",targety="10+random(10)/10",strength=0,range=0})          
		elseif c == "j" then
			set_item("it-wormhole",i-1,line, {targetx=" 4.5",targety="3+random(10)/10",strength=0,range=0})          
            elseif c == "k" then
			set_item("it-wormhole",i-1,line, {targetx=" 16.5",targety="4+random(10)/10",strength=0,range=0})          
		elseif c == "l" then
			set_item("it-wormhole",i-1,line, {targetx=" 6.5",targety="10+random(10)/10",strength=0,range=0})          
            elseif c == "m" then
			set_item("it-wormhole",i-1,line, {targetx=" 13.5",targety="3+random(10)/10",strength=0,range=0})          
		elseif c == "n" then
			set_item("it-wormhole",i-1,line, {targetx=" 4.5",targety="10+random(10)/10",strength=0,range=0})          
            elseif c == "p" then
			set_item("it-wormhole",i-1,line, {targetx=" 16.5",targety="9+random(10)/10",strength=0,range=0})          
		elseif c == "q" then
			set_item("it-wormhole",i-1,line, {targetx=" 6.5",targety="3+random(10)/10",strength=0,range=0})          
  		elseif c=="z" then
			set_actor("ac-whiteball-small", i-.5,line+.5, {player=0})
		elseif c=="+" then
			set_item("it-hollow", i-1, line)
		elseif c=="*" then
			set_stone("st-actorimpulse_invisible", i-1,line)
		end
	end	
end


--               01234567890123456789
renderLine(00 , "                    ")
renderLine(01 , " ****************** ")
renderLine(02 , " *a a*b b**c c*d d* ")
renderLine(03 , " *m+ * zn**pz * +q* ")
renderLine(04 , " *a a*b b**c c*d d* ")
renderLine(05 , " ****************** ")
renderLine(06 , " ################## ")  
renderLine(07 , " ****************** ")
renderLine(08 , " *e e*f f**g g*h h* ")
renderLine(09 , " *i+ * zj**kz * +l* ")
renderLine(10 , " *e e*f f**g g*h h* ")
renderLine(11 , " ****************** ") 
--               01234567890123456789

oxyd_shuffle()





















