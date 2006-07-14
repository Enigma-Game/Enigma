--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world( levelw, levelh)
oxyd_default_flavor="a"
fill_floor("fl-acblack", 0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-stone1", i-1, line)
        elseif c == "o" then
            oxyd( i-1, line)
        elseif c == "m" then
            set_stone( "st-wood-growing", i-1, line)
        end
    end
end

-- Floor:   " "
-- Border:  "#"
-- Oxyd:    "o"
-- Movable: "m"

renderLine(  0, "####################")
renderLine(  1, "#   =   #o##########")
renderLine(  2, "# ###m  #          #")
renderLine(  3, "# ###  m##########|#")
renderLine(  4, "#o#  m m ##      = #")
renderLine(  5, "### # ## #o ###### #")
renderLine(  6, "#   # ## ####  ss# #")
renderLine(  7, "# m  m         ss# #")
renderLine(  8, "##### ### # #  ss# #")
renderLine(  9, "#  o#     #|###### #")
renderLine( 10, "#|########>        #")
renderLine( 11, "#           <#######")
renderLine( 12, "####################")

set_stone("st-door", 4, 1 , {name="door1", type="v"})
set_item("it-trigger", 15,6, {action="openclose", target="door1"})

set_stone("st-door", 17, 4 , {name="door2", type="v"})
set_item("it-trigger", 16,6, {action="openclose", target="door2"})

set_attrib(laser(12,11, TRUE, WEST), "name", "laser1")
set_item("it-trigger", 15,7, {action="onoff", target="laser1"})

set_stone("st-door", 18, 3 , {name="door4", type="h"})
set_item("it-trigger", 16,7, {action="openclose", target="door4"})

set_attrib(laser(10,10, TRUE, EAST), "name", "laser2")
set_item("it-trigger", 15,8, {action="onoff", target="laser2"})

set_stone("st-door", 1, 10 , {name="door6", type="h"})
set_item("it-trigger", 16,8, {action="openclose", target="door6"})



oxyd_shuffle()



set_actor("ac-blackball", 11.5, 8.5)
















