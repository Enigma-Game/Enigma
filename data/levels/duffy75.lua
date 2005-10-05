-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Bishop

levelw=60
levelh=40

create_world( levelw, levelh)

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-invisible", i-1, line)
			abyss(i-1,line)
		elseif c =="X" then
			set_stone( "st-invisible", i-1, line)
		   set_floor("fl-springboard",i-1,line)
		elseif c =="w" then
			set_stone( "st-glass", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		   set_floor("fl-springboard",i-1,line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == " " then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-.5,line+.5, {player=0})
		   set_floor("fl-springboard",i-1,line)
		   set_item("it-spring1",i-1,line)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="x" then
		   set_floor("fl-springboard",i-1,line)
	     end
	end	
end

renderLine(00,"#####XXXX####XXXX####XX##XX##XX##XX##XX##XXXX####XXXX#####")
renderLine(01,"#oxxx    xxxx    xx  xx  xx  xx  xx  xxxx    xxxx    xxxo#")
renderLine(02,"#xxxx    xxxx    xxxx  xx  xx  xx  xx  xx    xxxx    xxxx#")
renderLine(03,"#xxxx    xxxx    xxxx  xx  xx  xx  xx  xx    xxxx    xxxx#")
renderLine(04,"#xxxx    xxxx    xx  xx  xx  xx  xx  xxxx    xxxx    xxxx#")
renderLine(05,"X    xxxx    xxxx    xx  xx  xx  xx  xx  xxxx    xxxx    X")
renderLine(06,"X    xxxx    xxxx  xx  xx  xx  xx  xx    xxxx    xxxx    X")
renderLine(07,"X    xxxx    xxxx  xx  xx  xx  xx  xx    xxxx    xxxx    X")
renderLine(08,"X    xxxx    xxxx    xx  xx  xx  xx  xx  xxxx    xxxx    X")
renderLine(09,"#xxxx    xxxx    xx  xx  xx  xx  xx  xxxx    xxxx    xxxx#")
renderLine(10,"#xxxx    xxxx    xxxx  xx  xx  xx  xxxxxx    xxxx    xxxx#")
renderLine(11,"#xxxx    xxxx    xxxx  xx  xx  xx  xx xxx    xxxx    xxxx#")
renderLine(12,"X   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx  #")
renderLine(13,"X   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx  #")
renderLine(14,"#xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxX")
renderLine(15,"#xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxX")
renderLine(16,"#xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxX")
renderLine(17,"X   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx  #")
renderLine(18,"X   xxx   xxx   xxx   xxx   xzx   xxx   xxx   xxx   xxx  #")
renderLine(19,"X   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx  #")
renderLine(20,"#xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxX")
renderLine(21,"#xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxX")
renderLine(22,"#xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxX")
renderLine(23,"X   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx  #")
renderLine(24,"X   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx   xxx  #")
renderLine(25,"#xxxx    xxxx    xx  xx  xx  xx  xx  xxxx    xxxx    xxxx#")
renderLine(26,"#xxxx    xxxx    xx  xx  xx  xx  xx  xxxx    xxxx    xxxx#")
renderLine(27,"#xxxx    xxxx    xxxx  xx  xx  xx  xx  xx    xxxx    xxxx#")
renderLine(28,"X    xxxx    xxxx  xx  xx  xx  xx  xx    xxxx    xxxx    X")
renderLine(29,"X    xxxx    xxxx    xx  xx  xx  xx  xx  xxxx    xxxx    X")
renderLine(30,"X    xxxx    xxxx    xx  xx  xx  xx  xx  xxxx    xxxx    X")
renderLine(31,"X    xxxx    xxxx  xx  xx  xx  xx  xx    xxxx    xxxx    X")
renderLine(32,"#xxxx    xxxx    xxxx  xx  xx  xx  xx  xx    xxxx    xxxx#")
renderLine(33,"#xxxx    xxxx    xx  xx  xx  xx  xx  xxxx    xxxx    xxxx#")
renderLine(34,"#xxxx    xxxx    xx  xx  xx  xx  xx  xxxx    xxxx    xxxx#")
renderLine(35,"#oxxx    xxxx    xxxx  xx  xx  xx  xx  xx    xxxx    xxxo#")
renderLine(36,"#####XXXX####XXXX##XX##XX##XX##XX##XX####XXXX####XXXX#####")

oxyd_shuffle()
