-- light barrier, a level for Enigma
-- Copyright (C) 2005 Richi Buetzer
-- Licensed under GPL v2.0 or above

create_world(20, 13)

fill_floor("fl-bluegray", 0,0,20,13)
fill_floor("fl-water",4 ,1,2,11)
fill_floor("fl-water",8 ,3,2,2)
fill_floor("fl-water",8 ,9,3,3)
fill_floor("fl-water",11 ,6,2,2)
fill_floor("fl-water",14 ,9,2,2)

draw_border("st-wood")
set_stone("st-laser", 6, 0, {name="laser1", dir=SOUTH, on="1"})
set_stone("st-switch", 9,11, {action="onoff", target="laser1"})

set_stone("st-fart", 0,12, {name="fart1"})
set_stone("st-timer", 0,0, {action="trigger", target="fart1", interval=10})

set_stone( "st-wood", 10, 3)
set_stone( "st-wood", 10, 4)
set_stone( "st-wood", 13, 6)
set_stone( "st-wood", 13, 7)
set_stone( "st-wood", 16, 9)
set_stone( "st-wood", 16, 10)

local ac1=set_actor("ac-blackball", 16.5,3.5, {player=0, mouseforce="1"})

oxyd(1,1)
oxyd(1,3)
oxyd(1,5)
oxyd(1,7)
oxyd(1,9)
oxyd(1,11)
oxyd_default_flavor = "c"
oxyd_shuffle()


























