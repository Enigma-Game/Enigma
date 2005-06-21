--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)

fill_floor("fl-bluegreen", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-glass", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "s" then
			set_stone( "st-shogun-s", i-1, line)
		elseif c == "<" then
			oneway( i-1, line, enigma.WEST)
		elseif c == ">" then
			oneway( i-1, line, enigma.EAST)
		elseif c == "v" then
			oneway( i-1, line, enigma.SOUTH)
		elseif c == "^" then
			oneway( i-1, line, enigma.NORTH)
		end
	end
end

-- Floor:   " "
-- Border:  "#"
-- Oxyd:    "o"

renderLine(  0, "####################")
renderLine(  1, "#  #o#        #o#  #")
renderLine(  2, "# s# > ##  ## < #s #")
renderLine(  3, "#  # # ##  ## # #  #")
renderLine(  4, "#  # #  <  >  # #  #")
renderLine(  5, "#  # ## #  # ^# #  #")
renderLine(  6, "#       #  #       #")
renderLine(  7, "#  # #v #  # ## #  #")
renderLine(  8, "#  # #  <  >  # #  #")
renderLine(  9, "#  # # ##  ## # #  #")
renderLine( 10, "# s# # ##  ## # #s #")
renderLine( 11, "#  #o#        #o#  #")
renderLine( 12, "####################")

oxyd_shuffle()

set_stone("st-door", 4, 5, {name="door1", type="h"})
shogundot1( 6, 4, {target="door1", action="openclose"})

set_stone("st-door", 4, 7, {name="door2", type="h"})
shogundot1( 6, 8, {target="door2", action="openclose"})

set_stone("st-door", 15,5, {name="door3", type="h"})
shogundot1( 13, 4, {target="door3", action="openclose"})

set_stone("st-door", 15, 7, {name="door4", type="h"})
shogundot1( 13, 8, {target="door4", action="openclose"})


set_actor("ac-blackball", 10.5, 5.5)
