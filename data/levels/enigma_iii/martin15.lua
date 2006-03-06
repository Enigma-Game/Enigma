--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)

fill_floor("fl-concrete", 0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-glass1", i-1, line)
        elseif c == "o" then
            oxyd( i-1, line)
        elseif c == "x" then
            fakeoxyd( i-1, line)
        elseif c == "$" then
            set_stone( "st-death", i-1, line)
        end
    end
end

-- Floor:   " "
-- Border:  "#"
-- Oxyd:    "o"
-- Death:   "$"

renderLine(  0, "####o###o##o###o####")
renderLine(  1, "#x    x      x    x#")
renderLine(  2, "# xx xxx xx xxx xx #")
renderLine(  3, "# x   x      x   x #")
renderLine(  4, "o   $   x  x   $   o")
renderLine(  5, "# x   x      x   x #")
renderLine(  6, "#xxx    $$$$    xxx#")
renderLine(  7, "# x   x      x   x #")
renderLine(  8, "o   $   x  x   $   o")
renderLine(  9, "# x   x      x   x #")
renderLine( 10, "# xx xxx xx xxx xx #")
renderLine( 11, "#x    x      x    x#")
renderLine( 12, "####o###o##o###o####")

oxyd_shuffle()

set_actor("ac-blackball", 10.5, 5)















