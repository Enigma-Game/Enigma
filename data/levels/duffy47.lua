-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Cage the Lions

levelw=20
levelh=13

create_world( levelw, levelh)

fill_floor("fl-hay", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c =="X" then
			set_stone( "st-death", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
--			fill_floor("fl-water", i-1,line, 1,1)
                elseif c == "~" then
--			abyss(i-1,line)
			fill_floor("fl-water", i-1,line, 1,1)
		elseif c=="z" then
		        set_actor("ac-blackball", i,line+.5)
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
		elseif c=="T" then
		   set_actor("ac-top", i-.5,line+.5, {player=1, mouseforce=0, range=30, force=30})
		elseif c=="R" then
		   set_actor("ac-rotor", i-.5,line+.5, {player=1, mouseforce=0, range=30, force=30})
		elseif c =="H" then
			set_stone("st-switch", i-1,line, {action="callback", target="funcc1"})
		elseif c =="F" then
		   set_stone("st-door_b",i-1,line,{name="door1"})
		elseif c =="G" then
		   set_stone("st-door_b",i-1,line,{name="door2"})
		elseif c =="a" then
			set_item("it-trigger", i-1,line, {action="openclose", target="door3"})
		elseif c =="b" then
			set_item("it-trigger", i-1,line, {action="openclose", target="door4"})
		elseif c =="c" then
			set_item("it-trigger", i-1,line, {action="openclose", target="door5"})
		elseif c =="e" then
			set_item("it-trigger", i-1,line, {action="openclose", target="door6"})
		elseif c=="A" then
			doorv( i-1,line, {name="door3"})
		elseif c=="B" then
			doorv( i-1,line, {name="door4"})
		elseif c=="C" then
			doorv( i-1,line, {name="door5"})
		elseif c=="D" then
			doorv( i-1,line, {name="door6"})
	     end
	end	
end

function yy1( color, x, y)
       stone = format( "st-%s4", color)
        set_stone( stone, x, y)
     end

renderLine(00,"####################")
renderLine(01,"#RF  3d1         ABo")
renderLine(02,"###  r l         ###")
renderLine(03,"#    9u7  # #  3d1 #")
renderLine(04,"#  ###    #e#  r l #")
renderLine(05,"#   a#    ###  9u7 #")
renderLine(06,"H  ###   z    ###  H")
renderLine(07,"# 3d1  ###    #c   #")
renderLine(08,"# r l  #b#    ###  #")
renderLine(09,"# 9u7  # #  3d1    #")
renderLine(10,"###         r l  ###")
renderLine(11,"oDC         9u7  GR#")
renderLine(12,"####################")

oxyd_shuffle()

function funcc1()
    SendMessage("door1", "openclose")
    SendMessage("door2", "openclose")
end