--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 13)
if (difficult) then
    enigma.ConserveLevel=FALSE
end
oxyd_default_flavor = "a"

fill_floor( "fl-abyss", 0, 0, level_width, 2)
fill_floor( "fl-rough", 0, 2, level_width, level_height-2)
fill_floor( "fl-rough_medium", 5, 7, 10, 1)

draw_border("st-rock1", 0, 2, level_width, level_height-2)
draw_border("st-rock1", 2, 4, level_width-4, level_height-6)
draw_border("st-rock1", 4, 6, level_width-8, level_height-10)

set_stone("st-death", 4, 7)
set_stone("st-death",15, 7)

oxyd( 0, 7)
oxyd( 2, 4)
oxyd( 2, 7)
oxyd( 2,10)
oxyd( 4, 6)
oxyd( 4, 8)
oxyd(19, 7)
oxyd(17, 4)
oxyd(17, 7)
oxyd(17,10)
oxyd(15, 6)
oxyd(15, 8)
oxyd_shuffle()

set_actor("ac-blackball", 10, 7.5)
if (difficult) then
    set_actor("ac-whiteball-small", 10, 5.5, {player=0})
else
    set_actor("ac-killerball", 10, 5.5, {mouseforce=1,controllers=1})
end
set_actor("ac-whiteball-small", 10,11.5, {player=0, mouseforce=1})










