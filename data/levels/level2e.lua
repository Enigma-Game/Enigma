--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-greenbrown")
fill_floor("fl-gray")

enigma.AddConstantForce(0,50)

oxyd(0,1)
oxyd(18,8)
oxyd(14,6)
oxyd(19,1)
oxyd(3,5)
oxyd(1,8)
oxyd(6,3)
oxyd(14,3)
oxyd_shuffle()

draw_stones("st-greenbrown", {2,3}, {1,0},2)
draw_stones("st-greenbrown", {17,3}, {1,0},2)
draw_stones("st-greenbrown", {5,8}, {1,0},3)
draw_stones("st-greenbrown", {12,8}, {1,0},4)
draw_stones("st-greenbrown", {1,10}, {1,0},4)
draw_stones("st-greenbrown", {17,10}, {1,0},2)
set_stones("st-greenbrown", {{8,3},{10,3},{12,3}})

oneway(7,3, enigma.NORTH)
oneway(9,3, enigma.NORTH)
oneway(11,3, enigma.NORTH)
oneway(13,3, enigma.NORTH)

draw_stones("st-death", {1, 0}, {1,0},18)
set_stone("st-death", 10,9)
set_stone("st-death", 1,3)
set_stone("st-death", 19,2)

--document(9,11, "Vorsicht, Lebensgefahr")

set_actor("ac-blackball", 9.5,4.5)
