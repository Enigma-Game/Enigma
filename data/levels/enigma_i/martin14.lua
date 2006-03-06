--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw =  20
levelh = 13

create_world( levelw, levelh)

fill_floor("fl-gray", 0,0,levelw,levelh)
stone = "st-glass"

draw_border( stone)

mytimer = 1
function timer_handler()
    laser1=enigma.GetNamedObject("laser1")
    laser2=enigma.GetNamedObject("laser2")
    laser3=enigma.GetNamedObject("laser3")

    if mytimer == 0 then
        SendMessage(laser1, "on")
        if (difficult) then SendMessage(laser2, "off")
        else SendMessage(laser3, "off")
        end
    end
    if mytimer == 1 then
        SendMessage(laser2, "on")
        if (difficult) then SendMessage(laser3, "off")
        else SendMessage(laser1, "off")
        end
    end
    if mytimer == 2 then
        SendMessage(laser3, "on")
        if (difficult) then SendMessage(laser1, "off")
        else SendMessage(laser2, "off")
        end
    end
    mytimer = mytimer + 1
    if mytimer == 3 then
        mytimer = 0
    end
end

set_attrib( laser( 2, 2, TRUE, EAST), "name", "laser1")
laser1 = enigma.GetNamedObject("laser1")

set_attrib( laser( 2, 3, FALSE, EAST), "name", "laser2")
laser2 = enigma.GetNamedObject("laser2")

set_attrib( laser( 2, 4, FALSE, EAST), "name", "laser3")
laser3 = enigma.GetNamedObject("laser3")

mirrorp( 18, 2, FALSE, FALSE, 2)
mirrorp( 17, 3, FALSE, FALSE, 2)
mirrorp( 16, 4, FALSE, FALSE, 2)
mirrorp( 18, 5, FALSE, TRUE, 4)
mirrorp( 17, 6, FALSE, TRUE, 4)
mirrorp( 16, 7, FALSE, TRUE, 4)
mirrorp( 18, 8, FALSE, FALSE, 4)
mirrorp( 17, 9, FALSE, FALSE, 4)
mirrorp( 16,10, FALSE, FALSE, 4)

set_stone( "st-timer", 0, 0, {action="callback", target="timer_handler"})

oxyd( 2, 0)
oxyd( 6, 0)
oxyd(10, 0)
oxyd(14, 0)

oxyd( 2,12)
oxyd( 6,12)
oxyd(10,12)
oxyd(14,12)

oxyd_shuffle()

fakeoxyd( 2, 5)
fakeoxyd( 2, 6)
fakeoxyd( 2, 7)
fakeoxyd( 2, 8)
fakeoxyd( 2, 9)
fakeoxyd( 2,10)

oneway( 1, 2, enigma.NORTH)
oneway( 3, 1, enigma.EAST)
oneway( 7, 1, enigma.EAST)
oneway(11, 1, enigma.EAST)
oneway( 3,11, enigma.WEST)
oneway( 7,11, enigma.WEST)
oneway(11,11, enigma.WEST)

draw_stones( stone, {16,11},{-1,-1}, 5)
draw_stones( stone, {18,1}, {-1,1}, 6)
draw_stones( stone, {3,2},{1,1},9)
draw_stones( stone, {3,10},{1,-1},9)
set_stone( stone, 7,10)
set_stone( stone, 7,2)
set_stone( stone,11,2)

set_actor("ac-blackball", 1.5, 1.5)
timer_handler()
timer_handler()















