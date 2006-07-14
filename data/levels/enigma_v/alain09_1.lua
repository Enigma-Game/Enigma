-- Send me the keys !, a level for Enigma
-- Copyright (C) 2006 Alain Busser
-- Licensed under GPL v2.0 or above 

levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "a"
draw_border("st-blackballs")
fill_floor("fl-black", 0,0, level_width,level_height)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone("st-blackballs", i-1, line)
		elseif c=="G" then
			set_floor("fl-space",i-1,line)
			set_stone("st-glass",i-1,line)
		elseif c=="s" then
			set_floor("fl-mortar",i-1,line)
		elseif c=="D" then
			document(i-1,line,"ALL the keys !!!")
		elseif c == "o" then
			oxyd( i-1, line)
  		elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5, {name="ac0"})
			set_item("it-yinyang", i-1, line)
		elseif c=="y" then
			set_actor("ac-whiteball", i-.5,line+.5, {name="ac1"})
			set_item("it-yinyang", i-1, line)
		elseif c=="a" then
            	set_stone("st-key_a", i-1,line, {action="openclose", target="door1"})
		elseif c=="b" then
			set_stone("st-key_a",i-1,line,{action="openclose", target="door2"})
		elseif c=="c" then
			set_stone("st-key_a",i-1,line,{action="openclose", target="door3"})
		elseif c=="d" then
			set_stone("st-key_a",i-1,line,{action="openclose", target="door4"})
		elseif c=="+" then
			set_stone("st-grate1", i-1,line)
		elseif c=="w" then
			set_stone("st-wood", i-1,line)
		elseif c=="f" then
			doorh(i-1,line, {name="door1"})
		elseif c=="g" then
			doorh(i-1,line, {name="door2"})
		elseif c=="h" then
			doorh(i-1,line, {name="door3"})
		elseif c=="i" then
			doorh(i-1,line, {name="door4"})
            elseif c =="U" then
			set_item("it-key_a",i-1,line)
			set_stone("st-wood",i-1,line)
            elseif c =="V" then
			set_item("it-key_a",i-1,line)
     	   	elseif c =="L" then
     	       	set_stone("st-mail-w", i-1, line)
     	   	elseif c =="R" then
     	       	set_stone("st-mail-e", i-1, line)
      	elseif c =="H" then
            	set_item("it-pipe-h",i-1,line)
        	elseif c =="I" then
            	set_item("it-pipe-v",i-1,line)
        	elseif c =="1" then
            	set_item("it-pipe-ne",i-1,line)
        	elseif c =="3" then
            	set_item("it-pipe-sw",i-1,line)
		elseif c=="B" then
			set_item("it-bag",i-1,line)
		end
	end	
end


--               01234567890123456789
renderLine(00 , "                    ")
renderLine(01 , " ssssssRGGG  HIH1H3 ")
renderLine(02 , " ++++++  #G   HHH   ")
renderLine(03 , " + w w+  #GG        ")
renderLine(04 , " +w U +  #     ab   ")
renderLine(05 , " + UVw+  GGGL  cd   ")  
renderLine(06 , " +w U +  #          ")
renderLine(07 , " + w w+  #sss#f#sss ")
renderLine(08 , " +w w +  #s s#g#sys ")
renderLine(09 , " ++++++  #sss#h#sss ")
renderLine(10 , "    z    #####i#### ")
renderLine(11 , "    D    #o       o ")
renderLine(12 , "                    ") 
--               01234567890123456789

oxyd_shuffle()


















