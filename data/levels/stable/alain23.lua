-- pool with a pole, a meditation level for Enigma
-- Copyright (C) 2006 Alain Busser
-- Licensed under GPL v2.0 or above 

levelw = 20
levelh = 13

create_world(levelw, levelh)

draw_border("st-actorimpulse")

fill_floor("fl-leaves", 0,0, level_width,level_height)

enigma.BumperForce=600
enigma.SlopeForce=8

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone("st-white4", i-1,line)
		elseif c == "o" then
			set_item("it-hollow", i-1, line)
        	elseif c == "1" then
            	        set_floor("fl-gradient",  i-1,  line, {type=21})
        	elseif c == "2" then
            	        set_floor("fl-gradient",  i-1,  line, {type=22})
        	elseif c == "3" then
            	        set_floor("fl-gradient",  i-1,  line, {type=23})
        	elseif c == "4" then
            	        set_floor("fl-gradient",  i-1,  line, {type=24})
		elseif c=="*" then
			set_actor("ac-whiteball-small", i-0.5, line+0.5, {player=1, mouseforce=0})
		elseif c=="z" then
			set_actor("ac-blackball", i-0.5, line+0.5, {player=0, mouseforce=1})
		end
	end	
end

if difficult then
 --               01234567890123456789
 renderLine(00 , "                    ")
 renderLine(01 , " 311111111111111114 ")
 renderLine(02 , " 31111o1o11o1o11114 ")
 renderLine(03 , " 331111111111111144 ")
 renderLine(04 , " 333111111111111444 ")
 renderLine(05 , " 3333 * * * * *4444 ")
 renderLine(06 , " 3o33     z    44o4 ")  
 renderLine(07 , " 3333* * * * * 4444 ")
 renderLine(08 , " 333222222222222444 ")
 renderLine(09 , " 332222222222222244 ")
 renderLine(10 , " 32222o2o22o2o22224 ")
 renderLine(11 , " 322222222222222224 ") 
 --               01234567890123456789
elseif not difficult then
 --               01234567890123456789
 renderLine(00 , "                    ")
 renderLine(01 , " 311111111111111114 ")
 renderLine(02 , " 311111111111111114 ")
 renderLine(03 , " 33111o1o11o1o11144 ")
 renderLine(04 , " 333111111111111444 ")
 renderLine(05 , " 3333 * * * * *4444 ")
 renderLine(06 , " 33o3     z    4o44 ")  
 renderLine(07 , " 3333* * * * * 4444 ")
 renderLine(08 , " 333222222222222444 ")
 renderLine(09 , " 33222o2o22o2o22244 ")
 renderLine(10 , " 322222222222222224 ")
 renderLine(11 , " 322222222222222224 ") 
 --               01234567890123456789
end

draw_border("st-white4",4,4,12,5)


















