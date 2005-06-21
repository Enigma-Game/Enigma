--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)

fill_floor("fl-water", 0,0,levelw,levelh)

oxyd_default_flavor = "d"

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-yinyang2", i-1, line)
            set_floor( "fl-wood", i-1, line)
        elseif c == "o" then
            oxyd( i-1, line)
        elseif c == "=" then
            set_floor( "fl-wood", i-1, line)
        end
    end
end

renderLine(  0, " o                  ")
renderLine(  1, "o################## ")
renderLine(  2, " #  #  #o   #  # o# ")
renderLine(  3, " #  #  #    #  #  # ")
renderLine(  4, " #  #  #    #  #  # ")
renderLine(  5, " #  #o #    #  #  # ")
renderLine(  6, " #######====####### ")
renderLine(  7, " #  #  #    # o#  # ")
renderLine(  8, " #  #  #    #  #  # ")
renderLine(  9, " #  #  #    #  #  # ")
renderLine( 10, " #o #  #   o#  #  # ")
renderLine( 11, " ##################o")
renderLine( 12, "                  o ")

oxyd_shuffle()

set_item("it-yinyang", 9, 6)
set_item("it-yinyang",10, 6)

set_actor("ac-whiteball", 9.5, 6.5)
set_actor("ac-blackball",10.5, 6.5)
