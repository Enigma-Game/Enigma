--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

enigma.ConserveLevel=FALSE                  -- \dh\

levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "a"
draw_border("st-rock5")
fill_floor("fl-samba")

oxyd ( 1, 0)
oxyd ( 2, 0)
oxyd (17, 0)
oxyd (18, 0)
oxyd ( 1,12)
oxyd ( 2,12)
oxyd (17,12)
oxyd (18,12)

oxyd_shuffle()

draw_stones("st-glass", { 4, 1}, {1,0},10)
draw_stones("st-glass", { 6,11}, {1,0},10)
draw_stones("st-glass", { 4, 6}, {1,0},12)
draw_stones("st-glass", { 1, 2}, {0,1}, 4)
draw_stones("st-glass", { 1, 7}, {0,1}, 3)
draw_stones("st-glass", {18, 3}, {0,1}, 3)
draw_stones("st-glass", {18, 7}, {0,1}, 4)

draw_stones("st-3mirror", { 4, 0}, {1,0},10, {transparent=1,orientation=1})
draw_stones("st-3mirror", { 6,12}, {1,0},10, {transparent=1,orientation=3})
draw_stones("st-3mirror", { 0, 2}, {0,1}, 4, {transparent=1,orientation=4})
draw_stones("st-3mirror", { 0, 7}, {0,1}, 3, {transparent=1,orientation=4})
draw_stones("st-3mirror", {19, 3}, {0,1}, 3, {transparent=1,orientation=2})
draw_stones("st-3mirror", {19, 7}, {0,1}, 4, {transparent=1,orientation=2})

set_item("it-coin4", 1, 6,{value=12})
set_item("it-coin4", 1,10,{value=12})
set_item("it-coin4",18, 2,{value=12})
set_item("it-coin4",18, 6,{value=12})

set_stone("st-coinslot",16, 6, {action="onoff", target="laser01"})
set_stone("st-coinslot", 3, 6, {action="onoff", target="laser02"})

set_attrib (mirrorp ( 6, 3, 1, 0,1), "name", "mirror01")
set_attrib (mirrorp ( 7, 3, 1, 0,1), "name", "mirror02")
set_attrib (mirrorp ( 8, 3, 1, 0,1), "name", "mirror03")
set_attrib (mirrorp ( 6, 4, 1, 0,1), "name", "mirror04")
set_attrib (mirrorp ( 8, 4, 1, 0,1), "name", "mirror05")
set_attrib (mirrorp ( 6, 5, 1, 0,1), "name", "mirror06")
set_attrib (mirrorp ( 7, 5, 1, 0,1), "name", "mirror07")
set_attrib (mirrorp ( 8, 5, 1, 0,1), "name", "mirror08")
set_attrib (mirrorp (11, 7, 1, 0,2), "name", "mirror09")
set_attrib (mirrorp (12, 7, 1, 0,2), "name", "mirror10")
set_attrib (mirrorp (13, 7, 1, 0,2), "name", "mirror11")
set_attrib (mirrorp (11, 8, 1, 0,2), "name", "mirror12")
set_attrib (mirrorp (13, 8, 1, 0,2), "name", "mirror13")
set_attrib (mirrorp (11, 9, 1, 0,2), "name", "mirror14")
set_attrib (mirrorp (12, 9, 1, 0,2), "name", "mirror15")
set_attrib (mirrorp (13, 9, 1, 0,2), "name", "mirror16")

set_stone( "st-timer", 7, 4, {action="callback", target="t_h1"})
set_stone( "st-timer",12, 8, {action="callback", target="t_h2"})

set_attrib(laser( 16, 2, TRUE, WEST), "name", "laser01")
set_attrib(laser(  3,10, TRUE, EAST), "name", "laser02")

timer = 0

function t_h1()
    if timer == 1 then
        SendMessage("mirror01", "turn")
        SendMessage("mirror02", "turn")
        SendMessage("mirror03", "turn")
        SendMessage("mirror04", "turn")
        SendMessage("mirror05", "turn")
        SendMessage("mirror06", "turn")
        SendMessage("mirror07", "turn")
        SendMessage("mirror08", "turn")
    end
end

function t_h2()
    if timer == 1 then
        SendMessage("mirror09", "turn")
        SendMessage("mirror10", "turn")
        SendMessage("mirror11", "turn")
        SendMessage("mirror12", "turn")
        SendMessage("mirror13", "turn")
        SendMessage("mirror14", "turn")
        SendMessage("mirror15", "turn")
        SendMessage("mirror16", "turn")
    end

    timer = timer + 1
    if timer ==  2 then
        timer = 0
    end
end

document(3,11, "Danger...laser beams")

set_actor("ac-blackball", 1.5,11.5)

