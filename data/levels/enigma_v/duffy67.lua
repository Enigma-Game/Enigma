-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Anticipation

levelw=20
levelh=13

create_world( levelw, levelh)
enigma.SlopeForce=20

fill_floor("fl-metal", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock6", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
                elseif c == " " then
			abyss(i-1,line)
		elseif c=="w" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {player=0,mouseforce=1})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="H" then
		   set_item("it-hollow",i-1,line)
		elseif c=="A" then
		   set_item("it-wormhole", i-1, line, {strength=3,targetx="2.5",targety="1.5", name="ww1"})
		elseif c=="B" then
		   set_item("it-wormhole", i-1, line, {strength=3,targetx="9.5",targety="11.5", name="ww2"})
		elseif c=="C" then
		   set_item("it-wormhole", i-1, line, {strength=3,targetx="18.5",targety="4.5", name="ww3"})
		elseif c=="D" then
		   set_item("it-wormhole", i-1, line, {strength=3,targetx="13.5",targety="2.5", name="ww4"})
		elseif c=="E" then
		   set_item("it-wormhole", i-1, line, {strength=3,targetx="13.5",targety="7.5", name="ww5"})
		elseif c=="F" then
		   set_item("it-wormhole", i-1, line, {strength=3,targetx="2.5",targety="7.5", name="ww6"})
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
		elseif c=="R" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {player=0,mouseforce=1})
		        set_floor("fl-gradient",  i-1,  line, {type=3})
		elseif c=="L" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {player=0,mouseforce=1})
		        set_floor("fl-gradient",  i-1,  line, {type=4})
		elseif c=="U" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {player=0,mouseforce=1})
		        set_floor("fl-gradient",  i-1,  line, {type=2})
		elseif c=="S" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {player=0,mouseforce=1})
		        set_floor("fl-gradient",  i-1,  line, {type=1})
	     end
	end	
end

renderLine(00,"########### ###     ")
renderLine(01,"#HRrrrrrr1# #H# ####")
renderLine(02,"#########d# #S# #D1#")
renderLine(03,"        #B# #E# ##u#")
renderLine(04," ###    ### ###  #U#")
renderLine(05," #A#             #H#")
renderLine(06," #u# ########### ###")
renderLine(07," #U# #FllllllLH#    ")
renderLine(08," #H# ########### ###")
renderLine(09," ###             #C#")
renderLine(10,"       ###########u#")
renderLine(11,"       #HRrrrrrrrr7#")
renderLine(12,"       #############")













