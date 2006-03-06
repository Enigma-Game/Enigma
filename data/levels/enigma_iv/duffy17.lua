-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Double-Spiral

levelw=50
levelh=50

create_world( levelw, levelh)
enigma.TwoPlayerGame = TRUE

fill_floor("fl-metal", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
                elseif c == "!" then
--			abyss(i-1,line)
			fill_floor("fl-water", i-1,line, 1,1)
		elseif c == "s" then
			set_stone("st-stoneimpulse", i-1,line)
		elseif c == "2" then --1-d
			set_floor("fl-gradient",  i-1,  line, {type=1})
		elseif c == "8" then --2-u
			set_floor("fl-gradient",  i-1,  line, {type=2})
		elseif c == "6" then --3-r
			set_floor("fl-gradient",  i-1,  line, {type=3})
		elseif c == "4" then --4-l
			set_floor("fl-gradient",  i-1,  line, {type=4})
		elseif c == "1" then --ur
			set_floor("fl-gradient",  i-1,  line, {type=11})
		elseif c == "3" then --dl
			set_floor("fl-gradient",  i-1,  line, {type=9})
		elseif c == "7" then --dr
			set_floor("fl-gradient",  i-1,  line, {type=12})
		elseif c == "9" then --ul
			set_floor("fl-gradient",  i-1,  line, {type=10})
		elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5)
		elseif c=="y" then
			set_actor("ac-whiteball", i-.5,line+.5)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
			fill_floor("fl-leaves", i-1,line, 1,1)
		elseif c==":" then
			fill_floor("fl-space", i-1,line, 1,1)
		elseif c=="=" then
			fill_floor("fl-leaves", i-1,line, 1,1)
		elseif c=="S" then
			set_stone( "st-swap", i-1, line)
		elseif c == "R" then
			puzzle(i-1,line,PUZ_0100)
		elseif c == "L" then
			puzzle(i-1,line,PUZ_0001)
		elseif c=="g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="h" then
			doorh(i-1,line,{name="door1"})
		elseif c=="q" then
			doorh(i-1,line,{name="door2"})
		elseif c=="j" then
			doorh(i-1,line,{name="door3"})
		elseif c=="v" then
			doorh(i-1,line,{name="door4"})
		elseif c=="u" then
			oneway(i-1,line, enigma.NORTH)
			fill_floor("fl-inverse", i-1,line, 1,1)
		elseif c=="r" then
			oneway(i-1,line, enigma.EAST)
			fill_floor("fl-inverse", i-1,line, 1,1)
		elseif c=="d" then
			oneway(i-1,line, enigma.SOUTH)
			fill_floor("fl-inverse", i-1,line, 1,1)
		elseif c=="l" then
			oneway(i-1,line, enigma.WEST)
			fill_floor("fl-inverse", i-1,line, 1,1)
		elseif c=="I" then
			fill_floor("fl-inverse", i-1,line, 1,1)
		elseif c=="F" then
			set_stone("st-floppy", i-1, line, {action="openclose", target="door2"})
		elseif c=="U" then
			set_stone("st-floppy", i-1, line, {action="openclose", target="door3"})

		elseif c=="f" then
			set_item("it-floppy", i-1, line)
		elseif c=="K" then
			set_stone("st-key_a", i-1, line, {action="openclose", target="door1"})
		elseif c=="V" then
			set_stone("st-key_a", i-1, line, {action="openclose", target="door4"})
		elseif c=="k" then		
			set_item("it-key_a", i-1 ,line)
		elseif c == "b" then
			yy1( "black",  i-1, line)
		elseif c == "w" then
			yy1( "white",  i-1, line)
		elseif c == "X" then
            		draw_stones("st-yinyang1", { i-1,line}, {0,1},1)
		elseif c=="P" then
			set_item("it-brush",i-1,line)
		elseif c=="p" then
			draw_stones("st-stonebrush", {i-1,line}, {1,0},1)
		end
	end	
end

function yy1( color, x, y)
        stone = format( "st-%s4", color)
        set_stone( stone, x, y)
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"#######################################")
renderLine(01,"#===s=======!!!!===p p   ppppp    p   #")
renderLine(02,"#===s+=+=!!!!!!!+==p ppp p     pp p p #")
renderLine(03,"#===s=+=!!!+!!!!===p p   ppppppp    p #")
renderLine(04,"#===s+=!!!!=!!!!===p p ppp p  Pppppp  #")
renderLine(05,"#==+#!!!!!!=!!!!===p p p   p ppp p  p #")
renderLine(06,"#===!!!!++!!!!!!===p p p ppp p     p  #")
renderLine(07,"#==+!!====!!!!!!+==p           p pp p #")
renderLine(08,"#===!!==+=!!!==!===ppp p pppppp     p #")
renderLine(09,"#===!!==!!!!+==!===  p p       p pp   #")
renderLine(10,"#===!!#!!+!!!!!!===p p  pppppp p p p  #")
renderLine(11,"#==+!!!!!====!!!===p   p       p p    #")
renderLine(12,"###### #############################v##")
renderLine(13,"#                  #o##bbbbbbbbbbbwX K#")
renderLine(14,"#w#w#w#w#w#w#w#w#wb#h##bwwwwwwwwwbw# U#") 
renderLine(15,"#wbXbXbXbXbXbXbXbXb# k#bwbbbbbbbwbw#  #")
renderLine(16,"#wbwbwbwbwbwbwbwbwb#  #bwbwwww#bwbw#y #")
renderLine(17,"#wbwbwbwbwbwbwbwbwb#  #bwbw#  #bwbw#  #") 
renderLine(18,"#wbwbwbwbwbwbwbwbwb#bb#bwbw#RL#bwbw#ww#")
renderLine(19,"#wbwbwbwbwbwbwbwbwb#  #bwbw#  #bwbw#  #")
renderLine(20,"#wbwbwbwbwbwbwbwbwb# z#bwbw#bbbbwbw#  #") 
renderLine(21,"#wXwXwXwXwXwXwXwXwX#  #bwbwwwwwwwbw#f #")
renderLine(22,"##b#b#b#b#b#b#b#bwb#V #bwbbbbbbbbbw##q#")
renderLine(23,"#                  #F Xbwwwwwwwwwww##o#") 
renderLine(24,"######### ###########j#################")
renderLine(25,"#:::::::::::#::::::#IIlIIIII#II#IIIrII#")
renderLine(26,"#:::#::::::::::#:::#d###u##I#IIlIII#II#")
renderLine(27,"#::::::::::::::#:::#III#II###d##II##d##")
renderLine(28,"#222222#:::#:::::::#IIIlIIlIIII#u##III#")
renderLine(29,"#::::::####::::::::##d######IIIrII###d#")
renderLine(30,"#:::::::::4::::::::#IIIIIIIrI###IIlIII#")
renderLine(31,"##::::::::122222222#######d###IlII#III#")
renderLine(32,"#::::::##::::::::::#IIIIIrIlI#u#####ud#")
renderLine(33,"#::::::::::::::##::#d###u#u#IIIrIIIlII#")
renderLine(34,"#IIIIIIIIIIIIIIIIII#IIIrIIIrII####I##I#")
renderLine(35,"#IIIIIIIIIIIIIIIIIIIIII#IIIlIIlIIlIIrI#")
renderLine(36,"#######################################")

oxyd_shuffle()

















