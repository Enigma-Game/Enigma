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
        if c =="#" then
	    set_floor( "fl-rough", i-1, line)
            set_stone( "st-fakeoxyd", i-1, line)
        elseif c == "o" then
	    set_floor( "fl-rough", i-1, line)
            oxyd( i-1, line)
        elseif c == " " then
            set_floor( "fl-abyss", i-1, line)
	elseif c == "=" then
	    set_floor( "fl-rough", i-1, line)
        end
    end
end

-- Abyss:   " "
-- Floor:   "="
-- Border:  "#"
-- Oxyd:    "o"

renderLine(  0, "######o########o####")
renderLine(  1, "#======= ==========#")
renderLine(  2, "#==   == ==========#")
renderLine(  3, "#== =      =    o==#")
renderLine(  4, "#== = ========== ==#")
renderLine(  5, "#== = ========== ==#")
renderLine(  6, "o== =  ===== === ==o")
renderLine(  7, "#== ======== ===  =#")
renderLine(  8, "#==o======== ======#")
renderLine(  9, "#====== = ==    ===#")
renderLine( 10, "#=== =  ======= ===#")
renderLine( 11, "#=== == ======= ===#")
renderLine( 12, "#o####o###o#######o#")

oxyd_shuffle()

set_actor("ac-blackball", 11.5, 5.5)










