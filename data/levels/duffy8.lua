-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Checkerboard

levelw=80
levelh=20

create_world( levelw, levelh)

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
--			fill_floor("fl-water", i-1,line, 1,1)
		elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5)
			set_item("it-yinyang", i-1, line+.5)
			fill_floor("fl-bluegreen", i-1,line, 1,1)
		elseif c=="y" then
			set_actor("ac-whiteball", i-.5,line+.5)
			set_item("it-yinyang", i-1, line+.5)
			fill_floor("fl-bluegreen", i-1,line, 1,1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
			fill_floor("fl-bluegreen", i-1,line, 1,1)
		elseif c=="=" then
			set_stone( "st-wood", i-1, line)
		elseif c=="B" then
			set_stone("st-bombs", i-1,line)
		elseif c=="d" then
			set_stone("st-oneway-s", i-1,line)	
		elseif c=="l" then
			set_stone("st-oneway-w", i-1,line)
		elseif c == "X" then
            		draw_stones("st-yinyang1", { i-1,line}, {0,1},1)
		elseif c=="C" then
		       doorh( i-1,line, {name="door3"})
		elseif c == "b" then
			yy1( "black",  i-1, line)
		elseif c == "w" then
			yy1( "white",  i-1, line)
		elseif c == "x" then
			fill_floor("fl-bluegreen", i-1,line, 1,1)
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

renderLine(00,"####################")
renderLine(01,"ow!x!x!+!x!x!+!x!xbo")
renderLine(02,"#!x!+!x!x!+!x!x!x!x#")
renderLine(03,"#x!x!+!x!+!+!x!+!x!#")
renderLine(04,"#!x!+!+!+!x!+!+!+!x#")
renderLine(05,"#+!xxx!+!x!x!+!xxx!#")
renderLine(06,"#!x!+z+!x!+!x!+y+!+#")
renderLine(07,"#+!xxx!+!x!x!+!xxx!#")
renderLine(08,"#!x!+!+!+!x!+!+!+!x#")
renderLine(09,"#x!x!+!x!+!+!x!+!x!#")
renderLine(10,"#!x!+!x!x!+!x!x!x!x#")
renderLine(11,"ob!x!x!+!x!x!+!x!xwo")
renderLine(12,"####################")

oxyd_shuffle()
