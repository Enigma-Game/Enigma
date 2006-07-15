--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 25

create_world( levelw, levelh)

fill_floor("fl-water", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_floor( "fl-wood", i-1, line)
			set_stone( "st-glass", i-1, line)
		elseif c == "o" then
			set_floor( "fl-wood", i-1, line)
			oxyd( i-1, line)
		elseif c == "=" then
			set_floor( "fl-wood", i-1, line)
		end
	end
end

-- Floor:   "="
-- Border:  "#"
-- Oxyd:    "o"

renderLine(  0, "#######             ")
renderLine(  1, "o================== ")
renderLine(  2, "#=##### =    =   o= ")
renderLine(  3, " =      =    =    = ")
renderLine(  4, " =      =    =    = ")
renderLine(  5, " =      =o   =    = ")
renderLine(  6, " = ================ ")
renderLine(  7, " = =    =      =  = ")
renderLine(  8, " = =    =      =  = ")
renderLine(  9, " = =    =      =  = ")
renderLine( 10, " = =    =      =  = ")
renderLine( 11, " = =============  = ")
renderLine( 12, "#=#= ==   =    =  = ")
renderLine( 13, "#=#= =    =    =  = ")
renderLine( 14, "#=#= =    =    =  = ")
renderLine( 15, "#=#= =    =    =  = ")
renderLine( 16, "#=#= =    ======= = ")
renderLine( 17, "#=#= =    =  = =  = ")
renderLine( 18, "#=#= ========= =  = ")
renderLine( 19, "#=#o      =  =    = ")
renderLine( 20, "#=######  =  ====== ")
renderLine( 21, "#======#  ==== =  = ")
renderLine( 22, "#o######  =    = o= ")
renderLine( 23, "          ========= ")
renderLine( 24, "                    ")

oxyd_shuffle()

oneway( 1, 2, enigma.SOUTH)
set_stone( "st-wood", 8,11)
set_stone( "st-wood", 8,18)
set_stone( "st-wood",11,18)

set_stone( "st-shogun-l", 4,11)
set_stone( "st-shogun-s",17, 6)
set_stone( "st-shogun-m", 5,14)

set_stone( "st-shogun-sml",15,20)

set_stone( "st-door", 6,1, {name="door1", type="v"})
shogundot3( 8,3, {action="openclose", target="door1"})

set_stone( "st-door", 1,12, {name="door2", type="h"})
shogundot3( 3,11, {action="openclose", target="door2"})


set_actor("ac-blackball", 5.5, 18.5)











