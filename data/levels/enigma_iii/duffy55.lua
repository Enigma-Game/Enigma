-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Build Up Speed

levelw=100
levelh=40

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-abyss", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
			set_floor("fl-metal",i-1,line)
		elseif c =="R" then
		   set_stone( "st-rubberband", i-1, line,{strength=15})
			set_floor("fl-hay",i-1,line)
		elseif c =="T" then
		   set_stone( "st-rubberband", i-1, line,{strength=80})
			set_floor("fl-metal",i-1,line)
		elseif c =="S" then
			set_stone( "st-scissors", i-1, line)
			set_floor("fl-hay",i-1,line)
		elseif c =="t" then
			set_stone( "st-scissors", i-1, line)
			set_floor("fl-metal",i-1,line)
		elseif c =="r" then
		   set_stone( "st-rubberband", i-1, line,{name="r1"})
			set_floor("fl-hay",i-1,line)
		elseif c =="&" then
			set_stone( "st-greenbrown", i-1, line)
			set_floor("fl-hay",i-1,line)
		elseif c =="%" then
			set_stone( "st-invisible", i-1, line)
		elseif c =="X" then
			set_stone( "st-death", i-1, line)
		elseif c =="Q" then
			set_stone( "st-death", i-1, line)
			set_floor("fl-metal",i-1,line)
		elseif c =="q" then
			set_stone( "st-death", i-1, line)
			set_floor("fl-hay",i-1,line)
		elseif c == "o" then
			oxyd( i-1, line)
			set_floor("fl-metal",i-1,line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
			set_floor("fl-hay",i-1,line)
                elseif c == "!" then
			abyss(i-1,line)
--			fill_floor("fl-water", i-1,line, 1,1)
		elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5, {name="actor1"})
			set_floor("fl-hay",i-1,line)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
			set_floor("fl-hay",i-1,line)
		elseif c == "G" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
			set_floor("fl-metal",i-1,line)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
			set_floor("fl-hay",i-1,line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="x" then
			set_floor("fl-hay",i-1,line)
		elseif c=="v" then
			set_floor("fl-metal",i-1,line)
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
		elseif c=="H" then
		   set_stone("st-actorimpulse",i-1,line)
		elseif c=="J" then
		   set_item("it-springboard",i-1,line)
		   set_floor("fl-hay",i-1,line)
		elseif c=="m" then
		   set_item("it-magicwand",i-1,line)
                   set_floor("fl-hay",i-1,line)
		elseif c=="s" then
		   set_item("it-ring",i-1,line)
                   set_floor("fl-hay",i-1,line)
	     end
	end	
     end


-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%")
renderLine(01,"%                                                                                              %")
renderLine(02,"%    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxm    %")
renderLine(03,"%    xxxxxRxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxRxx    %")
renderLine(04,"%    xxxxxxxxxxxxRxxxxxxxxxxxxxxxRxxxxxxxxRxxxxxxxxxxxxRxxxxxxxxxRxxxxxxxxRxxxxxxxxxxxxxxxx    %")
renderLine(05,"%    xxxxxxxxxxxxxxxxxxxRxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxRxxxxxxxxx    %")
renderLine(06,"%    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    %")
renderLine(07,"%    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxRxxxxxxxxxxxxRxxxxxxxxxxxxxxxRxxxxxxxxxxxxxxxxxxxx    %")
renderLine(08,"%    xxxxxxRxxxxxxxxxRxxxxxxxxRxxxxxxxxxxxxxxxxxRxxxxxxxxxxxxxxRxxxxxxxxxxxxxxxxxxxxxxRxxxx    %")
renderLine(09,"%    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxRxxxxxxxxxxxxx    %")
renderLine(10,"%    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    %")
renderLine(11,"%    xxxxxxxxx                                                                    xxxxxxxxx    %")
renderLine(12,"%    xxxxxxxxx                        ####################                        xxxxxxxxx    %")
renderLine(13,"%    xxRxxxxxx                        #ovvvvvvvvvvvvvvvvo#                        xxxxxxRxx    %")
renderLine(14,"%    xxxxxxxxx                        #vvvvvvvvvvvvvvvvvv#                        xxxxxxxxx    %")
renderLine(15,"%    xxxxxxRxx                        #vvvvovvvvvvvvovvvv#                        xxxxxxxxx    %")
renderLine(16,"%    xxxxxxxxx                        #vvvvvvvvvvvvvvvvvv#                        xxxxxxxxx    %")
renderLine(17,"%    xxxxxxxxx                        Qvvvvvvvvoovvvvvvvv#                        xxxxxxxxx    %")
renderLine(18,"%    SxrxzxxxJ                        vvvvvvvvvvvvvvvvvTG#                        xxxxxxxxx    %")
renderLine(19,"%    xxxxxxxxx                        Qvvvvvvvvoovvvvvvvv#                        xxxxxxxxx    %")
renderLine(20,"%    xxxxxxxxx                        #vvvvvvvvvvvvvvvvvv#                        xRxxxxxxx    %")
renderLine(21,"%    xxxxRxxxx                        #vvvvovvvvvvvvovvvv#                        xxxxxxxxx    %")
renderLine(22,"%    xxxxxxxxx                        #vvvvvvvvvvvvvvvvvv#                        xxxxxxxxx    %")
renderLine(23,"%    xxxxxxxxx                        #ovvvvvvvvvvvvvvvvo#                        xxxxxxxRx    %")
renderLine(24,"%    xxxxxxxxx                        ####################                        xxxxxxxxx    %")
renderLine(25,"%    xRxxxxxxx                                                                    xxRxxxxxx    %")
renderLine(26,"%    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    %")
renderLine(27,"%    xxxxxRxxxxxxxxxxxRxxxxxxxxxxRxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxRxxxxxxxxxxxxxxxxxxxxxx    %")
renderLine(28,"%    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxRxxxxxxxxxxxxxxRxxxxxxxxxxxxxxxxxxxxxxxRxxxxxxxxx    %")
renderLine(29,"%    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxRxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    %")
renderLine(30,"%    xxxxxxxRxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxRxxxxxxxxxxxxxx    %")
renderLine(31,"%    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxRxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxRxxxx    %")
renderLine(32,"%    xxxxxxxxxxxxxRxxxxxxxRxxxxxxxxxxxxxxRxxxxxxxxxxxxxxxxxxxxxxxxxxxRxxxxxxxxxxxxxxxxxxxxx    %")
renderLine(33,"%    xxRxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxRxxxxxxxxxxRxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    %")
renderLine(34,"%    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxs    %")
renderLine(35,"%                                                                                              %")
renderLine(36,"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%")

oxyd_shuffle()

actor1=enigma.GetNamedObject("actor1")
r1=enigma.GetNamedObject("r1")
AddRubberBand(actor1,r1,15,0)















