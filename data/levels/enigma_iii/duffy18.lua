-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Yo-yo

levelw=50
levelh=30

create_world( levelw, levelh)

fill_floor("fl-bluegreen", 0,0,levelw,levelh)

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
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="l" then
			oneway(i-1,line, enigma.WEST)
		elseif c=="u" then
			oneway(i-1,line, enigma.NORTH)
		elseif c=="r" then
			oneway(i-1,line, enigma.EAST)
		elseif c=="d" then
			oneway(i-1,line, enigma.SOUTH)
		elseif c=="z" then
                    set_actor("ac-blackball", i-.5,line+.5)
		elseif c=="y" then
                    set_actor("ac-whiteball", i-.5,line+.5, {player=0})
		elseif c=="H" then
		   hollow(i-1,line)

		elseif c=="g" then
		   draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="G" then
		   draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		   set_item("it-seed", i-1, line)

elseif c=="b" then
yy1( "black",  i-1, line)
elseif c=="w" then
yy1( "white",  i-1, line)
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
renderLine(01,"#        wow       #")
renderLine(02,"#        www  www  #")
renderLine(03,"#  wwww       wow  #")
renderLine(04,"#  wwww       www  #")
renderLine(05,"#  woow            #")
renderLine(06,"#  wwww  www wwww  #")
renderLine(07,"#  wwww  wow woow  #")
renderLine(08,"#        www woow  #")
renderLine(09,"#            wwww  #")
renderLine(10,"#ww   wwwww        #")
renderLine(11,"#ow   wowow        #")
renderLine(12,"####################")

oxyd_shuffle()

local actor1=set_actor("ac-blackball", 9.5,4.5)
local actor2=set_actor("ac-whiteball",  11.5, 4.5)

AddRubberBand(actor1, actor2, 5, 0)














