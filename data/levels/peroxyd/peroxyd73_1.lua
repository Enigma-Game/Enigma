--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

create_world( 20, 13)
draw_border("st-rock4")
fill_floor("fl-himalaya")

oxyd_default_flavor = "d"

oxyd( 1, 0)
oxyd(12, 0)
oxyd(18, 0)
oxyd( 1,12)
oxyd( 4,12)
oxyd(18,12)

oxyd_shuffle()

fill_floor("fl-water", 4, 8, 1,1)
fill_floor("fl-water", 8, 6, 1,1)
fill_floor("fl-water",11, 4, 1,1)
fill_floor("fl-water",13,10, 1,2)
fill_floor("fl-water",14, 3, 1,1)

draw_stones( "st-stoneimpulse", { 2, 5}, {0,1}, 4)
draw_stones( "st-stoneimpulse", { 2, 9}, {1,0}, 2)
draw_stones( "st-stoneimpulse", { 4, 3}, {0,1}, 5)
draw_stones( "st-stoneimpulse", { 4, 9}, {0,1}, 2)
draw_stones( "st-stoneimpulse", { 6, 3}, {1,0}, 2)
draw_stones( "st-stoneimpulse", { 6, 5}, {0,1}, 4)
draw_stones( "st-stoneimpulse", { 6,10}, {0,1}, 2)
draw_stones( "st-stoneimpulse", { 7,10}, {1,0}, 2)
draw_stones( "st-stoneimpulse", { 8, 2}, {0,1}, 2)
draw_stones( "st-stoneimpulse", { 8, 5}, {1,0}, 2)
draw_stones( "st-stoneimpulse", { 8, 7}, {0,1}, 3)
draw_stones( "st-stoneimpulse", {10, 7}, {1,0}, 4)
draw_stones( "st-stoneimpulse", {10, 9}, {1,0}, 5)
draw_stones( "st-stoneimpulse", {11, 1}, {0,1}, 3)
draw_stones( "st-stoneimpulse", {13, 1}, {0,1}, 6)
draw_stones( "st-stoneimpulse", {15, 4}, {0,1}, 5)
draw_stones( "st-stoneimpulse", {17, 2}, {0,1},10)
set_stones("st-stoneimpulse", {{2,1},{2,11},{6,1},{11,5}})
draw_stones( "st-rock4", { 2, 3}, {1,0}, 2)
set_stones("st-rock4", {{10,5},{12,10},{15,3},{15,9}})
set_stones("st-wood", {{5,4},{8,4},{9,3},{10,2},{15,10}})
set_stones("st-block", {{5,3},{12,4},{12,11},{14,4}})

set_stone("st-door", 1, 1, {name="door1", type="h"})
set_item("it-trigger", 1, 9, {action="openclose", target="door1"})
set_stone("st-door", 2, 2, {name="door2", type="v"})
set_item("it-trigger",14, 8, {action="openclose", target="door2"})
set_stone("st-door",18,11, {name="door3", type="h"})
set_item("it-trigger",15,11, {action="callback", target="xx1"})
set_floor("fl-bridge",16, 3, {name="bridge1", type="y"})
set_floor("fl-bridge",18, 4, {name="bridge2", type="y"})

function xx1()
       doora=enigma.GetNamedObject("door3")
       bridgea=enigma.GetNamedObject("bridge1")
       bridgeb=enigma.GetNamedObject("bridge2")
       SendMessage(doora, "openclose")
       SendMessage(bridgea, "openclose")
       SendMessage(bridgeb, "openclose")
end

set_actor("ac-blackball", 3.5, 1.5)











