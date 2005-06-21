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
		elseif c == "$" then
			set_stone( "st-death", i-1, line)
		end
	end
end

-- Floor:   " "
-- Border:  "#"
-- Oxyd:    "o"
-- Death:   "$"

renderLine(  0, "####################")
renderLine(  1, "#        #         #")
renderLine(  2, "#######o ########o #")
renderLine(  3, "#        #         #")
renderLine(  4, "#  $#####$ o########")
renderLine(  5, "#  #     #         #")
renderLine(  6, "#  #  o  #######$  #")
renderLine(  7, "#  #  #  #      #  #")
renderLine(  8, "#  #  #  # o#$  #  #")
renderLine(  9, "#  #  #  #   #  #  #")
renderLine( 10, "#  o  #  $#o $  o  #")
renderLine( 11, "#     #      #     #")
renderLine( 12, "####################")

oxyd_shuffle()

set_actor("ac-blackball", 1.5, 1.5)
