-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Under the Radar

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.SlopeForce=18.5

fill_floor("fl-metal", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c =="Q" then
		   set_stone( "st-blue-sand", i-1, line,{name="b1"})
		elseif c == "o" then
			oxyd( i-1, line)
			abyss(i-1,line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-.5,line+.5, {player=0})
			set_floor("fl-gradient",  i-1,  line, {type=4})
		elseif c=="y" then
		   set_actor("ac-whiteball", i-1,line+.5, {player=1})
		elseif c=="A" then
		   set_actor("ac-rotor", i-.5,line+.5, {range=0,force=0,player=1,name="r1"})
			set_floor("fl-gradient",  i-1,  line, {type=3})
		elseif c=="B" then
		   set_actor("ac-rotor", i-.5,line+.5, {range=0,force=0,player=1,name="r2"})
			set_floor("fl-gradient",  i-1,  line, {type=3})
		elseif c=="C" then
		   set_actor("ac-rotor", i-.5,line+.5, {range=0,force=0,player=1,name="r3"})
			set_floor("fl-gradient",  i-1,  line, {type=3})
		elseif c=="D" then
		   set_actor("ac-rotor", i-.5,line+.5, {range=0,force=0,player=1,name="r4"})
			set_floor("fl-gradient",  i-1,  line, {type=3})
		elseif c=="E" then
		   set_actor("ac-rotor", i-.5,line+.5, {range=0,force=0,player=1,name="r5"})
			set_floor("fl-gradient",  i-1,  line, {type=3})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
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
	     end
	end	
end

renderLine(00,"!!!!!!!o!o!o!o!!!!!!")
renderLine(01,"!!!!!!!rrrErrr!!!!!!")
renderLine(02,"!!!!!!9rrrDrrr3!!!!!")
renderLine(03,"!!!!ouu9rrCrr3ddo!!!")
renderLine(04,"!!!!!uuu9rBr3ddd!!!!")
renderLine(05,"!!!!ouuuu9A3ddddo!!!")
renderLine(06,"!!!!!uuuuuQddddd!!!!")
renderLine(07,"!!!!ouuuu7l1ddddo!!!")
renderLine(08,"!!!!!uuu7lll1ddd!!!!")
renderLine(09,"!!!!ouu7lllll1ddo!!!")
renderLine(10,"!!!!!!7llzllll1!!!!!")
renderLine(11,"!!!!!!!lllllll!!!!!!")
renderLine(12,"!!!!!!!o!o!o!o!!!!!!")

oxyd_shuffle()

b1=enigma.GetNamedObject("b1")
r1=enigma.GetNamedObject("r1")
r2=enigma.GetNamedObject("r2")
r3=enigma.GetNamedObject("r3")
r4=enigma.GetNamedObject("r4")
r5=enigma.GetNamedObject("r5")

AddRubberBand(r1,b1,200,1.2)
AddRubberBand(r1,r2,200,1)
AddRubberBand(r2,r3,200,1)
AddRubberBand(r3,r4,200,1)
AddRubberBand(r4,r5,200,1)
















