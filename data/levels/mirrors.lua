-- Copyright (C) 2003 Daniel Heck 
-- Licensed under GPL v2.0 or above
levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-glass")
fill_floor("fl-gray")
-- draw_stones("st-glass", {1,1}, {1,0}, level_width-2)
-- draw_stones("st-glass", {1,1}, {0,1}, level_height-2)
-- draw_stones("st-glass", {1,levelh-2},{1,0}, levelw-2)
-- draw_stones("st-glass", {levelw-2,1},{0,1}, levelh-2)

draw_stones("st-glass", {1,5},{1,0},4)
draw_stones("st-glass", {1,7},{1,0},4)
doorv(1,6, {name="door1"})
doorv(2,6, {name="door2"})
doorv(3,6, {name="door3"})
doorv(4,6, {name="door4"})


function sdot1()
    SendMessage("door1", "openclose")
    SendMessage("door3", "openclose")
end
function sdot2()
    SendMessage("door2", "openclose")
end
function sdot3()
    SendMessage("door1", "openclose")
    SendMessage("door4", "openclose")
end
function sdot4()
    SendMessage("door3", "openclose")
    SendMessage("door4", "openclose")
end

shogundot1(10,8, {action="callback", target="sdot1"})
shogundot1(7,4, {action="callback", target="sdot2"})
shogundot1(7,8, {action="open", target="door2"})
shogundot1(13,4, {action="close", target="door4"})
shogundot1(13,8, {action="callback", target="sdot3"})
shogundot1(10,4, {action="callback", target="sdot4"})

set_stone("st-shogun", 7,6)
set_stone("st-shogun", 10,6)
set_stone("st-shogun", 13,6)

mirrorp(16,6,0,0,3)
mirror3(16,5,0,1,3)
mirror3(16,7,0,1,1)

mirrorp(19,1,0,0,2)
mirrorp(19,11,0,0,4)
mirrorp(0,1,0,0,4)
mirrorp(0,5,0,0,2)
mirrorp(0,7,0,0,4)
mirrorp(0,11,0,0,2)

mirrorp(19,11,0,0,4)

laser(0,6, TRUE, EAST)


oxyd(0,0)
oxyd(level_width-1,level_height-1)
oxyd(level_width-1, 0)
oxyd(0,level_height-1)
--oxyd_shuffle()

set_actor("ac-blackball", 10,8.5)

