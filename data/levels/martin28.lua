--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)
fill_floor( "fl-plank", 0, 0, levelw, levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c == "o" then
            oxyd( i-1, line)
	elseif c == "#" then
	    set_stone( "st-brick", i-1, line)
        end
    end
end

renderLine(  0, "############o#######")
renderLine(  1, "#   #  ##  # ##  # #")
renderLine(  2, "#                  #")
renderLine(  3, "## ## ###  # ##  # #")
renderLine(  4, "#  #      #      #o#")
renderLine(  5, "## # ####  # ### ###")
renderLine(  6, "##    o#           #")
renderLine(  7, "#  ###o# # # ### ###")
renderLine(  8, "##   # #         #o#")
renderLine(  9, "##   # # # # ### # #")
renderLine( 10, "#    # #           #")
renderLine( 11, "## #         # # # #")
renderLine( 12, "##o###########o#o###")

oxyd_shuffle()

shogundot1( 6, 4, {action="openclose", target="door1"})
shogundot1( 7, 4, {action="openclose", target="door2"})
shogundot1( 8, 4, {action="openclose", target="door3"})
shogundot1( 9, 4, {action="openclose", target="door4"})
shogundot1(10, 3, {action="openclose", target="door6"})
shogundot1(10, 5, {action="openclose", target="door7"})
shogundot1( 8, 6, {action="openclose", target="door8"})
shogundot1(11, 6, {action="openclose", target="door5"})

set_stone( "st-shogun-s", 3, 6)
set_stone( "st-shogun-s",11, 2)
set_stone( "st-shogun-s",13, 6)

doorh( 2,11, {name="door1"})
doorv( 5, 6, {name="door2"})
doorh( 6, 8, {name="door3"})
doorh(14,11, {name="door4"})
doorh(18, 3, {name="door5"})
doorh(12, 1, {name="door6"})
doorh(18, 9, {name="door7"})
doorh(16,11, {name="door8"})


set_actor("ac-blackball", 9.5, 5.5)
