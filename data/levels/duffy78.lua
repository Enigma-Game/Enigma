-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Stealing from Thieves

levelw=60
levelh=15

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-hay", 0,0,levelw,levelh)

enigma.SlopeForce=100

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c =="S" then
			set_stone( "st-thief", i-1, line)
			set_floor("fl-gradient",  i-1,  line, {type=1})
		elseif c =="T" then
			set_stone( "st-thief", i-1, line)
			set_floor("fl-gradient",  i-1,  line, {type=2})
		elseif c =="X" then
			set_stone( "st-death", i-1, line)
		elseif c == "o" then
		   oxyd(i-1,line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
					   set_floor("fl-water",i-1,line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-1,line+.5, {player=0})
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
		elseif c == "D" then --1-d
			set_floor("fl-gradient",  i-1,  line, {type=1})
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c == "U" then --2-u
			set_floor("fl-gradient",  i-1,  line, {type=2})
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="c" then
			set_item("it-coin1", i-1,line)
		elseif c=="C" then
			set_stone("st-coinslot", i-1,line, {action="openclose", target="door1"})
		elseif c=="A" then
		   doorv( i-1,line, {name="door1"})
	     end
	end	
     end

renderLine(00,"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
renderLine(01,"!###############!!!!!!!!!!!!!!!!!!!!!!!!!!!###############!")
renderLine(02,"!#            cTTTTTTTTTTTTTTTTTTTTTTTTTTTTT             #!")
renderLine(03,"!# ####        UuuuuuuuuuuuuuuuuuuuuuuuuuuuU        #### #!")
renderLine(04,"!#    #        TTTTTTTTTTTTTTTTTTTTTTTTTTTTT        #    #!")
renderLine(05,"!###  #        #!!!!!!!!!!!!!!!!!!!!!!!!!!!#        #  ###!")
renderLine(06,"!#C   o    z   #!!!!!!!!!!!!!!!!!!!!!!!!!!!#        *  Ao#!")
renderLine(07,"!###  #        #!!!!!!!!!!!!!!!!!!!!!!!!!!!#        #  ###!")
renderLine(08,"!#    #        SSSSSSSSSSSSSSSSSSSSSSSSSSSSS        #    #!")
renderLine(09,"!# ####        DdddddddddddddddddddddddddddD        #### #!")
renderLine(10,"!#            cSSSSSSSSSSSSSSSSSSSSSSSSSSSSS             #!")
renderLine(11,"!###############!!!!!!!!!!!!!!!!!!!!!!!!!!!###############!")
renderLine(12,"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")

oxyd_shuffle()
