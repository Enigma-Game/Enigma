-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Island Paradise

levelw=21
levelh=13

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-water", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="%" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c =="#" then
			set_stone( "st-glass", i-1, line)
		elseif c =="S" then
			set_stone( "st-stoneimpulse", i-1, line)
		elseif c =="T" then
			set_stone( "st-stoneimpulse", i-1, line)
			set_floor("fl-leaves",i-1,line)
		elseif c =="X" then
			set_stone( "st-death", i-1, line)
		elseif c == "o" then
		   oxyd(i-1,line)
		   set_floor("fl-leaves",i-1,line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-1,line, {player=0})
			set_floor("fl-leaves",i-1,line)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
			set_floor("fl-leaves",i-1,line)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
			set_floor("fl-leaves",i-1,line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="x" then
			set_floor("fl-leaves",i-1,line)
		elseif c == "d" then --1-d
			set_floor("fl-gradient",  i-1,  line, {type=1})
		elseif c == "u" then --2-u
			set_floor("fl-gradient",  i-1,  line, {type=2})
		elseif c == "r" then --3-r
			set_floor("fl-gradient",  i-1,  line, {type=3})
		elseif c == "l" then --4-l
			set_floor("fl-gradient",  i-1,  line, {type=4})
		elseif c == "1" then --ur
			set_floor("fl-gradient",  i-1,  line, {type=11})
		elseif c == "3" then --dl
			set_floor("fl-gradient",  i-1,  line, {type=9})
		elseif c == "7" then --dr
			set_floor("fl-gradient",  i-1,  line, {type=12})
		elseif c == "9" then --ul
			set_floor("fl-gradient",  i-1,  line, {type=10})
		elseif c=="s" then
		        set_item("it-seed",i-1,line)
			set_floor("fl-leaves",i-1,line)
	     end
	end	
     end

renderLine(00,"####################")
renderLine(01,"#oxx   +xx+ xx Sx o#")
renderLine(02,"#  +        #+x x  #")
renderLine(03,"#x+x     x+ +x+    #")
renderLine(04,"# +x xxx    x+x x+ #")
renderLine(05,"#S+x xz+ +xx  + +++#")
renderLine(06,"#    x+x x+     x x#")
renderLine(07,"#xxx         +x    #")
renderLine(08,"#+++    x+x xxx    #")
renderLine(09,"# +  +x +xx     +++#")
renderLine(10,"#    xx      x+ xxx#")
renderLine(11,"#oxx     x+ xxx xxo#")
renderLine(12,"####################")

oxyd_shuffle()