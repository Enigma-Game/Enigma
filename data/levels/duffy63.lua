-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Wildfire

levelw=40
levelh=13

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-wood", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c =="%" then
			set_stone( "st-greenbrown", i-1, line)
			set_floor("fl-normal",i-1,line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
--			fill_floor("fl-water", i-1,line, 1,1)
		   set_item("it-burnable_fireproof",i-1,line)
		elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5)
		   set_item("it-burnable",i-1,line)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		   set_item("it-burnable",i-1,line)
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
		elseif c=="X" then
		   set_item("it-extinguisher", i-1,line, {load=2})
		elseif c==" " then
		   set_item("it-burnable",i-1,line)
		elseif c=="F" then
		   set_item("it-burnable_ignited",i-1,line)
		elseif c=="a" then
		   set_item("it-trigger",i-1,line,{action="callback",target="funcc1"})
		elseif c=="A" then
		   set_stone("st-door_b",i-1,line,{name="door1"})
		   set_item("it-burnable",i-1,line)
		elseif c=="B" then
		   set_stone("st-door_b",i-1,line,{name="door2"})
		   set_item("it-burnable",i-1,line)
		elseif c=="C" then
		   set_stone("st-door_b",i-1,line,{name="door3"})
		   set_item("it-burnable",i-1,line)
	     end
	end	
     end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"################################a####o#")
renderLine(01,"#XXXXXXXXXX                    ###  #!#")
renderLine(02,"#                               #   #!#")
renderLine(03,"#                               #     #")
renderLine(04,"#                               #     #")
renderLine(05,"#                               A ++  #")
renderLine(06,"#  #d#####                      B ++  #")
renderLine(07,"#  #3rrr3#                      C ++  #")
renderLine(08,"#  #%%%%d#                      #     #")
renderLine(09,"#  #1l7%d#                      #     #")
renderLine(10,"#  #d%u%d#                      #   #!#")
renderLine(11,"#FF#z%7l1#                      #   #!#")
renderLine(12,"#####################################o#")

oxyd_shuffle()

f1=0
tt=0

function funcc1()
    SendMessage("door1","openclose")
    SendMessage("door2","openclose")
    SendMessage("door3","openclose")
    f1=1
end

function timer_callback2()
   if f1==1 then
      tt=tt+1
      if tt==11 then
	 enigma.KillStone(32,5)
	 enigma.KillStone(32,6)
	 enigma.KillStone(32,7)
	 f1=0
      end
   end
end

ferry_x = 1
ferry_y = 0
ferry_width = 1
ferry_height = 1
ferry_end = 34

function move_ferry()
    set_stone("st-blue-sand",ferry_x+ferry_width-2,ferry_y)
    ferry_x = ferry_x + 1
end

function timer_callback()
    if ferry_x < ferry_end then
        move_ferry()
    end
end

set_stone( "st-timer", 39, 0,
           {target="timer_callback", action="callback",
            name="timer", interval=2.3} )

set_stone( "st-timer", 39, 1,
           {target="timer_callback2", action="callback",
            name="timer", interval=0.1} )
