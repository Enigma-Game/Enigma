--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

stone="st-greenbrown"

create_world( levelw, levelh)
fill_floor("fl-sand", 0,0,levelw,levelh)

draw_border( stone)
draw_stones( stone, {6,1}, {0,1}, 6)
draw_stones( stone, {4,6}, {0,1}, 6)
draw_stones( stone, {11,5}, {1,0}, 8)
draw_stones( stone, {11,6}, {1,0}, 8)
draw_stones( stone, {11,7}, {1,0}, 8)
draw_stones( stone, {13,1}, {0,1}, 3)
draw_stones( stone, {13,9}, {0,1}, 3)

set_stone( "st-wood", 6, 7)
set_item( "it-hammer", 6, 7)
set_item( "it-magicwand", 8, 8)
set_stone( "st-stoneimpulse", 5,8)

mytimer=0
function timer_action()
    if mytimer == 0 then
        SendMessage("fart", "trigger")
        SendMessage("door1", "open")
        SendMessage("door2", "close")
    elseif mytimer == 1 then
        SendMessage("fart", "trigger")
        SendMessage("door1", "close")
        SendMessage("door2", "open")
    end
    mytimer = mytimer + 1
    if mytimer == 2 then
        mytimer = 0
    end
end

set_stone( "st-fart", 6, 3, {name="fart"})
doorv( 13, 4, {name="door1"})
doorv( 13, 8, {name="door2"})
set_stone( "st-timer", 13,6, { action="callback", target="timer_action", interval=5})

oxyd(1,1)
oxyd(1,11)
oxyd(18,1)
oxyd(18,11)
oxyd_shuffle()

set_actor("ac-blackball", 9, 8)










