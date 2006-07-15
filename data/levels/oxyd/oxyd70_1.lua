-- NAME, a level for Enigma
-- Copyright (C) YEAR AUTHOR
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 20
levelh = 25

Require("levels/lib/libpuzzle.lua")

create_world(levelw, levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
  		if c=="o" then
			set_item("it-hollow",i-1,line)
			set_floor("fl-brick",i-1,line)
                elseif c=="#" then
                        set_stone("st-rock2",i-1,line)
                elseif c=="H" then
                        set_stone("st-rock2_hole",i-1,line)
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
renderLine(01 , "ssssssssssssssssssss")
renderLine(02 , "####################")
renderLine(03 , "#                  #")
renderLine(04 , "#                  #")
renderLine(05 , "#     da da da     #")
renderLine(06 , "#     cb cb cb     #")
renderLine(07 , "#       o  o       #")
renderLine(08 , "#     da da da     #")
renderLine(09 , "#     cb cb cb     #")
renderLine(10 , "#                  #")
renderLine(11 , "#                  #")
renderLine(12 , "####H##########H####")
renderLine(13 , "#                  #")
renderLine(14 , "#                  #")
renderLine(15 , "#     da da da     #")
renderLine(16 , "#     cb cb cb     #")
renderLine(17 , "#       o  o       #")
renderLine(18 , "#     da da da     #")
renderLine(19 , "#     cb cb cb     #")
renderLine(20 , "#                  #")
renderLine(21 , "#                  #")
renderLine(22 , "####################")
renderLine(23 , "ssssssssssssssssssss")
renderLine(24 , "ssssssssssssssssssss")

-- Special
set_actor("ac-whiteball-small", 10,4.5, {player=0})
set_actor("ac-whiteball-small", 10,10.5, {player=0})  

set_actor("ac-whiteball-small", 10,14.5, {player=0})
set_actor("ac-whiteball-small", 10,20.5, {player=0})  

puzzle({{0,0,1,0,0,1,0,0},{0,0,1,0,0,1,0,0},{1,1,1,1,1,1,1,1},{0,0,1,0,0,1,0,0},{0,0,1,0,0,1,0,0}},6,5,"red","yes")
puzzle({{0,0,1,0,0,1,0,0},{0,0,1,0,0,1,0,0},{1,1,1,1,1,1,1,1},{0,0,1,0,0,1,0,0},{0,0,1,0,0,1,0,0}},6,15,"red","no")

