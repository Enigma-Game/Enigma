--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)
fill_floor( "fl-leavesb", 0, 0, levelw, levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c == "o" then
            oxyd( i-1, line)
	elseif c == "#" then
	    set_stone( "st-brick", i-1, line)
        elseif c == "*" then
            set_stone( "st-glass", i-1, line)
	elseif c == "!" then
	    set_stone( "st-brick_magic", i-1, line)
        end
    end
end

renderLine(  0, "#o#######o########o#")
renderLine(  1, "#!     !*!        !#")
renderLine(  2, "#*!!!! !**!!!!!!  !#")
renderLine(  3, "#  ! ! !*     !  ! #")
renderLine(  4, "#  !   !!!!!! !  !*#")
renderLine(  5, "#!   !      ! ! !!!#")
renderLine(  6, "#*!! !!!! !        #")
renderLine(  7, "#!   !      ! ! !!!#")
renderLine(  8, "#  !   !!!!!! !  !*#")
renderLine(  9, "#  ! ! !*     !  ! #")
renderLine( 10, "#*!!!! !**!!!!!!  !#")
renderLine( 11, "#!     !*!        !#")
renderLine( 12, "#o#######o########o#")

oxyd_shuffle()

set_item( "it-magicwand", 6,7)
set_stone( "st-switch", 19,6,{on=TRUE,action="onoff", target="laser"})
set_attrib( laser(0,6,TRUE, EAST), "name", "laser")
mirrorp( 1,3,FALSE,FALSE,4)
mirrorp( 1,9,FALSE,FALSE,4)
mirrorp( 9,3,FALSE,FALSE,4)
mirrorp( 9,9,FALSE,FALSE,4)
mirror3( 9,6,FALSE,TRUE,2)
mirror3(18,3,FALSE,FALSE,2)
mirror3(18,9,FALSE,FALSE,2)

document( 15,8, "Do not use too much magic!")
set_actor("ac-blackball", 15.5, 9.5)
