-- NAME, a level for Enigma
-- Copyright (C) YEAR AUTHOR
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 20
levelh = 13

Require("levels/privat/libpuzzle.lua")

create_world(levelw, levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
  		if c=="o" then
			set_item("it-hollow",i-1,line)
			set_floor("fl-brick",i-1,line)
                elseif c=="#" then
                        set_stone("st-brick",i-1,line)
                elseif c==" " then
			set_floor("fl-brick",i-1,line)

                elseif c=="a" then
                        set_stone("st-bigbrick-sw",i-1,line)
                elseif c=="b" then
                        set_stone("st-bigbrick-nw",i-1,line)
                elseif c=="c" then
                        set_stone("st-bigbrick-ne",i-1,line)
                elseif c=="d" then
                        set_stone("st-bigbrick-es",i-1,line)

                elseif c=="s" then
			set_floor("fl-abyss_fake",i-1,line)
		end
	end	
end

renderLine(00 , "ssssssssssssssssssss")
renderLine(01 , "####################")
renderLine(02 , "#                  #")
renderLine(03 , "#                  #")
renderLine(04 , "#     da da da     #")
renderLine(05 , "#     cb cb cb     #")
renderLine(06 , "#       o  o       #")
renderLine(07 , "#     da da da     #")
renderLine(08 , "#     cb cb cb     #")
renderLine(09 , "#                  #")
renderLine(10 , "#                  #")
renderLine(11 , "####################")
renderLine(12 , "ssssssssssssssssssss")

-- Special
set_actor("ac-whiteball-small", 10,3.5, {player=0})
set_actor("ac-whiteball-small", 10,9.5, {player=0})  

puzzle({{0,0,1,0,0,1,0,0},{0,0,1,0,0,1,0,0},{1,1,1,1,1,1,1,1},{0,0,1,0,0,1,0,0},{0,0,1,0,0,1,0,0}},6,4,"red","yes")

