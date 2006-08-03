--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "c"
draw_border("st-rock1")
fill_floor("fl-bluegreen")

oxyd( 5, 2)
oxyd( 9, 2)
oxyd( 10, 2)
oxyd( 14, 2)
oxyd( 5, 10)
oxyd( 9, 10)
oxyd( 10, 10)
oxyd( 14, 10)
oxyd_shuffle()

set_stone( "st-timer", 2, 2, {action="callback", target="t_h1"})
set_stone( "st-timer", 17, 2, {action="callback", target="t_h2"})
set_stone( "st-timer", 17, 10, {action="callback", target="t_h3"})

hammer(5,6)

draw_stones("st-thief", {2,5}, {1,0},16)
draw_stones("st-thief", {2,7}, {1,0},16)

draw_stones("st-stone_break", {4,1}, {1,0},12)
draw_stones("st-stone_break", {4,3}, {1,0},12)
draw_stones("st-stone_break", {4,9}, {1,0},12)
draw_stones("st-stone_break", {4,11}, {1,0},12)
draw_stones("st-stone_break", {6,2}, {1,0},3)
draw_stones("st-stone_break", {11,2}, {1,0},3)
draw_stones("st-stone_break", {4,2}, {1,0},1)
draw_stones("st-stone_break", {15,2}, {1,0},1)
draw_stones("st-stone_break", {6,10}, {1,0},3)
draw_stones("st-stone_break", {11,10}, {1,0},3)
draw_stones("st-stone_break", {4,10}, {1,0},1)
draw_stones("st-stone_break", {15,10}, {1,0},1)

draw_stones("st-grate2", {2,6},{1,0},2)
draw_stones("st-grate2", {16,6},{1,0},2)

oneway(1,5, enigma.SOUTH)
oneway(1,7, enigma.NORTH)
oneway(18,5, enigma.NORTH)
oneway(18,7, enigma.SOUTH)

fill_floor("fl-abyss", 1,1, 5,3)
fill_floor("fl-abyss", 14,1, 5,3)
fill_floor("fl-abyss", 14,9, 5,3)
fill_floor("fl-abyss", 3,9, 3,3)
fill_floor("fl-abyss", 7,2, 6,2)
fill_floor("fl-abyss", 7,9, 6,2)
fill_floor("fl-abyss", 1,11, 2,1)
fill_floor("fl-abyss", 2,9, 1,1)

timer = 0

function t_h1()
    doora=enigma.GetNamedObject("door1")
    doorb=enigma.GetNamedObject("door2")
    doorc=enigma.GetNamedObject("door3")
    doord=enigma.GetNamedObject("door4")

    if timer == 0 then
        SendMessage(doora, "open")
     end
    if timer == 1 then
        SendMessage(doorb, "open")
    end
    if timer == 2 then
        SendMessage(doorc, "open")
    end
    if timer == 3 then
        SendMessage(doord, "open")
        SendMessage(doora, "close")
    end
    if timer == 4 then
        SendMessage(doorb, "close")
    end
    if timer == 5 then
        SendMessage(doorc, "close")
    end
    if timer == 6 then
        SendMessage(doord, "close")
    end
end

function t_h2()
    doora=enigma.GetNamedObject("door5")
    doorb=enigma.GetNamedObject("door6")
    doorc=enigma.GetNamedObject("door7")
    doord=enigma.GetNamedObject("door8")

    if timer == 4 then
        SendMessage(doora, "open")
    end
    if timer == 5 then
        SendMessage(doorb, "open")
    end
    if timer == 6 then
        SendMessage(doorc, "open")
    end
    if timer == 7 then
        SendMessage(doord, "open")
        SendMessage(doora, "close")
    end
    if timer == 8 then
        SendMessage(doorb, "close")
    end
    if timer == 9 then
        SendMessage(doorc, "close")
    end
    if timer == 10 then
        SendMessage(doord, "close")
    end
end

function t_h3()
    doora=enigma.GetNamedObject("door9")
    doorb=enigma.GetNamedObject("door10")
    doorc=enigma.GetNamedObject("door11")
    doord=enigma.GetNamedObject("door12")

    if timer == 8 then
        SendMessage(doora, "open")
    end
    if timer == 9 then
        SendMessage(doorb, "open")
    end
    if timer == 10 then
        SendMessage(doorc, "open")
    end
    if timer == 11 then
        SendMessage(doord, "open")
        SendMessage(doora, "close")
    end
    if timer == 0 then
        SendMessage(doorb, "close")
    end
    if timer == 1 then
        SendMessage(doorc, "close")
    end
    if timer == 2 then
        SendMessage(doord, "close")
    end

    timer = timer + 1
    if timer == 12 then
        timer = 0
    end
end

set_stone("st-door_b", 4,6, {name="door1", type="v"})
set_stone("st-door_b", 5,6, {name="door2", type="v"})
set_stone("st-door_b", 6,6, {name="door3", type="v"})
set_stone("st-door_b", 7,6, {name="door4", type="v"})
set_stone("st-door_b", 8,6, {name="door5", type="v"})
set_stone("st-door_b", 9,6, {name="door6", type="v"})
set_stone("st-door_b", 10,6, {name="door7", type="v"})
set_stone("st-door_b", 11,6, {name="door8", type="v"})
set_stone("st-door_b", 12,6, {name="door9", type="v"})
set_stone("st-door_b", 13,6, {name="door10", type="v"})
set_stone("st-door_b", 14,6, {name="door11", type="v"})
set_stone("st-door_b", 15,6, {name="door12", type="v"})

set_actor("ac-blackball", 2.5, 10.5)



















