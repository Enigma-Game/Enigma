--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)

fill_floor("fl-sand", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock2", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "H" then
			set_stone( "st-grate1", i-1, line)
		end
	end
end

-- Floor:   " "
-- Border:  "#"
-- Oxyd:    "o"

renderLine(  0, "####################")
renderLine(  1, "#                  #")
renderLine(  2, "#        HHH       #")
renderLine(  3, "#       H   H      #")
renderLine(  4, "#      H  H  H     #")
renderLine(  5, "#      H H H H     #")
renderLine(  6, "#      H  H  H     #")
renderLine(  7, "#       H   H      #")
renderLine(  8, "#        HHH       #")
renderLine(  9, "#                  #")
renderLine( 10, "########## #########")
renderLine( 11, "#o                o#")
renderLine( 12, "####################")

set_stone("st-door", 10, 10 , {name="door1", type="h"})
set_stone( "st-shogun-sml", 9, 6)
shogundot3( 10, 5, {target="door1", action="openclose"})
set_stone("st-swap", 6,5)
oxyd_shuffle()

set_actor("ac-blackball", 3.5, 5.5)
