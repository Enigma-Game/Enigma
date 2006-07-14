--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 25

create_world(levelw, levelh)
draw_border("st-greenbrown")
fill_floor("fl-leaves")
enigma.ConserveLevel = FALSE

oxyd ( 3,18)
oxyd (16,18)

draw_stones("st-grate1", {15, 2}, {1,1},3)
draw_stones("st-greenbrown", { 2, 3}, {1,0}, 3)
draw_stones("st-greenbrown_hole", { 5, 3}, {1,0}, 1)
draw_stones("st-greenbrown", { 5, 2}, {0,1}, 1)
draw_stones("st-greenbrown", { 1,12}, {1,0}, 5)
draw_stones("st-greenbrown", { 7,12}, {1,0}, 6)
draw_stones("st-greenbrown", { 4,10}, {1,0}, 2)
draw_stones("st-greenbrown", { 7,10}, {1,0}, 6)
draw_stones("st-greenbrown", {14,10}, {1,0}, 2)
draw_stones("st-greenbrown", {14,12}, {1,0}, 5)

set_stone("st-oneway-n", 1, 3)
set_stone("st-oneway-e", 5, 1)
set_stone("st-oneway-w", 4,11)
set_stone("st-oneway-e",15,11)
set_stone("st-oneway-n", 6,10)
set_stone("st-oneway-s",13,10)

fill_floor("fl-water",13,12, 1, 1)
fill_floor("fl-water", 8,13, 4,11)

set_item("it-seed", 5, 3)
set_item("it-coin1",17, 2)

set_stone("st-coinslot", 1, 1, {action="openclose", target="door1"})
doorh( 6,12, {name="door1"})

SetDefaultAttribs("it-wormhole", {range=1.0, strength=10})

set_item("it-wormhole", 5,18, {targetx="17.5",targety="23.5"})
set_item("it-wormhole",13,18, {targetx="2.5",targety="11.5"})

set_actor("ac-blackball", 2.5,11.5)
















