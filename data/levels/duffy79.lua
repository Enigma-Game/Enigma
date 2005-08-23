-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Oxyd Maze

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock1", i-1, line)
		elseif c == "1" then
		   set_stone("st-oxyd", i-1, line,{name="o01",color="0"})
		elseif c == "2" then
		   set_stone("st-oxyd", i-1, line,{name="o11",color="1"})
		elseif c == "3" then
		   set_stone("st-oxyd", i-1, line,{name="o21",color="2"})
		elseif c == "4" then
		   set_stone("st-oxyd", i-1, line,{name="o31",color="3"})
		elseif c == "5" then
		   set_stone("st-oxyd", i-1, line,{name="o41",color="4"})
		elseif c == "6" then
		   set_stone("st-oxyd", i-1, line,{name="o51",color="5"})
		elseif c == "7" then
		   set_stone("st-oxyd", i-1, line,{name="o61",color="6"})
		elseif c == "8" then
		   set_stone("st-oxyd", i-1, line,{name="o71",color="7"})
		elseif c == "9" then
		   set_stone("st-oxyd", i-1, line,{name="o02",color="0"})
		elseif c == "0" then
		   set_stone("st-oxyd", i-1, line,{name="o12",color="1"})
		elseif c == "-" then
		   set_stone("st-oxyd", i-1, line,{name="o22",color="2"})
		elseif c == "=" then
		   set_stone("st-oxyd", i-1, line,{name="o32",color="3"})
		elseif c == "j" then
		   set_stone("st-oxyd", i-1, line,{name="o42",color="4"})
		elseif c == "k" then
		   set_stone("st-oxyd", i-1, line,{name="o52",color="5"})
		elseif c == "l" then
		   set_stone("st-oxyd", i-1, line,{name="o62",color="6"})
		elseif c == "m" then
		   set_stone("st-oxyd", i-1, line,{name="o72",color="7"})
		elseif c == "a" then
		   set_item("it-trigger",i-1,line,{invisible=1,action="trigger",target="o01"})
		elseif c == "b" then
		   set_item("it-trigger",i-1,line,{invisible=1,action="trigger",target="o11"})
		elseif c == "c" then
		   set_item("it-trigger",i-1,line,{invisible=1,action="trigger",target="o21"})
		elseif c == "d" then
		   set_item("it-trigger",i-1,line,{invisible=1,action="trigger",target="o31"})
		elseif c == "e" then
		   set_item("it-trigger",i-1,line,{invisible=1,action="trigger",target="o41"})
		elseif c == "f" then
		   set_item("it-trigger",i-1,line,{invisible=1,action="trigger",target="o51"})
		elseif c == "g" then
		   set_item("it-trigger",i-1,line,{invisible=1,action="trigger",target="o61"})
		elseif c == "h" then
		   set_item("it-trigger",i-1,line,{invisible=1,action="trigger",target="o71"})
		elseif c == "A" then
		   set_item("it-trigger",i-1,line,{invisible=1,action="trigger",target="o02"})
		elseif c == "B" then
		   set_item("it-trigger",i-1,line,{invisible=1,action="trigger",target="o12"})
		elseif c == "C" then
		   set_item("it-trigger",i-1,line,{invisible=1,action="trigger",target="o22"})
		elseif c == "D" then
		   set_item("it-trigger",i-1,line,{invisible=1,action="trigger",target="o32"})
		elseif c == "E" then
		   set_item("it-trigger",i-1,line,{invisible=1,action="trigger",target="o42"})
		elseif c == "F" then
		   set_item("it-trigger",i-1,line,{invisible=1,action="trigger",target="o52"})
		elseif c == "G" then
		   set_item("it-trigger",i-1,line,{invisible=1,action="trigger",target="o62"})
		elseif c == "H" then
		   set_item("it-trigger",i-1,line,{invisible=1,action="trigger",target="o72"})
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball",i-1,line+.5, {player=0})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
	     end
	end	
end

renderLine(00,"12################56")
renderLine(01,"34#gg  a   ff    #78")
renderLine(02,"###bbc a a EE   d###")
renderLine(03,"#  b c   aGGG#EEddh#")
renderLine(04,"#e####  aaaaaDcccBh#")
renderLine(05,"#e#CCHHHa    D cBB #")
renderLine(06,"#eGGCHHHH zffDDcBB #")
renderLine(07,"#HHGG  GffffEDDcBB #")
renderLine(08,"#   GGGG  E E DDB  #")
renderLine(09,"#      A AAAEEE#   #")
renderLine(10,"###FFF   AAAA    ###")
renderLine(11,"90#FFFBB AFFA    #jk")
renderLine(12,"-=################lm")