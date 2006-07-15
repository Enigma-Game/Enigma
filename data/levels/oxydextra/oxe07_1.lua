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
	elseif c == "#" then
	    set_floor( "fl-wood", i-1, line)
	    set_stone( "st-greenbrown", i-1, line)
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

renderLine(  0, "####################")
renderLine(  1, "#==================#")
renderLine(  2, "#==================#")
renderLine(  3, "#==================#")
renderLine(  4, "#==     ====     ==#")
renderLine(  5, "#=  www  ==  www  =#")
renderLine(  6, "#= w===w == wo=ow =#")
renderLine(  7, "#=  www  ==  www  =#")
renderLine(  8, "#==     ====     ==#")
renderLine(  9, "#==================#")
renderLine( 10, "#==================#")
renderLine( 11, "#==================#")
renderLine( 12, "####################")

oxyd_shuffle()

set_actor("ac-blackball", 5.5, 6.5)











