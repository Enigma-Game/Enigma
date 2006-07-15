--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="w" then
	    set_floor( "fl-wood", i-1, line)
            set_stone( "st-wood", i-1, line)
        elseif c == "o" then
	    set_floor( "fl-wood", i-1, line)
            oxyd( i-1, line)
	elseif c == "b" then
	    set_floor( "fl-wood", i-1, line)
	    set_stone( "st-greenbrown_move", i-1, line)
        elseif c == " " then
            set_floor( "fl-abyss", i-1, line)
	elseif c == "=" then
	    set_floor( "fl-wood", i-1, line)
        end
    end
end

-- Abyss:   " "
-- Floor:   "="
-- Wood:    "w"
-- Brownie  "b"
-- Oxyd:    "o"

renderLine(  0, "                    ")
renderLine(  1, "        o           ")
renderLine(  2, "                    ")
renderLine(  3, "  ====         =    ")
renderLine(  4, " ==w=====      ==   ")
renderLine(  5, " =====w==b== =====  ")
renderLine(  6, " ==b=w==w==  ===b=  ")
renderLine(  7, " ===b====w=  ==bo   ")
renderLine(  8, " ===ww=bw   ====b   ")
renderLine(  9, " o==ww==    ======  ")
renderLine( 10, "    ==        ===   ")
renderLine( 11, "          o         ")
renderLine( 12, "                    ")

oxyd_shuffle()

set_actor("ac-blackball", 5.5, 4.5)











