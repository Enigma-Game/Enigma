-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Ball and Chain

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
abyss(i-1,line)
elseif c=="B" then
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

renderLine(00,"##o#o######o########")
renderLine(01,"# BbBbbbbb Bb      #")
renderLine(02,"# bb     b bb bbbb #")
renderLine(03,"#  b bbb b  b b    #")
renderLine(04,"#b b bob b bb b bbb#")
renderLine(05,"#b b  Bb   b  b b Bo")
renderLine(06,"#b bbbbb   bBb   b #")
renderLine(07,"#b b       bob b b #")
renderLine(08,"#b b bbbb  bbb b b #")
renderLine(09,"#b b   bb b    b b #")
renderLine(10,"#   bb b  b b bb b #")
renderLine(11,"# b    bb   bBbB   #")
renderLine(12,"#############o#o####")

oxyd_shuffle()

local actor1=set_actor("ac-blackball", 9.5,6.5)

local actor2=set_actor("ac-whiteball",  9.5, 9.5)

AddRubberBand(actor1, actor2, 15, 0)

enigma.ConserveLevel=FALSE















