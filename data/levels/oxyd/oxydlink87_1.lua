-- NAME, a level for Enigma
-- Copyright (C) YEAR AUTHOR
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 20
levelh = 13

create_world(levelw, levelh)

fill_floor("fl-water",0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
  		if c=="o" then
			oxyd(i-1,line)

                elseif c=="n" then
                   set_floor("fl-normal",i-1,line)
                elseif c=="s" then
                   set_floor("fl-acblack",i-1,line)
                elseif c=="w" then
                   set_floor("fl-acwhite",i-1,line)
		end
	end	
end

renderLine(00 , "                    ")
renderLine(01 , "    o               ")
renderLine(02 , "onwwnnnssnnnnnnnnns ")
renderLine(03 , " n    n         w s ")
renderLine(04 , " nnnnnnssnnwwno w n ")
renderLine(05 , " s n      n  n  n n ")
renderLine(06 , " s n     onssn  n n ")
renderLine(07 , " n nwwnnnnn  nnnn n ")
renderLine(08 , " n o  s   w    n  n ")
renderLine(09 , " n    s   w   on  n ")
renderLine(10 , " nnnnwwnnnnnnnnnwwno")
renderLine(11 , "  o                 ")
renderLine(12 , "                    ")

oxyd_shuffle()

-- Special
set_actor("ac-blackball", 1.5,2.5, {player=0})  
set_actor("ac-whiteball", 2.5,2.5, {player=1})  
set_item("it-yinyang",1,2)
set_item("it-yinyang",2,2)

