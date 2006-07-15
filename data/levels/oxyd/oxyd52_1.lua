--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 39
levelh = 13

create_world(levelw, levelh)
draw_border( "st-greenbrown")
fill_floor("fl-space")

enigma.AddConstantForce(0,20)

oxyd(3,5)
oxyd(7,3)
oxyd(6,9)
oxyd(11,6)
oxyd(15,10)
oxyd(16,4)
oxyd(21,5)
oxyd(25,3)
oxyd(24,10)
oxyd(29,6)
oxyd(33,9)
oxyd(35,4)

oxyd_shuffle()

draw_stones("st-death", {1,12}, {1,0},37)

set_stones("st-greenbrown", {{2,5},{4,5},{6,3},{8,3},{5,9},{7,9}})
set_stones("st-greenbrown", {{10,6},{12,6},{14,10},{16,10},{15,4},{17,4}})
set_stones("st-greenbrown", {{20,5},{22,5},{24,3},{26,3},{23,10},{25,10}})
set_stones("st-greenbrown", {{28,6},{30,6},{32,9},{34,9},{34,4},{36,4}})

fill_floor("fl-sand", 2,3, 3,2)
fill_floor("fl-sand", 6,1, 3,2)
fill_floor("fl-sand", 5,7, 3,2)
fill_floor("fl-sand", 10,4, 3,2)
fill_floor("fl-sand", 14,8, 3,2)
fill_floor("fl-sand", 15,2, 3,2)
fill_floor("fl-sand", 20,3, 3,2)
fill_floor("fl-sand", 24,1, 3,2)
fill_floor("fl-sand", 23,8, 3,2)
fill_floor("fl-sand", 28,4, 3,2)
fill_floor("fl-sand", 32,7, 3,2)
fill_floor("fl-sand", 34,2, 3,2)

-- document(12,5, "Landezone nicht begehbar")

set_actor("ac-blackball", 10.5,4.5)















