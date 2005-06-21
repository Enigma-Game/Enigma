-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Space Tubes

levelw=50
levelh=30

create_world( levelw, levelh)

fill_floor("fl-bluegreen", 0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-greenbrown", i-1, line)
        elseif c == "o" then
            oxyd( i-1, line)
        elseif c == "!" then
            abyss(i-1,line)
        elseif c == "=" then
            fill_floor("fl-bluegreen", i-1,line, 1,1)
        elseif c == "+" then
            fill_floor("fl-space", i-1,line, 1,1)
        elseif c == "a" then
            set_stone("st-switch", i-1,line, {action="openclose", target="door1"})
        elseif c == "A" then
            set_stone("st-door", i-1, line , {name="door1", type="h"})
        elseif c == "z" then
            set_actor("ac-blackball", i-1-.5,line+.5)
        elseif c == "d" then --1-d
            set_floor("fl-gradient",  i-1,  line, {type=1})
        elseif c == "u" then --2-u
            set_floor("fl-gradient",  i-1,  line, {type=2})
        elseif c == "r" then --3-r
            set_floor("fl-gradient",  i-1,  line, {type=3})
        elseif c == "l" then --4-l
            set_floor("fl-gradient",  i-1,  line, {type=4})
        elseif c == "1" then --ur
            set_floor("fl-gradient",  i-1,  line, {type=6})
        elseif c == "3" then --dl
            set_floor("fl-gradient",  i-1,  line, {type=5})
        elseif c == "7" then --dr
            set_floor("fl-gradient",  i-1,  line, {type=8})
        elseif c == "9" then --ul
            set_floor("fl-gradient",  i-1,  line, {type=7})
        elseif c == "2" then --small ur
            set_floor("fl-gradient",  i-1,  line, {type=11})
        elseif c == "4" then --small dl
            set_floor("fl-gradient",  i-1,  line, {type=9})
        elseif c == "6" then --small dr
            set_floor("fl-gradient",  i-1,  line, {type=12})
        elseif c == "8" then --small ul
            set_floor("fl-gradient",  i-1,  line, {type=10})
        end
    end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"#######################################")
renderLine(01,"#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!#")
renderLine(02,"#!a+==llrr==rr++ll=======r==rr++ll===!#")
renderLine(03,"#!!!!!!!!!!!!!!!!!!!!!!==r==rr++ll=+=!#")
renderLine(04,"#!3ddd1!!3ddddd!!dddd1!!!r==rr++ll+++!#")
renderLine(05,"#!r===2dd4++++++++===l!!!!!#A#!!!!rul!#")
renderLine(06,"#!r=z=++++++++++++===l!3dd1#=#3d1!rul!#")
renderLine(07,"#!r===6uu8++++++++===l!r++++++++l!rul!#")
renderLine(08,"#!9uuu7!!9uuuuu!!r+++l!!u8++3d1+!!rul!#")
renderLine(09,"#!!!!!!!!!!!!!!!!98+67!!+r++r+l+!!rul!#")
renderLine(10,"#!==r=l+=r+llll=!!r+l!!!d4++9u7+!!rul!#")
renderLine(11,"#!=!!!!!!!!!!!!=!!r+l!!!+3d1++6u!!rul!#")
renderLine(12,"#!===!!!!3dddd4+l!r+l!!!+r+l++l+!!rul!#")
renderLine(13,"#!===!!!!r++++++l!r+l!!!+9u7++2d!!rul!#")
renderLine(14,"#!===!!!!r+6uuuu7!r+l!!!++3dd1++!!rul!#")
renderLine(15,"#!8+6!!!!r+l!!!!!!r+l!!!l+r++l+r!!rul!#")
renderLine(16,"#!r+l!!!!4+2!!!!!!!+!!!!l+9uu7+r!!rul!#")
renderLine(17,"#!r+l!!!!===!##!13===8!!++++++++!!rul!#")
renderLine(18,"#!r+l!!!!===ddu+++===r!r========l!rul!#")
renderLine(19,"#!r+l!!!!===uud+++===4!o========o!rul!#")
renderLine(20,"#!r+2dd1!!!!!!!!!!!!!!!!!!!!!!!!!!rul!#")
renderLine(21,"#!r++++2ddddddlllddddudddduudddudd4ul!#")
renderLine(22,"#!9uuuuuuuuuuullluuuuduuuudduuuuuduu7!#")
renderLine(23,"#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!#")
renderLine(24,"#######################################")

oxyd_shuffle()
display.SetFollowMode(display.FOLLOW_SCROLLING)

