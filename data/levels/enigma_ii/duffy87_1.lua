-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Puppet

levelw=21
levelh=13

create_world( levelw, levelh)

fill_floor("fl-hay", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c =="%" then
			set_stone( "st-greenbrown", i-1, line)
			set_floor("fl-normal",i-1,line)
		elseif c =="A" then
			set_stone( "st-actorimpulse", i-1, line)
		elseif c =="w" then
			set_stone( "st-glass", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
			set_floor("fl-normal",i-1,line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-.5,line+.5, {player=0})
		   set_floor("fl-normal",i-1,line)
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
		elseif c == "a" then --ur
			set_floor("fl-gradient",  i-1,  line, {type=11})
			set_item("it-ring",i-1,line)
		elseif c == "b" then --dl
			set_floor("fl-gradient",  i-1,  line, {type=9})
			set_item("it-ring",i-1,line)
		elseif c == "c" then --dr
			set_floor("fl-gradient",  i-1,  line, {type=12})
			set_item("it-ring",i-1,line)
		elseif c == "`" then --ul
			set_floor("fl-gradient",  i-1,  line, {type=10})
			set_item("it-ring",i-1,line)
		elseif c=="F" then
		   set_floor("fl-normal",i-1,line)
		   set_item("it-flagblack",i-1,line)
		elseif c=="m" then
		   set_floor("fl-normal",i-1,line)
		elseif c=="R" then
		   set_actor("ac-rotor", i-.5,line+.5, {player=1, mouseforce=0, range=30, force=30})
  			set_item("it-trigger", i-1,line, {action="open", target="door1"})
		elseif c=="Q" then
		   set_item("it-wormhole", i-1, line, {strength=0,targetx="10.5",targety="6.5"})
		elseif c=="S" then
		   set_floor("fl-normal",i-1,line)
		   set_item("it-wormhole", i-1, line, {strength=0,targetx="10.5",targety="6.5"})
		elseif c=="s" then
		   set_floor("fl-gradient",  i-1,  line, {type=12})
		   set_item("it-wormhole", i-1, line, {strength=0,targetx="10.5",targety="6.5"})
		elseif c=="q" then
		   set_floor("fl-gradient",  i-1,  line, {type=9})
		   set_item("it-wormhole", i-1, line, {strength=0,targetx="10.5",targety="6.5"})
		elseif c=="T" then
  			set_item("it-trigger", i-1,line, {action="close", target="door1"})
		elseif c=="t" then
		   set_stone("st-door_a",i-1,line,{name="door1"})
	     end
	end	
end

renderLine(00,"%%%%%%%%%%%%%%%%%%%%")
renderLine(01,"%brrrmmmmmmmmmmllla%")
renderLine(02,"%d################d%")
renderLine(03,"%d#  1d3 lllllo#o#d%")
renderLine(04,"%d#d##q#########d#d%")
renderLine(05,"%m# rrrSl#     #d#m%")
renderLine(06,"%z####9###R# # #d#F%")
renderLine(07,"%m#Q#  #Q### ###d#m%")
renderLine(08,"%u# # ##   Q   #d#u%")
renderLine(09,"%u#    t T   #   #u%")
renderLine(10,"%u################u%")
renderLine(11,"%`rrrmmmmmmmmmmlllc%")
renderLine(12,"%%%%%%%%%%%%%%%%%%%%")

oxyd_shuffle()















