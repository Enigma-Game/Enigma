-- NAME, a level for Enigma
-- Copyright (C) YEAR AUTHOR
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 20
levelh = 13

Require("levels/lib/libterrain.lua")

create_world(levelw, levelh)

fill_floor("fl-abyss_fake",0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
  		if c=="o" then
			set_item("it-hollow",i-1,line)
			set_floor("fl-gray",i-1,line)
                elseif c=="#" then
                        set_stone("st-marble",i-1,line)
                elseif c==" " then
			set_floor("fl-wood",i-1,line)
                elseif c=="-" then
			set_floor("fl-gray",i-1,line)
                elseif c=="s" then
			set_floor("fl-abyss_fake",i-1,line)
		end
	end	
end

renderLine(00 , "ssssssssssssssssssss")
renderLine(01 , "####################")
renderLine(02 , "#                  #")
renderLine(03 , "#                  #")
renderLine(04 , "#  o            o  #")
renderLine(05 , "#       ----       #")
renderLine(06 , "#       ----       #")
renderLine(07 , "#       ----       #")
renderLine(08 , "#  o            o  #")
renderLine(09 , "#                  #")
renderLine(10 , "#                  #")
renderLine(11 , "####################")
renderLine(12 , "ssssssssssssssssssss")

oxyd_shuffle()

-- Special
set_actor("ac-whiteball-small", 7.5,6.5, {player=0})
set_actor("ac-whiteball-small", 8.5,6.5, {player=0})  
set_actor("ac-whiteball-small", 11.5,6.5, {player=0})  
set_actor("ac-whiteball-small", 12.5,6.5, {player=0})    

draw_ring(2,3,3,3,1)
draw_ring(2,7,3,3,1)
draw_ring(15,3,3,3,1)
draw_ring(15,7,3,3,1)

draw_ring(7,4,6,5,0)


