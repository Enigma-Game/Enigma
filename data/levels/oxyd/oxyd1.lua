-- NAME, a level for Enigma
-- Copyright (C) YEAR AUTHOR
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 20
levelh = 13

create_world(levelw, levelh)

fill_floor("fl-abyss_fake",0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
  		if c=="X" then
			oxyd(i-1,line)
                elseif c=="#" then
                        set_stone("st-brick",i-1,line)
                elseif c==" " then
			set_floor("fl-brick",i-1,line)
                elseif c=="-" then
			set_floor("fl-swamp",i-1,line)
                elseif c=="s" then
			set_floor("fl-abyss_fake",i-1,line)
		end
	end	
end

renderLine(00 , "ssssssssssssssssssss")
renderLine(01 , "####################")
renderLine(02 , "#                  #")
renderLine(03 , "#  --------------  #")
renderLine(04 , "#  -            -  #")
renderLine(05 , "#  - X X    X X -  #")
renderLine(06 , "#  -            -  #")
renderLine(07 , "#  - X X    X X -  #")
renderLine(08 , "#  -            -  #")
renderLine(09 , "#  --------------  #")
renderLine(10 , "#                  #")
renderLine(11 , "####################")
renderLine(12 , "ssssssssssssssssssss")

oxyd_shuffle()

-- Special
set_actor("ac-blackball", 10.5,10.5, {player=0})  
set_actor("ac-rotor", 10,6.5, {})  


